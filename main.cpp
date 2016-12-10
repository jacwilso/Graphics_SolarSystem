// HEADERS /////////////////////////////////////////////////////////////////////

#include <GL/glew.h>

#ifdef __APPLE__
    #include <ALUT/alut.h>
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
    #include <GLUT/glut.h>
#else
/////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////
    /*#include <AL/alut.h>
    #include <AL/al.h>
    #include <AL/alc.h>*/
    #include <GL/glut.h>
#endif

#include <SOIL/SOIL.h>

// C Libraries we need
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// C++ Libraries we'll use
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

// Headers We've Written
#include "helper.h"
#include "Shader_Utils.h"

#include "camera.h"
#include "cubemap.h"
#include "sound.h"
#include "planet.h"
#include "solar_system.h"

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth = 640;
static size_t windowHeight = 480;
static float aspectRatio;

GLint leftMouseButton, rightMouseButton;    // status of the mouse buttons
int mouseX = 0, mouseY = 0;                 // last known X and Y of the mouse
int keyState[256];

/******** MENU ********/
GLint menuId;			// handle for our menu

/******** Camera View ********/
int camera=0;
Camera cam;
Point newPos, newLook;

/******** SOUND ********/
int wavSrcs = 0;
/////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////
// Sound wav( wavSrcs ); // # srcs
bool soundOff=true;

/******** SKYBOX ********/
string skyFolder = "skybox1/craterlake";
string skyExt = ".tga";
string skyFace[6] = {"_ft","_rt","_dn","_up","_bk","_lf"};
CubemapTexture sky;

/******** PICKING ********/
#define PICK_TOL 10.
#define PICK_BUFFER_SIZE 256
unsigned int PickBuffer[PICK_BUFFER_SIZE];
int RenderMode;
int Nhits, dx, dy, nitems, zmin, zmax, item, d;
GLint GrWindow;
bool Debug=true;

/******** SHADER ********/
GLuint shaderProgramHandle = 0;

/******** PLANET ********/
Solar_System solar;
//vector<Planet*> solar_sys;

// resizeWindow() //////////////////////////////////////////////////////////////
void resizeWindow(int w, int h) {
  aspectRatio = w / (float)h;

  windowWidth = w;
  windowHeight = h;

  //update the viewport to fill the window
  glViewport(0, 0, w, h);

  //update the projection matrix with the new window properties
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,aspectRatio,0.1,100000);
}

// initScene() /////////////////////////////////////////////////////////////////
void initScene()  {
  glEnable(GL_DEPTH_TEST);

  //******************************************************************
  // this is some code to enable a default light for the scene;
  // feel free to play around with this, but we won't talk about
  // lighting in OpenGL for another couple of weeks yet.
  float lightCol[4] = { 1, 1, 1, 1};
  float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
  float lPosition[4] = { 10, 10, 10, 1 };
  glLightfv( GL_LIGHT0, GL_POSITION,lPosition );
  glLightfv( GL_LIGHT0, GL_DIFFUSE,lightCol );
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );

  glDisable(GL_CULL_FACE);
  glEnable(GL_NORMALIZE);
  glEnable( GL_COLOR_MATERIAL );
  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
  //******************************************************************
  glInitNames();
  RenderMode = GL_RENDER;
  
  glShadeModel(GL_SMOOTH);

  // Camera Initialization

  cam.camLook = Point(0,0,0);
  cam.camPos = Point(500,500,500);
  cam.recomputeOrientation();
  newPos = cam.camPos;
  newLook = cam.camLook;
  
  // Skybox Intialization
  for(int i=0; i<6; i++)
    skyFace[i]=skyFolder+skyFace[i]+skyExt;
  sky.Load(skyFace);
  
  // Solar System Initialization
  solar.startTime();

  //Sound
  /////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////
  //wav.positionListener(person pos, cam direction,0,1,0);
  /*for(int i=0; i<wavSrcs; i++) // move to render if dynamic
    wav.positionSource(wav.sources[i], sound pos);*/

}

// renderScene() ///////////////////////////////////////////////////////////////
void renderScene(void)  {
  glClearColor(0,0,0,1);
  glDrawBuffer( GL_BACK );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  /** PICKING **/
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT,viewport);
  
  dx=glutGet(GLUT_WINDOW_WIDTH);
  dy=glutGet(GLUT_WINDOW_HEIGHT);

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  
  if(RenderMode==GL_SELECT)
    gluPickMatrix(double(mouseX), (double)(dy-mouseY), PICK_TOL, PICK_TOL, viewport);

  gluPerspective(45.0,aspectRatio,0.1,100000);

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  cam.gluLook(); 
  //cam.setLookAt(lookAtX,lookAtY+3.7,lookAtZ); // set the camera's look at

  glPushMatrix();

    //drawGrid();
    
    /*
    glPushMatrix();
      glScalef(1000,1000,1000);
      sky.cubeTexture();
    glPopMatrix();
    */

    // Solar System render here
    glPushName(1);
    glPushMatrix();
      glTranslatef( solar.position.getX(), solar.position.getY(), solar.position.getZ() );
      solar.draw();
    glPopMatrix();
    glPopName();
    //if solar.pick[i] do something

  if(RenderMode == GL_RENDER)
    glutSwapBuffers();
}

// mouseCallback() /////////////////////////////////////////////////////////////
void mouseCallback(int button, int state, int thisX, int thisY) {
  cam.mouseX = thisX;
  cam.mouseY = thisY;

  if(button==GLUT_LEFT_BUTTON && glutGetModifiers()==GLUT_ACTIVE_SHIFT && state==GLUT_DOWN){
    RenderMode=GL_SELECT;
    glRenderMode(RenderMode);
    renderScene();
    renderScene();
    //renderScene();
    RenderMode=GL_RENDER;
    Nhits=glRenderMode(RenderMode);
#ifdef BUG_KLUDGE
    if(Nhits==0){
      RenderMode=GL_SELECT;
      glRenderMode(RenderMode);
      renderScene();
      RenderMode=GL_RENDER;
      Nhits=glRenderMode(RenderMode);
    }
#endif 
    if(Debug)
      fprintf(stderr,"# pick hits = %d\n", Nhits);
    if(Nhits==0)
      for(unsigned int k=0; k<solar.pick.size(); k++)
        solar.pick[k]=false;
    bool picked=false;
    for(int i=0, index=0; i<Nhits; i++){
      //printf("\n");
      //for(int j=0; j<4; j++)
        //cout<<PickBuffer[j]<<endl;
      nitems=PickBuffer[index++];
      zmin=PickBuffer[index++];
      zmax=PickBuffer[index++];
      if(Debug){
        fprintf(stderr,"Hit # %2d: found %2d items on the name stack\n", i, nitems);
        fprintf(stderr,"\tZmin = 0x%0x, Zmax = 0x%0x\n", zmin, zmax);
      }
      for(int j=0; j<nitems; j++){
        picked=true;
        item=PickBuffer[index++];
        for(unsigned int k=0; k<solar.pick.size(); k++)
          solar.pick[k]=false;
        solar.pick[item-1]=true;
        if(Debug)
          fprintf(stderr,"\t%2d: %6d\n", j, item);
      }
      if(!picked)
        for(unsigned int k=0; k<solar.pick.size(); k++)
          solar.pick[k]=false;
    }
    glutSetWindow(GrWindow);
    glutPostRedisplay();
  }
  else if(button == GLUT_LEFT_BUTTON)
      leftMouseButton = state;
}

// mouseMotion() ///////////////////////////////////////////////////////////////
void mouseMotion(int x, int y) {
  if(leftMouseButton == GLUT_DOWN){
    if(glutGetModifiers()==GLUT_ACTIVE_CTRL){
      cam.setCamR(x,y);
    }
    else{
      cam.setCamAngle(x,y);
      cam.mouseX = x;
      cam.mouseY = y;
    }
    cam.arcBall();
  }
}

// normalKeys() ////////////////////////////////////////////////////////////
void normalKeysDown( unsigned char key, int x, int y ) {
  keyState[key]=true; // add the key to the keyState map if it isn't already, and set the value to true
  if( key == 'q' || key == 'Q' || key == 27 )
    exit(0);
  if(key=='`') camera++; 
  else if(key=='1') camera=0; // arc ball
  for(int i=0; i<solar.solar_sys.size(); i++){
    if(camera == i){
      newPos = Point( solar.solar_sys[i]->position.getX(), solar.solar_sys[i]->position.getY() + 3.5*solar.solar_sys[i]->radius, solar.solar_sys[i]->position.getZ() )/EARTH_RADIUS;
      newLook = Point( solar.solar_sys[i]->position.getX(), solar.solar_sys[i]->position.getY(), solar.solar_sys[i]->position.getZ()+1 )/EARTH_RADIUS;
      cam.smooth(newPos, newLook);
    }
    cam.recomputeOrientation();
    //cout<< cam.camPos.getX()<<" "<<cam.camPos.getY()<<" "<<cam.camPos.getZ()<<endl;
  }
}

void specialKeys(int key, int x, int y){
}

void normalKeysUp(unsigned char key, int x, int y){
  keyState[key]=false; // add the key to the keyState map (is probably already included), and set the value to false
}

void normalKeys(){}

// myTimer() ////////////////////////////////////////////////////////////////////
void myTimer( int value ) {
  normalKeys();
  solar.update();
  cam.update();

  /////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////
  /*ALenum sourceState;
  for(int i=0; i<wavSrcs; i++){
    alGetSourcei( wav.sources[i], AL_SOURCE_STATE, &sourceState );
    if(soundOff) alSourcePause( wav.sources[i] );
  }*/

  glutPostRedisplay();
  glutTimerFunc( 1000.0f / 60.0f, myTimer, 0 );
}

// myMenu() /////////////////////////////////////////////////////////////////////
void myMenu( int value ) {
  switch(value){
    case 0: exit(0); break;
    case 1: soundOff = !soundOff;
      if(soundOff) glutChangeToMenuEntry(1,"Turn ON Sound",1);
      else glutChangeToMenuEntry(1,"Turn OFF Sound",1);
  }
}

// createMenus() ///////////////////////////////////////////////////////////////
void createMenus() {
  glutCreateMenu(myMenu);
  glutAddMenuEntry("Quit",0);
  glutAddMenuEntry("Turn ON Sound",1);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// registerCallbacks() /////////////////////////////////////////////////////////
void registerCallbacks() {
  // keyboard callbacks
  glutSetKeyRepeat(   GLUT_KEY_REPEAT_ON );
  glutKeyboardFunc(   normalKeysDown     );
  glutKeyboardUpFunc( normalKeysUp       );
  glutSpecialFunc(    specialKeys   );

  // mouse callbacks
  glutMouseFunc(      mouseCallback      );
  glutMotionFunc(     mouseMotion        );

  // display callbacks
  glutDisplayFunc(    renderScene        );
  glutReshapeFunc(    resizeWindow       );

  // timer callback
  glutTimerFunc( 1000.0f / 60.0f, myTimer, 0 );
}

// setupShaders() //////////////////////////////////////////////////////////////////////
void setupShaders(char* vertex, char* fragment) {
  shaderProgramHandle = createShaderProgram(vertex, fragment);
  // can change to string and use .c_str()
  //"shaders/customShader.v.glsl"
  //"shaders/customShader.f.glsl"
}

// cleanup() //////////////////////////////////////////////////////////////////////
void cleanup(){
  /////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////
  // wav.cleanupOpenAL();
  solar.cleanup();
}

// main() //////////////////////////////////////////////////////////////////////
int main( int argc, char **argv ) {
  
  glutInit( &argc, argv );
  /////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////
  // alutInit(&argc,argv);
  glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
  glutInitWindowPosition( 50, 50 );
  glutInitWindowSize( windowWidth, windowHeight );
  GrWindow = glutCreateWindow( "Solar System" );
  glSelectBuffer(PICK_BUFFER_SIZE,PickBuffer);

  GLenum glewResult = glewInit();
  if( glewResult != GLEW_OK ) {
      printf( "[ERROR]: Error initalizing GLEW\n");
      return 0;
  }

  GLint maxVertexAttribs = 0;
  glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs );
  
  GLint maxLights = 0;
  glGetIntegerv( GL_MAX_LIGHTS, &maxLights );
  
  GLint maxTextures = 0;
  glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures );

  fprintf( stdout, "[INFO]: /--------------------------------------------------------\\\n");
  fprintf( stdout, "[INFO]: | OpenGL Information                                     |\n");
  fprintf( stdout, "[INFO]: |--------------------------------------------------------|\n");
  fprintf( stdout, "[INFO]: |   OpenGL Version:  %35s |\n", glGetString(GL_VERSION) );
  fprintf( stdout, "[INFO]: |   OpenGL Renderer: %35s |\n", glGetString(GL_RENDERER) );
  fprintf( stdout, "[INFO]: |   OpenGL Vendor:   %35s |\n", glGetString(GL_VENDOR) );
  fprintf( stdout, "[INFO]: |   Shading Version: %35s |\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );
  fprintf( stdout, "[INFO]: >--------------------------------------------------------<\n");
  fprintf( stdout, "[INFO]: |   Max # Vertex Attributes:  %26d |\n", maxVertexAttribs );
  fprintf( stdout, "[INFO]: |   Max # Lights:    %35d |\n", maxLights );
  fprintf( stdout, "[INFO]: |   Max # Textures:  %35d |\n", maxTextures );
  fprintf( stdout, "[INFO]: \\--------------------------------------------------------/\n\n");
  
  if( !glewIsSupported( "GL_VERSION_2_0" ) ) {
      printf( "[ERROR]: System does not support OpenGL 2.0 and GLSL\n" );
      return 0;
  }
  
  printf( "[INFO]: System supports OpenGL2.0 and GLSL!\n\n" );

  initScene();
  printf( "[INFO]: OpenGL Scene set up\n" );
  
  setupShaders("shaders/particleShader.v.glsl","shaders/particleShader.f.glsl"); // need to pass some variables
  printf( "[INFO]: Shader compilation complete.\n" );

  /////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////
  //wav.initializeOpenAL(argc,argv);

  createMenus();

  registerCallbacks();
  
  atexit( cleanup );

  glutMainLoop();
  return(0);
}

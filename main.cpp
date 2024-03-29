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
/////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////
int wavSrcs = 1;
Sound wav;
bool soundOff=true;

/******** SKYBOX ********/
string skyFolder = "skybox1/purplenebula";
string skyExt = ".tga";
string skyFace[6] = {"_ft","_rt","_dn","_up","_bk","_lf"};
CubemapTexture sky;
GLuint sunTexture;

/******** PICKING ********/
#define PICK_TOL 10.
#define PICK_BUFFER_SIZE 256
unsigned int PickBuffer[PICK_BUFFER_SIZE];
int RenderMode;
int Nhits, dx, dy, nitems, zmin, zmax, item, d;
GLint GrWindow;
bool Debug=true;

/******** SHADER ********/
GLuint planetShader = 0;
GLuint planetVisibleShader = 0;
bool visiblePlanets = false;

GLuint particleShader = 0;

/******** TEXTURES ********/
GLuint paone = 0;
// GLuint deathStarTexture = 0;

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

void setPlanetShaders() {
  if(visiblePlanets){
    solar.setShader(planetVisibleShader, 0, EARTH);
    solar.setShader(planetVisibleShader, 0, JUPITER);
    solar.setShader(planetVisibleShader, 0, MARS);
    solar.setShader(planetVisibleShader, 0, MERCURY);
    solar.setShader(planetVisibleShader, 0, SATURN);
    solar.setShader(planetVisibleShader, 0, VENUS);
    solar.setShader(planetVisibleShader, 0, URANUS);
    solar.setShader(planetVisibleShader, 0, NEPTUNE);
  } else {
    solar.setShader(planetShader, 0, EARTH);
    solar.setShader(planetShader, 0, JUPITER);
    solar.setShader(planetShader, 0, MARS);
    solar.setShader(planetShader, 0, MERCURY);
    solar.setShader(planetShader, 0, SATURN);
    solar.setShader(planetShader, 0, VENUS);
    solar.setShader(planetShader, 0, URANUS);
    solar.setShader(planetShader, 0, NEPTUNE);
  }

  visiblePlanets = !visiblePlanets;
}

void drawShip() {
  glPushMatrix();
    Vector normLook;
    normLook = cam.camLook - cam.camPos;
    normLook.normalize();
    normLook *= 0.8;
    Point draw;
    draw = cam.camPos + normLook;
    glTranslatef(draw.getX(), draw.getY(), draw.getZ());

    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, paone);
    //GLUquadricObj *mySphere = gluNewQuadric();
    glScalef(0.01, 0.01, 0.01);
    
    glDisable(GL_LIGHTING);
    //gluSphere(mySphere, 1, 32, 32);
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    float lPosition[4] = { 0, 0, 0, 1 };
    glLightfv( GL_LIGHT1, GL_POSITION,lPosition );
  glPopMatrix();
}

// initScene() /////////////////////////////////////////////////////////////////
void initScene()  {
  glEnable(GL_DEPTH_TEST);

  //******************************************************************
  // this is some code to enable a default light for the scene;
  // feel free to play around with this, but we won't talk about
  // lighting in OpenGL for another couple of weeks yet.
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );

  float lightCol[4] = { 1, 1, 1, 1};
  float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
  float lPosition[4] = { 0, 1.0, 0, 1 };
  glLightfv( GL_LIGHT0, GL_POSITION,lPosition );
  glLightfv( GL_LIGHT0, GL_DIFFUSE,lightCol );
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );

  glEnable(GL_LIGHT1);
  glLightfv( GL_LIGHT1, GL_POSITION,lPosition );
  glLightfv( GL_LIGHT1, GL_DIFFUSE,lightCol );
  glLightfv( GL_LIGHT1, GL_AMBIENT, ambientCol );
  GLfloat dir[4] = {0, 0, -1, 0};
  glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, dir ); 
  glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 2.0 );
  glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 1.0 ); 
  glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);
  // glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 1);

  glDisable(GL_CULL_FACE);
  glEnable(GL_NORMALIZE);
  // glEnable( GL_COLOR_MATERIAL );
  // glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
  //******************************************************************
  glInitNames();
  RenderMode = GL_RENDER;
  
  glShadeModel(GL_SMOOTH);

  // Camera Initialization

  cam.initialize( Point( solar.solar_sys[0]->position.getX(), solar.solar_sys[0]->position.getY() + 3.5*solar.solar_sys[0]->radius, solar.solar_sys[0]->position.getZ() )/EARTH_RADIUS, 
    Point( solar.solar_sys[0]->position.getX(), solar.solar_sys[0]->position.getY(), solar.solar_sys[0]->position.getZ()+1 )/EARTH_RADIUS );
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
  wav.positionSource(wav.sources[0], 0,0,0);
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

  // glPushMatrix();

    //drawGrid();
  wav.positionListener(cam.camPos.getX(), cam.camPos.getY(), cam.camPos.getZ(), cam.camDir.getX(), cam.camDir.getY(), cam.camDir.getZ(), 0,1,0);  

  glPushMatrix();
    glScalef(1500,1500,1500);
    sky.cubeTexture();
  glPopMatrix();
    
  /* // For names and picking -- not working
  for(int i=0; i<solar.solar_sys.size(); i++){  
    glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef( solar.solar_sys[i]->position.getX()/EARTH_RADIUS, solar.solar_sys[i]->position.getY()/EARTH_RADIUS, solar.solar_sys[i]->position.getZ()/EARTH_RADIUS );
      glPushName(i);
        glutWireSphere(solar.solar_sys[i]->radius/EARTH_RADIUS, 32,32);
      glPopName();
    glPopMatrix();
  }
  */
  // Solar System render here
  //Pass the time to the shaders
  glPushMatrix();
    glTranslatef( solar.position.getX(), solar.position.getY(), solar.position.getZ() );
    solar.draw();
  glPopMatrix();

  for(unsigned int i = 0; i < solar.solar_sys.size(); i++) {
    if(solar.solar_sys[i]->type == SUN) {
        Point pos = solar.solar_sys[i]->position;
        float sunPos[4] = {pos.getX(), pos.getY(), pos.getZ(), 1.0};
        glLightfv( GL_LIGHT0, GL_POSITION, sunPos);
        break;
    }
  }
  glDisable( GL_TEXTURE_2D );

  drawShip();

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
      nitems=PickBuffer[index++];
      zmin=PickBuffer[index++];
      zmax=PickBuffer[index++];
      if(Debug && nitems){
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
  else if(key == '2') camera = 1;
  else if(key == '3') camera = 2;
  else if(key == '4') camera = 3;
  else if(key == '5') camera = 4;
  else if(key == '6') camera = 5;
  else if(key == '7') camera = 6;
  else if(key == '8') camera = 7;
  else if(key == '9') camera = 8;
  if(camera == solar.solar_sys.size()) camera = 0;
  
  if (key == '`' || isdigit(key))
    for(unsigned int i=0; i<solar.solar_sys.size(); i++){
      if(camera == i){
        newPos = Point( solar.solar_sys[i]->position.getX(), solar.solar_sys[i]->position.getY() + 3.5*solar.solar_sys[i]->radius, solar.solar_sys[i]->position.getZ() )/EARTH_RADIUS;
        newLook = Point( solar.solar_sys[i]->position.getX(), solar.solar_sys[i]->position.getY(), solar.solar_sys[i]->position.getZ()+1 )/EARTH_RADIUS;
        cam.smooth(newPos, newLook);
      }
    }

  if(key == ' ') {
    setPlanetShaders();
  }

  // Turn on the planet line for the planet you're focused on
  if(key == 'l'){
    solar.solar_sys[camera]->lineOn = !solar.solar_sys[camera]->lineOn;
  }

  //Turn on all planet lines
  if(key == 'n'){
    for(unsigned int i=0; i<solar.solar_sys.size(); i++){
      solar.solar_sys[i]->lineOn = true;
    }
  }

  //Turn off al planet lines
  if(key == 'm'){
    for(unsigned int i=0; i<solar.solar_sys.size(); i++){
      solar.solar_sys[i]->lineOn = false;
    }
  }

  if(key == 'f'){
    solar.solar_sys[camera]->onFire = true;
  }

  if(key == 'd'){
    for(unsigned int i=0; i<solar.solar_sys.size(); i++){
      solar.solar_sys[i]->onFire = true;
    }
  }

  if(key == 'p' || key == 'P') {
    solar.easterEgg();
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
  cam.update(Point(solar.solar_sys[camera]->position.getX(), solar.solar_sys[camera]->position.getY(), solar.solar_sys[camera]->position.getZ())/EARTH_RADIUS);

  /////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////

  glutPostRedisplay();
  glutTimerFunc( 1000.0f / 60.0f, myTimer, 0 );
}

// myMenu() /////////////////////////////////////////////////////////////////////
void myMenu( int value ) {
  switch(value){
    case 0: exit(0); break;
    case 1: soundOff = !soundOff;
      ALenum sourceState[wavSrcs];
      for(int i=0; i<wavSrcs; i++)
        alGetSourcei( wav.sources[i], AL_SOURCE_STATE, &sourceState[i] );
      if(soundOff){
        for(int i=0; i<wavSrcs; i++)
           if(sourceState[i] == AL_PLAYING) alSourceStop( wav.sources[i] );
      }
      else{
        for(int i=0; i<wavSrcs; i++)
          if(sourceState[i] != AL_PLAYING) alSourcePlay( wav.sources[i] );
      }
    break;
  }
}

// createMenus() ///////////////////////////////////////////////////////////////
void createMenus() {
  glutCreateMenu(myMenu);
  glutAddMenuEntry("Quit",0);
  glutAddMenuEntry("Toggle Sound",1);
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

////////////////////////////////////////////////////////////////////////////
//This will set all the values for the current texture. 
//Decided to clamp to edge, even though it shouldn't matter since all textures should be up to the edge anyway.
/////////////////////////////////////////////////////////////////////////////
void registerTexture() {
    glEnable(GL_TEXTURE_2D);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glDisable(GL_TEXTURE_2D);
}

// loadTextures() //////////////////////////////////////////////////////////////////////
// This will load in the textures and give them to the places we want them to be. 
void loadTextures() {
  GLuint texture;
  texture = SOIL_load_OGL_texture("textures/sun.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();
  solar.setTexture(texture, SUN);

  texture = SOIL_load_OGL_texture("textures/sun_specular.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();
  solar.specularTexture(texture, SUN);

  texture = SOIL_load_OGL_texture("textures/earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();
  solar.setTexture(texture, EARTH);

  texture = SOIL_load_OGL_texture("textures/jupiter.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();
  solar.setTexture(texture, JUPITER);

  texture = SOIL_load_OGL_texture("textures/mars.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();
  solar.setTexture(texture, MARS);

  texture = SOIL_load_OGL_texture("textures/mercury.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();
  solar.setTexture(texture, MERCURY);

  texture = SOIL_load_OGL_texture("textures/saturn.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();
  solar.setTexture(texture, SATURN);

  texture = SOIL_load_OGL_texture("textures/venus.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();
  solar.setTexture(texture, VENUS);

  texture = SOIL_load_OGL_texture("textures/uranus.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();
  solar.setTexture(texture, URANUS);

  texture = SOIL_load_OGL_texture("textures/neptune.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();
  solar.setTexture(texture, NEPTUNE);

  paone = SOIL_load_OGL_texture("textures/paone.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, paone);
  registerTexture();
  solar.setEggTexture(paone);

  /*deathStarTexture = SOIL_load_OGL_texture("textures/deathStar.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
  glBindTexture(GL_TEXTURE_2D, texture);
  registerTexture();*/
}

// setupShaders() //////////////////////////////////////////////////////////////////////
// sets up the shaders and then attaches them to the correct planet. 
void setupShaders() {
  GLuint sunShader = createShaderProgram("shaders/sunShader.v.glsl","shaders/sunShader.f.glsl");
  GLuint uniformTimeLoc = glGetUniformLocation(sunShader, "time");
  solar.setShader(sunShader, uniformTimeLoc, SUN);

  planetShader = createShaderProgram("shaders/planet.v.glsl", "shaders/planet.f.glsl");
  planetVisibleShader =  createShaderProgram("shaders/planetVisible.v.glsl", "shaders/planetVisible.f.glsl");
  setPlanetShaders();

  particleShader = createShaderProgram("shaders/particle.v.glsl","shaders/particle.f.glsl");
  solar.setParticleShader(particleShader);
}

// cleanup() //////////////////////////////////////////////////////////////////////
void cleanup(){
  /////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////
  wav.cleanupOpenAL();
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
  wav.initializeOpenAL(argc,argv);

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
  
  setupShaders(); // need to pass some variables
  printf( "[INFO]: Shader compilation complete.\n\n" );

  loadTextures();
  printf( "[INFO]: Texture creation complete.\n" );

  /////////////////////////////////// UNCOMMENT FOR SOUND ///////////////////////////////////

  createMenus();

  registerCallbacks();
  
  atexit( cleanup );

  glutMainLoop();
  return(0);
}

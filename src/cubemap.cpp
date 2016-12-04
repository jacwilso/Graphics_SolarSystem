#include "cubemap.h"

using namespace std;

bool CubemapTexture::RegisterOpenGLTexture(GLuint &textureHandle){
    // Set Min & Mag filters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Set how to wrap S & T
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    return true;
}

bool CubemapTexture::Load(string filename[6]){
  for(int i=0; i<6; i++){
    textureHandle[i] = SOIL_load_OGL_texture(filename[i].c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS|SOIL_FLAG_INVERT_Y|SOIL_FLAG_COMPRESS_TO_DXT);

      RegisterOpenGLTexture(textureHandle[i]);
  }
  return true;
}

void CubemapTexture::polygon(int a, int b, int c, int d){
  glBegin(GL_POLYGON);
    glTexCoord2fv(texture[0]);
    glVertex3fv(vertices[a]);
    glTexCoord2fv(texture[1]);
    glVertex3fv(vertices[b]);
    glTexCoord2fv(texture[2]);
    glVertex3fv(vertices[c]);
    glTexCoord2fv(texture[3]);
    glVertex3fv(vertices[d]);
  glEnd();
}

void CubemapTexture::cubeTexture(){
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  
  // FRONT
  glBindTexture(GL_TEXTURE_2D, textureHandle[0]);
  glNormal3fv(normals[0]);
  polygon(3,2,1,0);
  // RIGHT
  glBindTexture(GL_TEXTURE_2D, textureHandle[1]);
  glNormal3fv(normals[1]);
  polygon(7,6,2,3);
  // DOWN
  glBindTexture(GL_TEXTURE_2D, textureHandle[2]);
  glNormal3fv(normals[2]);
  polygon(4,7,3,0);
  // UP
  glBindTexture(GL_TEXTURE_2D, textureHandle[3]);
  glNormal3fv(normals[3]);
  polygon(6,5,1,2);
  // BACK
  glBindTexture(GL_TEXTURE_2D, textureHandle[4]);
  glNormal3fv(normals[4]);
  polygon(4,5,6,7);
  // LEFT
  glBindTexture(GL_TEXTURE_2D, textureHandle[5]);
  glNormal3fv(normals[5]);
  polygon(0,1,5,4);
  
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
}

void CubemapTexture::cube(){
  glLightf( GL_LIGHT0, GL_DIFFUSE, 0.0 );
  // FRONT
  glNormal3fv(normals[0]);
  polygon(3,2,1,0);
  // RIGHT
  glNormal3fv(normals[1]);
  polygon(7,6,2,3);
  // DOWN
  glNormal3fv(normals[2]);
  polygon(4,7,3,0);
  // UP
  glNormal3fv(normals[3]);
  polygon(6,5,1,2);
  // BACK
  glNormal3fv(normals[4]);
  polygon(4,5,6,7);
  // LEFT
  glNormal3fv(normals[5]);
  polygon(0,1,5,4);
}

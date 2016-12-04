#ifndef CUBEMAP_H
#define CUBEMAP_H

// Include our OpenGL & GLUT libraries
// GLUT automatically includes glu.h and gl.h
#ifdef __APPLE__
	#include <GLUT/glut.h>	// GLUT, GLU, GL Libraries for Mac
#else
	#include <GL/glut.h>	// GLUT, GLU, GL Libraries for Windows or *nix
#endif

#include <SOIL/SOIL.h>  	// Simple OpenGL Image Library

#include <string>

using namespace std;

const static GLfloat vertices[][3] = {{-1,-1,1},{-1,1,1},{1,1,1},{1,-1,1},{-1,-1,-1},{-1,1,-1},{1,1,-1},{1,-1,-1}};
const static GLfloat texture[][2] = {{0,0},{0,1},{1,1},{1,0}};
const static GLfloat normals[][3] = {{0,0,-1},{0,1,0},{-1,0,0},{1,0,0},{0,0,1},{0,-1,0}};

class CubemapTexture{
  private:
    bool RegisterOpenGLTexture(GLuint &textureHandle);
    void polygon(int a, int b, int c, int d);
    GLuint textureHandle[6];
  public:
    bool Load(string filename[6]);
    void cubeTexture();
    void cube();
};

#endif

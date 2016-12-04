#ifndef __HELPER__H__
#define __HELPER__H__

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include <vector>
#include <string>

using namespace std;

float max(float a, float b);
float min(float a, float b);
float getRand();
float getRand(float min, float max);
void drawGrid();
bool registerOpenGLTexture(unsigned char *imageData, int texWidth, int texHeight, GLuint &texHandle);

#endif

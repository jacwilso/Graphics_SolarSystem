#include "helper.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

//
//  float max()
//
//  Returns the maximum of two numbers.
//
float max(float a, float b)
{
    return (a > b ? a : b);
}

//
//  float min()
//
//  Returns the minimum of two numbers.
//
float min(float a, float b)
{
    return (a < b ? a : b);
}

//
//  float getRand()
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
float getRand()
{
    return rand() / (float)RAND_MAX;
}

//
//  float getRand()
//
//  Simple helper function to return a random number between the requested ranges.
//
float getRand(float min, float max)
{
    return (getRand() * (max - min)) + min;
}


//
//  void drawGrid();
//
//  Draws a grid at the origin, ranging from -10 to 10 in the X and Z axes.
//  This size can be controlled with the appropriate calls to glScale() beforehand.
//
void drawGrid()
{
    //turn off lighting for the grid so that it is just solid white lines
    glDisable(GL_LIGHTING);

    //draw a simple grid under the teapot
    glColor3f(.6,.6,.6);
    for(int dir = 0; dir < 2; dir++)
    {
        for(int i = -100; i <101 ; i++)
        {
            glBegin(GL_LINE_STRIP);
            for(int j = -100; j < 101; j++)
                glVertex3f(dir<1?i:j, 0, dir<1?j:i);
            glEnd();
        }
    }

    //and then draw the teapot itself!
    glEnable(GL_LIGHTING);
}

//
//  bool registerOpenGLTexture(unsigned char *imageData, int texWidth, int texHeight, GLuint &texHandle)
//
//  Attempts to register an image buffer with OpenGL. Sets the texture handle
//      appropriately upon success, and uses a number of default texturing
//      values. This function only provides the basics: just sets up this image
//      as an unrepeated 2D texture.
//
bool registerOpenGLTexture(unsigned char *imageData, int texWidth, int texHeight, GLuint &texHandle)
{
    //first off, get a real texture handle.
    glGenTextures(1, &texHandle);

    //make sure that future texture functions affect this handle
    glBindTexture(GL_TEXTURE_2D, texHandle);

    //set this texture's color to be multiplied by the surface colors -- 
    //  GL_MODULATE allows us to take advantage of OpenGL lighting
    //  GL_REPLACE uses only the color from the texture.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //set the minification ang magnification functions to be linear; not perfect
    //  but much better than nearest-texel (GL_NEAREST).
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Set the texture to repeat in S and T -- though it doesn't matter here
    //  because our texture coordinates are always in [0,0] to [1,1].
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //actually transfer the texture to the GPU!
    glTexImage2D(GL_TEXTURE_2D,                 //still working with 2D textures, obv.
                    0,                          //not using mipmapping, so this is the highest level.
                    GL_RGB,                     //we're going to provide OpenGL with R, G, and B components...
                    texWidth,                   //...of this width...
                    texHeight,                  //...and this height.
                    0,                          //give it a border of 0 pixels (none)
                    GL_RGB,                     //and store it, internally, as RGB (OpenGL will convert to floats between 0.0 and 1.0f)
                    GL_UNSIGNED_BYTE,           //this is the format our data is in, and finally,
                    imageData);                 //there's the data!

    //whoops! i guess this function can't fail. in an ideal world, there would
    //be checks with glGetError() that we could use to determine if this failed.
    return true;
}

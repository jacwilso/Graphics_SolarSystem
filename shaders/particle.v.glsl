
/*
 *   Phong Shading vertex shader to simulate a single point light with texturing
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

varying vec3 normalVec;
varying vec3 lightVec;
varying vec3 halfwayVec;
varying vec4 theColor;
varying float attenuation;
uniform float age;
uniform float lifespan;

void main() {

    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/

    // Vary the size of the particle based on its age
    // >> older = smaller
    gl_Vertex += (age/lifespan);

    // first things, first: set gl_Position equal to the vertex in clip space
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    theColor = gl_Color;
}

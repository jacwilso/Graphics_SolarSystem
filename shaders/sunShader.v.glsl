/*
 *   Vertex Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

varying vec4 theColor;
uniform float time;

void main() {
    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/
    
    //modify our vertex in object space
    // gl_Vertex = gl_Vertex + 1.2 * (sin(time) + 1) / 2 + 0.2;
    gl_Vertex = gl_Vertex + 0.2 * (sin(time) + 1) / 2 + 0.8;
    
    //Perform the Vertex Transformation from Object Space to Clip Space
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    //set our varying variable equal to the Vertex
    theColor =  vec4(1,0.776,0.05,1);
}

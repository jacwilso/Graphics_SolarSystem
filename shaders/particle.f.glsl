/*
 *   Fragment shader that implements Phong Shading with texturing.
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

uniform sampler2D tex;

void main(void) 
{

    gl_FragColor = vec4( 1,1,1,1 );
    gl_FragColor = theColor;
}

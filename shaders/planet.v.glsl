/*
 *   Phong Shading vertex shader to simulate a single point light with texturing
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

varying vec3 normalVec;
varying vec3 lightVec[2];
varying vec3 halfwayVec[2];
varying float attenuation[2];

uniform bool spotLight;

void main() {

    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/

    

    // first things, first: set gl_Position equal to the vertex in clip space
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    /*****************************************/
    /********* Texture Calculations  *********/
    /*****************************************/

    // pass the texture coordinate through to the fragment processor
    gl_TexCoord[0] = gl_MultiTexCoord0;

    /*****************************************/
    /*********  Vector Calculations  *********/
    /*****************************************/

    // compute our normal vector in eye space
    normalVec = normalize( gl_NormalMatrix * gl_Normal );

    // compute the light vector in eye space
    lightVec[0] = gl_LightSource[0].position.xyz - (gl_ModelViewMatrix * gl_Vertex).xyz;
    lightVec[1] = gl_LightSource[1].position.xyz - (gl_ModelViewMatrix * gl_Vertex).xyz;

    // compute the distance from the point to the light and compute attenuation
    float dist = length( lightVec[0] );
    attenuation[0] = 1.0 / (gl_LightSource[0].constantAttenuation + gl_LightSource[1].linearAttenuation * dist + gl_LightSource[1].quadraticAttenuation * dist * dist);

    dist = length( lightVec[1] );
    attenuation[1] = 1.0 / (gl_LightSource[1].constantAttenuation + gl_LightSource[1].linearAttenuation * dist + gl_LightSource[1].quadraticAttenuation * dist * dist);

    // now normalize our vector after we've used the magnitude for the distance
    lightVec[0] = normalize( lightVec[0] );
    lightVec[1] = normalize( lightVec[1] );

    // compute the camera vector in eye space
    vec3 cameraVec = normalize( vec3( gl_ModelViewMatrixInverse * vec4( 0.0, 0.0, 0.0, 1.0 ) ) - gl_Vertex.xyz );

    // compute the halfway vector between the light and camera vectors, used for specular highlighting
    halfwayVec[0] = normalize( lightVec[0] + cameraVec );
    halfwayVec[1] = normalize( lightVec[1] + cameraVec );
}

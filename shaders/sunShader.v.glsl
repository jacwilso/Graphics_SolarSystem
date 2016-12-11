/*
 *   Phong Shading vertex shader to simulate a single point light with texturing
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

varying vec2 vertex_uv;
varying vec3 normalVec;
varying vec3 lightVec;
varying vec3 halfwayVec;
varying float attenuation;

uniform sampler2D tex;
uniform sampler2D displace_map;

uniform float time;

void main() {

    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/
    vec4 displace = texture2D(displace_map,vec2(vertex_uv.s + time, vertex_uv.t + time));
    gl_Vertex = gl_Vertex - (sin(time) + 1) / 50;
    //gl_Vertex = gl_Normal.rgb;

    // vec4 dispMap = texture2D( displacement, gl_TexCoord[1].st );
    // gl_Vertex = gl_Vertex + normalVec * dispMap.rgb * 100;

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
    lightVec = gl_LightSource[0].position.xyz - (gl_ModelViewMatrix * gl_Vertex).xyz;

    // compute the distance from the point to the light and compute attenuation
    float dist = length( lightVec );
    attenuation = 1.0 / (gl_LightSource[0].constantAttenuation + gl_LightSource[0].linearAttenuation * dist + gl_LightSource[0].quadraticAttenuation * dist * dist);

    // now normalize our vector after we've used the magnitude for the distance
    lightVec = normalize( lightVec );

    // compute the camera vector in eye space
    vec3 cameraVec = normalize( vec3( gl_ModelViewMatrixInverse * vec4( 0.0, 0.0, 0.0, 1.0 ) ) - gl_Vertex.xyz );

    // compute the halfway vector between the light and camera vectors, used for specular highlighting
    halfwayVec = normalize( lightVec + cameraVec );
}

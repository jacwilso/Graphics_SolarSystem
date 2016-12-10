/*
 *   Fragment Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

varying vec4 theColor;

void main() {

    /*****************************************/
    /******* Final Color Calculations ********/
    /*****************************************/
    
    //set the fragment color!
    // gl_FragColor = vec4(0.97,0.8,0.05,.95);

    //use our varying variable
    gl_FragColor = theColor;
}

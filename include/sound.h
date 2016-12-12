#ifndef SOUND_H
#define SOUND_H

#ifdef __APPLE__			// if compiling on Mac OS
    #include <AL/alut.h>  // OpenAL Headers
    #include <AL/al.h>
    #include <AL/alc.h>
#else                       // else compiling on Linux OS
    #include <AL/alut.h>    // OpenAL Headers
    #include <AL/al.h>
    #include <AL/alc.h>
#endif

#include <stdio.h>

class Sound{
private:
    #define NUM_BUFFERS 2 // number of buffers
    #define NUM_SOURCES 2 // number of sources

    // lab04
    ALCdevice *device; 
    ALCcontext *context;
    ALuint buffers[ NUM_BUFFERS ];
    
    void PrintOpenALInfo(); // print the information
public:

    ALuint sources[ NUM_SOURCES ]; // wavs in source array
    
    void initializeOpenAL( int argc, char *argv[] ); // initializer of the sound -- called in main
    void cleanupOpenAL(); // clean the sound up
    
    void positionListener(float posX,float posY,float posZ,float dirX,float dirY,float dirZ,float upX=0,float upY=1,float upZ=0); // position the listener with proper variables
    void positionSource( ALuint src, float posX, float posY, float posZ ); // postion the source with proper variables
};

#endif

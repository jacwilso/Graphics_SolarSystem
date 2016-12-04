#ifndef SOUND_H
#define SOUND_H

#ifdef __APPLE__			// if compiling on Mac OS
    #include <ALUT/alut.h>  // OpenAL Headers
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else                       // else compiling on Linux OS
    #include <AL/alut.h>    // OpenAL Headers
    #include <AL/al.h>
    #include <AL/alc.h>
#endif

#include <stdio.h>
#include <string>
using namespace std;

#include "Point.h"

class Sound{
  private:
    int wavSrcs;

    ALCdevice *device; 
    ALCcontext *context;
    ALuint *buffers;
    
    void PrintOpenALInfo();
  public:
    ALuint *sources;
    
    Sound( int wavSrcs );
    void initializeOpenAL( int srcs, string sourceFile[], Point position[] );
    //void initializeOpenAL( int argc, char *argv[] ); 
    void cleanupOpenAL();
    
    void positionListener( Point position, Point direction, Point upVec);
    void positionSource( ALuint src, Point position ); 
};

#endif

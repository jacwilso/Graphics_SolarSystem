#include "sound.h"

// intializing the sound -- copied from lab04
void Sound::initializeOpenAL( int argc, char *argv[] ) {
    ALsizei size, freq;
    ALenum 	format;
    ALvoid 	*data;
    ALboolean loop;
    
    alutInit(&argc,argv);

    device=alcOpenDevice(NULL);
    context=alcCreateContext(device,NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(NUM_BUFFERS,buffers);
    alGenSources(NUM_SOURCES,sources);

// WAV #1 -- NEED TO CHANGE ON PROGRAM BASIS
    #ifdef __APPLE__
      alutLoadWAVFile((ALbyte*)"rocket.wav",&format,&data,&size,&freq);
    #else
      alutLoadWAVFile((ALbyte*)"rocket.wav",&format,&data,&size,&freq,&loop);
    #endif
      alBufferData(buffers[0],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);
    alSourcei(sources[0],AL_BUFFER,buffers[0]);
    alSourcei(sources[0],AL_LOOPING,AL_TRUE);

    PrintOpenALInfo();					// print our OpenAL versioning information
}

// print the openAl information -- copied from lab04
void Sound::PrintOpenALInfo() {
    fprintf( stdout, "[INFO]: /--------------------------------------------------------\\\n");
    fprintf( stdout, "[INFO]: | OpenAL Information                                     |\n");
    fprintf( stdout, "[INFO]: |--------------------------------------------------------|\n");
    fprintf( stdout, "[INFO]: |   OpenAL Version:  %35s |\n", alGetString(AL_VERSION) );
    fprintf( stdout, "[INFO]: |   OpenAL Renderer: %35s |\n", alGetString(AL_RENDERER) );
    fprintf( stdout, "[INFO]: |   OpenAL Vendor:   %35s |\n", alGetString(AL_VENDOR) );
    fprintf( stdout, "[INFO]: \\--------------------------------------------------------/\n\n");
}

// clean up the openAl things
void Sound::cleanupOpenAL() {
    alutExit();
    alDeleteBuffers(NUM_BUFFERS,buffers);
    alDeleteSources(NUM_SOURCES,sources);
}

// position listener -- copied from lab04
void Sound::positionListener(float posX,float posY,float posZ,float dirX,float dirY,float dirZ,float upX,float upY,float upZ){
    float listenerPosition[3]={posX,posY,posZ};
    float listenerOrientation[6]={dirX,dirY,dirZ,upX,upY,upZ};
    alListenerfv(AL_POSITION,listenerPosition);
    alListenerfv(AL_ORIENTATION,listenerOrientation);
}
 // position the source -- copied from lab04
void Sound::positionSource(ALuint src,float posX,float posY,float posZ){
    float srcPosition[3]={posX,posY,posZ};
    alSourcefv(src,AL_POSITION,srcPosition);
}

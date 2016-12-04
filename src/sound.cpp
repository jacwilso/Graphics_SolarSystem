#include "sound.h"
using namespace std;

Sound::Sound( int wavSrcs ){
  this->wavSrcs = wavSrcs;
  #define NUM_BUFFERS wavSrcs
  #define NUM_SOURCES wavSrcs
  buffers = new ALuint[wavSrcs];
  sources = new ALuint[wavSrcs];
}

void Sound::initializeOpenAL( int srcs, string sourceFile[], Point position[] ) {
    ALsizei size, freq;
    ALenum 	format;
    ALvoid 	*data;
    ALboolean loop;
    
    //alutInit(&argc,argv);

    device=alcOpenDevice( NULL );
    context=alcCreateContext( device, NULL );
    alcMakeContextCurrent( context );
    alGenBuffers( NUM_BUFFERS, buffers );
    alGenSources( NUM_SOURCES, sources );
    
    for(int i=0; i<srcs; i++){
      #ifdef __APPLE__
        alutLoadWAVFile( (ALbyte*)sourceFile[i].c_str(), &format, &data, &size, &freq );
      #else
        alutLoadWAVFile( (ALbyte*)sourceFile[i].c_str(), &format, &data, &size, &freq, &loop );
      #endif
      alBufferData( buffers[i], format, data, size, freq );
      alutUnloadWAV( format, data, size, freq );
      alSourcei( sources[i], AL_BUFFER, buffers[i] );
      alSourcei( sources[i], AL_LOOPING, AL_TRUE );
      positionSource( sources[i], position[i] );
    }

    PrintOpenALInfo();
}

void Sound::PrintOpenALInfo() {
    fprintf( stdout, "[INFO]: /--------------------------------------------------------\\\n");
    fprintf( stdout, "[INFO]: | OpenAL Information                                     |\n");
    fprintf( stdout, "[INFO]: |--------------------------------------------------------|\n");
    fprintf( stdout, "[INFO]: |   OpenAL Version:  %35s |\n", alGetString(AL_VERSION) );
    fprintf( stdout, "[INFO]: |   OpenAL Renderer: %35s |\n", alGetString(AL_RENDERER) );
    fprintf( stdout, "[INFO]: |   OpenAL Vendor:   %35s |\n", alGetString(AL_VENDOR) );
    fprintf( stdout, "[INFO]: \\--------------------------------------------------------/\n\n");
}

void Sound::cleanupOpenAL() {
  alutExit();
  alDeleteBuffers(wavSrcs,buffers);
  alDeleteSources(wavSrcs,sources);
  delete buffers;
  delete sources;
}

void Sound::positionListener( Point position, Point direction, Point upVec){
    float listenerPosition[3] = { position.getX(), position.getY(), position.getZ() };
    float listenerOrientation[6]={ direction.getX(), direction.getY(), direction.getZ(), upVec.getX(), upVec.getY(), upVec.getZ() };
    alListenerfv( AL_POSITION, listenerPosition );
    alListenerfv( AL_ORIENTATION, listenerOrientation );
}
void Sound::positionSource( ALuint src, Point position ){
    float srcPosition[3] = { position.getX(), position.getY(), position.getZ() };
    alSourcefv( src, AL_POSITION, srcPosition );
}

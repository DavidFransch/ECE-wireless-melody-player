#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <stdint.h>
#include <unistd.h> // for usleep()

#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

typedef struct
{
    uint32_t total_count;
    uint32_t up_count;

    uint32_t counter;
    uint32_t prev_freq;
    uint32_t freq;
    uint8_t volume;
} paTestData;

//volatile int freq = 0;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/

static PaStream *stream;
static paTestData data;


static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData)
{
    paTestData *data = (paTestData*)userData;
    uint8_t *out = (uint8_t*)outputBuffer;
    unsigned long i;
    uint32_t freq = data->freq;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    for( i=0; i<framesPerBuffer; i++ )
    {
        if(data->up_count > 0 && data->total_count == data->up_count) {
            *out++ = 0x00;
            continue;
        }
        data->total_count++;

        if(freq != data->prev_freq) {
            data->counter = 0;
        }

        if(freq) {
            int overflow_max = SAMPLE_RATE / freq;
            uint32_t data_cnt = data->counter % overflow_max;
            if(data_cnt > overflow_max/2)
                *out++ = data->volume;
            else {
                *out++ = 0x00;
            }
            data->counter++;
        }
        else {
            data->counter = 0;
            *out++ = 0;
        }
        data->prev_freq = freq;
    }

    return paContinue;
}




void buzzer_set_freq(int frequency)
{
    data.up_count = 0; // do not stop!
    data.freq = frequency;
}

void buzzer_set_volume(double volume){
    int out = 255*(volume/10);
    printf("the volume is: %d",out);
    data.volume = out;

}

void buzzer_beep(int frequency, int msecs)
{
    data.total_count = 0;
    data.up_count = SAMPLE_RATE * msecs / 1000;
    data.freq = frequency;
}



int buzzer_start(void)
{
    PaStreamParameters outputParameters;

    PaError err;
    //int i;

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    outputParameters.channelCount = 1;       /* stereo output */
    outputParameters.sampleFormat = paUInt8; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        patestCallback,
        &data );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;

}

int buzzer_stop() 
{
    PaError err = 0;
    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();

    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}
void msleep(int d){
    usleep(d*1000);
}
/*
int melody(tuple array[]){
    buzzer_start();
    for(int i=0; array.length; i++){
        buzzer_set_freq(array[i][0]);  // first element in the tuple array is frequency
        msleep(array[i][1]);  // second element in the tuple array is duration
    }
    buzzer_stop();
    return 0;
}
*/
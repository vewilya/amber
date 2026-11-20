#include <stdio.h>

// component.h
#ifndef COMPONENT_H
#define COMPONENT_H

typedef struct {
    double sampleRate;
    int numChannels;
    float gain;

} AudioComponent;

void audio_component_init(AudioComponent *c, double sampleRate, int numChannels);

void audio_component_set_gain(AudioComponent *c, const float gain_value);

void audio_component_process(AudioComponent *c, const float *inputBuffer,

                             float *outputBuffer, unsigned long framesPerBuffer);

#endif

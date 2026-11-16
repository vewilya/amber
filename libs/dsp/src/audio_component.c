#include <dsp/audio_component.h>

void audio_component_init(AudioComponent *c, double sampleRate,
                          int numChannels) {
  c->sampleRate = sampleRate;
  c->numChannels = numChannels;
  c->gain = 1.0f;
}

void audio_component_set_gain(AudioComponent *c, const float gain_value) {
  c->gain = gain_value;
  printf("gain set to %f\n", c->gain);
}

void audio_component_process(AudioComponent *c, const float *inputBuffer,
                             float *outputBuffer,
                             unsigned long framesPerBuffer) {

  unsigned long n = c->numChannels * framesPerBuffer;
  for (unsigned long i = 0; i < n; i++) {
    outputBuffer[i] = inputBuffer[i] * c->gain; // passthrough and gain change
  }
}

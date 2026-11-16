#ifndef AMBER_H
#define AMBER_H

#include <dsp/plugin.h>

Plugin *amber_create(double sample_rate, int num_channels, float drive);

#endif // AMBER_H

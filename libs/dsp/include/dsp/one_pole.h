#include <stdio.h>
#include <dsp/error.h>
#include <dsp/plugin.h>

#ifndef ONEPOLE_H
#define ONEPOLE_H

Plugin *one_pole_create(double sample_rate, int num_channels, float alpha);

#endif

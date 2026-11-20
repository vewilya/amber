#include <dsp/one_pole.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ONE_POLE_MAX_CHANNELS 2

typedef struct {
    Plugin base;
    double sample_rate;
    int num_channels;
    float alpha;
    float prev_sample[ONE_POLE_MAX_CHANNELS];
} OnePole;

static void one_pole_process(Plugin *self, float *buffer, size_t num_frames) {
    OnePole *op = (OnePole *)self;

    const float a = op->alpha;
    const float b = 1.0f - a;
    int num_channels = op->num_channels;

    for (unsigned long f = 0; f < num_frames; f++) {
        for (int c = 0; c < num_channels; c++) {
            size_t i = f * num_channels + c;
            float out = b * buffer[i] + a * op->prev_sample[c];
            buffer[i] = out;
            op->prev_sample[c] = out;
        }
    }
}

static void one_pole_destroy(Plugin *self) { free(self); }

static Err_t one_pole_set_parameter(Plugin *self, const char *name, float value) {

    OnePole *op = (OnePole *)self;
    if (strcmp(name, "alpha") != 0)
        return PARAM_ERR;
    if (value < 0.0f || value > 1.0f)
        return VALUE_ERR;

    op->alpha = value;

    return NO_ERR;
}

Plugin *one_pole_create(double sample_rate, int num_channels, float alpha) {
    if (num_channels <= 0 || num_channels > ONE_POLE_MAX_CHANNELS)
        return NULL;

    OnePole *op = calloc(1, sizeof(*op));
    if (!op)
        return NULL;

    op->base = (Plugin){
        .name = "One Pole",
        .process = one_pole_process,
        .set_parameter = one_pole_set_parameter,
        .destroy = one_pole_destroy,
    };

    op->sample_rate = sample_rate;
    op->num_channels = num_channels;
    op->alpha = alpha;

    return &op->base;
}

#include <dsp/amber.h>
#include <stdlib.h>
#include <string.h>
#include <torchline/torchline.h>


#define AMBER_MAX_CHANNELS 2

typedef struct {
    Plugin base;
    double sample_rate;
    int num_channels;
    const TlNetwork *net;
    float drive;
} Amber;

void  amber_process(Plugin *self, float *buffer, size_t num_frames) {
    Amber *amber = (Amber *)self;

    const int num_channels = amber->num_channels;

    for (size_t f = 0; f < num_frames; f++) {
        float *frame = &buffer[f * num_channels];
        float in[AMBER_MAX_CHANNELS];

        for (int c = 0; c < num_channels; c++) in[c] = frame[c] * amber->drive;
        tl_forward(amber->net, in, frame);
    }
}

Err_t amber_set_parameter(Plugin *self, const char *name, float value) {
    Amber *amber = (Amber *)self;
    if (strcmp(name, "drive") != 0) return PARAM_ERR;
    if (value < 0.0f || value > 3.0f) return VALUE_ERR;

    amber->drive = value;
    return NO_ERR;
}

void  amber_destroy(Plugin *self) {
    free(self);
}

Plugin *amber_create(double sample_rate, int num_channels, float drive) {
    if (num_channels <= 0 || num_channels > AMBER_MAX_CHANNELS) return NULL;

    Amber *amber =  calloc(1, sizeof(*amber));
    if (!amber) return NULL;

    amber->base = (Plugin){
        .name = "Amber",
        .process       = amber_process,
        .set_parameter = amber_set_parameter,
        .destroy       = amber_destroy,
    };

    amber->sample_rate = sample_rate;
    amber->num_channels = num_channels;
    amber->drive = drive;
    amber->net = tl_load_network();
    if (tl_input_size(amber->net) != num_channels ||
        tl_output_size(amber->net) != num_channels) {
        free(amber);
        return NULL;
    }

    return &amber->base;
}

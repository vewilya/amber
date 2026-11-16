#include <dsp/error.h>

#ifndef TYPES_H
#define TYPES_H

typedef struct Plugin_t Plugin;

struct Plugin_t {
    const char *name;
    void  (*process)(Plugin *self, float *buffer, size_t num_frames);
    Err_t (*set_parameter)(Plugin *self, const char *name, float value);
    void  (*destroy)(Plugin *self);
};


#endif

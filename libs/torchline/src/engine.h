#ifndef ENGINE_H
#define ENGINE_H

#include <torchline/torchline.h>

typedef enum {
    ACTIVATION_NONE = 0,
    ACTIVATION_RELU,
    ACTIVATION_SIGMOID,
    ACTIVATION_TANH,
} Activation;

typedef struct {
    int num_inputs;
    int num_outputs;
    const float *weights;
    const float *biases;
    Activation activation;
} Layer;

struct TlNetwork {
    int num_layers;
    const Layer *layers;
};

#endif /* ENGINE_H */

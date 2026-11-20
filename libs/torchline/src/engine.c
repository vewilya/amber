#include "engine.h"
#include "model_weights.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const TlNetwork *tl_load_network() {
    static const TlNetwork network = {LAYER_COUNT, layers};
    return &network;
}

int tl_input_size(const TlNetwork *net) { return net->layers[0].num_inputs; }
int tl_output_size(const TlNetwork *net) { return net->layers[net->num_layers - 1].num_outputs; }

static float activate(Activation function, float x) {
    switch (function) {
    case ACTIVATION_NONE:
        return x;
    case ACTIVATION_RELU:
        return x > 0 ? x : 0;
    case ACTIVATION_SIGMOID:
        return 1.0f / (1.0f + expf(-x));
    case ACTIVATION_TANH:
        return tanhf(x);
    default:
        return x;
    }
}

static void layer_forward(const Layer *layer, const float *inputs, float *outputs) {
    int num_nodes = layer->num_outputs;
    Activation func = layer->activation;
    int num_inputs = layer->num_inputs;

    for (int i = 0; i < num_nodes; i++) {
        float sum = 0.0f;

        for (int j = 0; j < num_inputs; j++) {
            sum += inputs[j] * layer->weights[i * num_inputs + j];
        }

        sum += layer->biases[i];
        outputs[i] = activate(func, sum);
    }
}

void tl_forward(const TlNetwork *net, const float *inputs, float *outputs) {
    float scrap_buffers[2][MAX_LAYER_SIZE];

    const float *current_inputs = inputs;

    for (int i = 0; i < net->num_layers; i++) {
        bool last_layer = (i == net->num_layers - 1);
        float *current_outputs = last_layer ? outputs : scrap_buffers[i % 2];
        layer_forward(&net->layers[i], current_inputs, current_outputs);

        current_inputs = current_outputs;
    }
}

#ifdef RUN_MAIN
int main(void) {
    printf("Hello, Net!\n");
    const TlNetwork *net = tl_load_network();

    float inputs[MODEL_INPUT_SIZE] = {0.5f, 0.3f};
    float outputs[MAX_LAYER_SIZE];
    tl_forward(net, inputs, outputs);

    for (int i = 0; i < MODEL_OUTPUT_SIZE; i++)
        printf("output[%d] = %f\n", i, outputs[i]);

    return 0;
}
#endif

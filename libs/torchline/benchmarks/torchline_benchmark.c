#include <stdio.h>

#include <bench/bench.h>
#include <torchline/torchline.h>

enum { MAX_FEATURES = 64 };

typedef struct {
    const TlNetwork *net;
    float inputs[MAX_FEATURES];
    float outputs[MAX_FEATURES];
} ForwardPass;

static void forward(void *context) {
    ForwardPass *pass = context;
    tl_forward(pass->net, pass->inputs, pass->outputs);
}

int main(void) {
    ForwardPass pass = {.net = tl_load_network()};
    if (tl_input_size(pass.net) > MAX_FEATURES || tl_output_size(pass.net) > MAX_FEATURES) {
        fprintf(stderr, "Network is wider than MAX_FEATURES (%d)\n", MAX_FEATURES);
        return 1;
    }
    for (int i = 0; i < tl_input_size(pass.net); ++i) {
        pass.inputs[i] = 0.25f;
    }

    // One forward pass of the compiled-in network, i.e. the cost per sample frame.
    bench_print_header();
    bench_run("tl_forward", forward, &pass, 1.0);
    return 0;
}

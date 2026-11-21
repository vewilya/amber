#include <bench/bench.h>
#include <gratlin/gratlin.h>

// One forward pass of the compiled-in network, i.e. the cost per sample frame.
// See libs/torchline/benchmarks/torchline_benchmark.c for a complete example.
//
// static void forward(void *context) {
//     ForwardPass *pass = context;
//     tl_forward(pass->net, pass->inputs, pass->outputs);
// }

int main(void) {
    bench_print_header();
    // bench_run("forward", forward, &pass, 1.0);
    return 0;
}

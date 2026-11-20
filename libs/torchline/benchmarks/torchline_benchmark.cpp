#include <benchmark/benchmark.h>

#include <vector>

extern "C" {
#include <torchline/torchline.h>
}

namespace {

// One forward pass of the compiled-in network, i.e. the cost per sample frame.
void BM_Forward(benchmark::State &state) {
    const TlNetwork *net = tl_load_network();
    std::vector<float> inputs(static_cast<size_t>(tl_input_size(net)), 0.25f);
    std::vector<float> outputs(static_cast<size_t>(tl_output_size(net)));

    for (auto _ : state) {
        tl_forward(net, inputs.data(), outputs.data());
        benchmark::DoNotOptimize(outputs.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}

} // namespace

BENCHMARK(BM_Forward);

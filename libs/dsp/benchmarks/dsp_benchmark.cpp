#include <benchmark/benchmark.h>

#include <cmath>
#include <vector>

extern "C" {
#include <dsp/amber.h>
#include <dsp/one_pole.h>
}

namespace {

constexpr double kSampleRate = 48000.0;
constexpr int kNumChannels = 2;

std::vector<float> make_sine_buffer(size_t num_frames) {
    std::vector<float> buffer(num_frames * kNumChannels);
    for (size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] = 0.5f * std::sin(static_cast<float>(i) * 0.01f);
    }
    return buffer;
}

// Runs one plugin over a block of the given size (state.range(0) frames), as the audio callback would.
void run_plugin(benchmark::State &state, Plugin *plugin) {
    const auto num_frames = static_cast<size_t>(state.range(0));
    // Processed in place every iteration; the output stays bounded, and the per-sample cost
    // does not depend on the signal, so re-feeding the previous block is fine.
    std::vector<float> buffer = make_sine_buffer(num_frames);

    for (auto _ : state) {
        plugin->process(plugin, buffer.data(), num_frames);
        benchmark::DoNotOptimize(buffer.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(num_frames));
    plugin->destroy(plugin);
}

void BM_AmberProcess(benchmark::State &state) {
    run_plugin(state, amber_create(kSampleRate, kNumChannels, 3.0f));
}

void BM_OnePoleProcess(benchmark::State &state) {
    run_plugin(state, one_pole_create(kSampleRate, kNumChannels, 0.1f));
}

} // namespace

// 1024 frames is the buffer size main.c uses.
BENCHMARK(BM_AmberProcess)->Arg(64)->Arg(256)->Arg(1024);
BENCHMARK(BM_OnePoleProcess)->Arg(64)->Arg(256)->Arg(1024);

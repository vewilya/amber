#include <stddef.h>
#include <stdio.h>

#include <bench/bench.h>
#include <dsp/amber.h>
#include <dsp/one_pole.h>

enum { NUM_CHANNELS = 2, MAX_FRAMES = 1024 }; // 1024 frames is the buffer size main.c uses
static const double SAMPLE_RATE = 48000.0;

typedef struct {
    Plugin *plugin;
    float *buffer;
    size_t num_frames;
} PluginBlock;

static void process_block(void *context) {
    PluginBlock *block = context;
    block->plugin->process(block->plugin, block->buffer, block->num_frames);
}

// Runs one plugin over blocks of several sizes, as the audio callback would. The buffer is
// processed in place every iteration; the output stays bounded, and the per-sample cost does not
// depend on the signal, so re-feeding the previous block is fine.
static void bench_plugin(const char *name, Plugin *plugin) {
    static float buffer[(size_t)MAX_FRAMES * NUM_CHANNELS];
    // Triangle wave in [-0.5, 0.5] with a 200-sample period: any bounded test signal will do.
    for (size_t i = 0; i < sizeof buffer / sizeof buffer[0]; ++i) {
        const float phase = (float)(i % 200) / 100.0f; // 0 .. 2
        buffer[i] = (phase < 1.0f ? phase : 2.0f - phase) - 0.5f;
    }

    const size_t block_sizes[] = {64, 256, MAX_FRAMES};
    for (size_t i = 0; i < sizeof block_sizes / sizeof block_sizes[0]; ++i) {
        char label[64];
        snprintf(label, sizeof label, "%s/%zu", name, block_sizes[i]);
        PluginBlock block = {.plugin = plugin, .buffer = buffer, .num_frames = block_sizes[i]};
        bench_run(label, process_block, &block, (double)block_sizes[i]);
    }
    plugin->destroy(plugin);
}

int main(void) {
    bench_print_header();
    bench_plugin("amber_process", amber_create(SAMPLE_RATE, NUM_CHANNELS, 3.0f));
    bench_plugin("one_pole_process", one_pole_create(SAMPLE_RATE, NUM_CHANNELS, 0.1f));
    return 0;
}

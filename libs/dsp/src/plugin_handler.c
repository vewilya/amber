#include <dsp/plugin_handler.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
// #include "types.h"

#define MAX_PLUGINS 10

static size_t plugin_count = 0;
static Plugin *plugin_chain[MAX_PLUGINS];

Err_t register_plugin_to_chain(Plugin *plugin_to_add) {
    if (plugin_count >= MAX_PLUGINS) {
        printf("%s", get_err_msg(PLUGIN_CHAIN_ERR));
        return PLUGIN_CHAIN_ERR;
    }

    plugin_chain[plugin_count++] = plugin_to_add;

    return NO_ERR;
};


void process_plugin_chain(const float *input, float *output, size_t num_frames, int num_channels) {
    memcpy(output, input, num_frames * num_channels * sizeof *output);

    for (size_t i = 0; i < plugin_count; i++) {
        Plugin *p = plugin_chain[i];
        p->process(p, output, num_frames);
    }
};

void shutdown_all_plugins(void) {
    for (size_t i = 0; i < plugin_count; i++) {
        Plugin *p = plugin_chain[i];
        p->destroy(p);
    }
};

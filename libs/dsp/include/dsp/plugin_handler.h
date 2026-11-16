#ifndef PLUGIN_HANDLER_H
#define PLUGIN_HANDLER_H

#include <dsp/plugin.h>

Err_t register_plugin_to_chain(Plugin *plugin_to_add);
Err_t init_plugin_chain(double sample_rate, int num_channels);
void process_plugin_chain(const float *input, float *output, size_t num_frames, int num_channels);
void shutdown_all_plugins(void);

#endif // PLUGIN_HANDLER_H

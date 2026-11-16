#include <dsp/audio_component.h>
#include <criterion/criterion.h>

Test(audio_component, set_gain_stores_value) {
  AudioComponent component;
  audio_component_init(&component, 48000.0, 2);
  audio_component_set_gain(&component, 0.5f);
  cr_assert_float_eq(component.gain, 0.5f, 1e-6);
}

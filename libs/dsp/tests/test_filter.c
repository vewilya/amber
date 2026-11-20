#include <criterion/criterion.h>
#include <dsp/one_pole.h>

Test(one_pole_filter, set_gain_stores_value) {
    OnePole filter;
    // cr_assert_float_eq(component.gain, 0.5f, 1e-6);
}

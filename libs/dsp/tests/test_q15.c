#include <dsp/q15.h>
#include <criterion/criterion.h>

Test(q15, float_from_q15) {
  cr_assert_float_eq(float_from_q15(200), 0.0061035156, 1e-6);
}

Test(q15, q15_from_float) {
     cr_expect_eq(q15_from_float( 1.0f), Q15_MAX);
     cr_expect_eq(q15_from_float(0.5f), 16384);
     cr_expect_eq(q15_from_float(-0.5f), -16384);
     cr_expect_eq(q15_from_float(-1.0f), -32768);
     cr_expect_eq(q15_from_float(0.0f), 0);
}

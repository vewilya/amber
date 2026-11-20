#include <dsp/inline_test.h>
#include <stdint.h>

#ifndef Q15_H
#define Q15_H

typedef int16_t q15_t;

#define Q15_MAX INT16_MAX //  32767 ≈ +0.99997
#define Q15_MIN INT16_MIN // -32768 = -1.0
#define Q15_SCALE 32768.0f

static inline q15_t q15_saturate(int32_t value) {
    if (value > Q15_MAX)
        return Q15_MAX;
    if (value < Q15_MIN)
        return Q15_MIN;
    return (q15_t)value;
}

static inline q15_t q15_add(q15_t a, q15_t b) {
    int32_t sum = (int32_t)a + b;
    return q15_saturate(sum);
}

static inline q15_t q15_mul(q15_t a, q15_t b) {
    int32_t product = (int32_t)a * b;
    int32_t shifted = (product + (1 << 14)) >> 15;
    return q15_saturate(shifted);
}

static inline q15_t q15_from_float(float x) {
    if (x >= 1.0f)
        return Q15_MAX;
    if (x <= -1.0f)
        return Q15_MIN;
    return (q15_t)(x * Q15_SCALE);
}

static inline float float_from_q15(q15_t x) { return (float)x * (1.0f / Q15_SCALE); }

#endif

TEST(q15, multiplication) { CHECK_EQ(q15_mul(16384, 16384), 8192); }

TEST(q15, addition) {
    CHECK_EQ(q15_add(1111, 1111), 2222);
    CHECK_EQ(q15_add(16384, 16384), Q15_MAX);
}

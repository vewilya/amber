/* inline_test.h */
#ifndef INLINE_TEST_H
#define INLINE_TEST_H

#include <stdio.h>

#ifdef RUN_TESTS
#include <criterion/criterion.h>
#define TEST(suite, name)    Test(suite, name)
#define CHECK_EQ(a, b)       cr_expect_eq(a, b)
#define CHECK_STR_EQ(a, b)   cr_expect_str_eq(a, b)
#else
#define TEST(suite, name)    static inline void suite##_##name(void)
#define CHECK_EQ(a, b)       ((void)0)
#define CHECK_STR_EQ(a, b)   ((void)0)
#endif

#endif // INLINE_TEST_H

#ifndef BENCH_H
#define BENCH_H

/// @file
/// Minimal benchmark harness. Each benchmark grows its iteration count until one run takes long
/// enough to time reliably, then repeats that run a few times and reports the fastest one (the
/// run least disturbed by the OS).

#include <stdint.h>
#include <stdio.h>
#include <time.h>

/// The code under test; called once per iteration with the context given to bench_run().
typedef void (*BenchFn)(void *context);

static inline double bench_now_seconds(void) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (double)now.tv_sec + (double)now.tv_nsec * 1e-9;
}

static inline double bench_time_iterations(BenchFn fn, void *context, uint64_t iterations) {
    const double start = bench_now_seconds();
    for (uint64_t i = 0; i < iterations; ++i) {
        fn(context);
    }
    return bench_now_seconds() - start;
}

static inline void bench_print_header(void) {
    printf("%-28s %14s %18s %12s\n", "Benchmark", "Time", "Throughput", "Iterations");
}

/// Times fn and prints one result line.
/// @param items_per_call Work done per call (e.g. frames per process() call), for the
///                       throughput column.
static inline void bench_run(const char *name, BenchFn fn, void *context, double items_per_call) {
    enum { REPETITIONS = 5 };
    const double min_run_seconds = 0.1;

    uint64_t iterations = 1;
    while (bench_time_iterations(fn, context, iterations) < min_run_seconds) {
        iterations *= 2;
    }

    double best_seconds = bench_time_iterations(fn, context, iterations);
    for (int repetition = 1; repetition < REPETITIONS; ++repetition) {
        const double seconds = bench_time_iterations(fn, context, iterations);
        if (seconds < best_seconds) {
            best_seconds = seconds;
        }
    }

    const double ns_per_call = best_seconds * 1e9 / (double)iterations;
    const double million_items_per_second =
        items_per_call * (double)iterations / best_seconds / 1e6;
    printf("%-28s %11.1f ns %10.2f Mitem/s %12llu\n", name, ns_per_call, million_items_per_second,
           (unsigned long long)iterations);
}

#endif // BENCH_H

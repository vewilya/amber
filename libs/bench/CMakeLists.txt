# Header-only benchmark harness shared by the libs/*/benchmarks executables.
add_library(bench INTERFACE)
target_include_directories(bench INTERFACE include)
# clock_gettime/CLOCK_MONOTONIC are POSIX, hidden by strict -std=c23 on glibc.
target_compile_definitions(bench INTERFACE _POSIX_C_SOURCE=200809L)

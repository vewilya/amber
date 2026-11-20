cmake_minimum_required(VERSION 3.25)
project(Amber VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build unit tests (Criterion)" ON)
option(BUILD_BENCHMARKS "Build benchmarks (Google Benchmark, needs C++)" OFF)
option(ENABLE_SANITIZERS "Build with address and undefined-behavior sanitizers" OFF)
option(ENABLE_COVERAGE "Build with clang source-based coverage" OFF)
option(ENABLE_CLANG_TIDY "Run clang-tidy on the libraries and app while compiling" OFF)

# Only the benchmarks are C++; everything else stays plain C.
if(BUILD_BENCHMARKS)
  enable_language(CXX)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  find_package(benchmark REQUIRED)
endif()

include(cmake/compiler-options.cmake)

if(BUILD_TESTS)
  enable_testing()   # before add_subdirectory, so ctest sees the library tests
  find_library(CRITERION_LIB criterion REQUIRED)
  find_path(CRITERION_INCLUDE criterion/criterion.h REQUIRED)
endif()

add_subdirectory(libs/dsp)
add_subdirectory(libs/torchline)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PORTAUDIO REQUIRED IMPORTED_TARGET portaudio-2.0)

add_executable(amber main.c)
target_link_libraries(amber PRIVATE dsp torchline PkgConfig::PORTAUDIO)

if(ENABLE_CLANG_TIDY)
  set_target_properties(dsp torchline amber PROPERTIES C_CLANG_TIDY "${clang_tidy_command}")
endif()

include(cmake/developer-targets.cmake)

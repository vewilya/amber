add_library(torchline STATIC
    src/engine.c
)

target_include_directories(torchline
  PUBLIC include
  PRIVATE src
)
target_link_libraries(torchline PRIVATE m)  # tanhf, expf

if(BUILD_TESTS)
  add_executable(torchline_tests
      tests/test_neuron.c
  )
  target_include_directories(torchline_tests PRIVATE ${CRITERION_INCLUDE})
  target_link_libraries(torchline_tests PRIVATE torchline ${CRITERION_LIB})
  add_test(NAME torchline_tests COMMAND torchline_tests)
endif()

if(BUILD_BENCHMARKS)
  add_executable(torchline_benchmarks benchmarks/torchline_benchmark.cpp)
  target_link_libraries(torchline_benchmarks PRIVATE torchline benchmark::benchmark_main)
endif()

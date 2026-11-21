add_library(gratlin STATIC
    src/gratlin.c
)

target_include_directories(gratlin
  PUBLIC include
  PRIVATE src
)
target_link_libraries(gratlin PRIVATE m)  # tanhf, expf

if(BUILD_TESTS)
  add_executable(gratlin_tests
      tests/test_gratlin.c
  )
  target_include_directories(gratlin_tests PRIVATE ${CRITERION_INCLUDE})
  target_link_libraries(gratlin_tests PRIVATE gratlin ${CRITERION_LIB})
  add_test(NAME gratlin_tests COMMAND gratlin_tests)
endif()

if(BUILD_BENCHMARKS)
  add_executable(gratlin_benchmarks benchmarks/gratlin_benchmark.c)
  target_link_libraries(gratlin_benchmarks PRIVATE gratlin bench)
endif()

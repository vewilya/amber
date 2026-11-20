add_library(dsp STATIC
    src/audio_component.c
    src/one_pole.c
    src/plugin_handler.c
    src/amber.c
)

target_include_directories(dsp PUBLIC include)
target_link_libraries(dsp PRIVATE torchline)

if(BUILD_TESTS)
  add_executable(dsp_tests
      tests/test_component.c
      tests/test_q15.c
      # tests/test_filter.c   # TODO: rewrite against one_pole_create (OnePole is private now)
  )
  target_include_directories(dsp_tests PRIVATE ${CRITERION_INCLUDE})
  target_link_libraries(dsp_tests PRIVATE dsp ${CRITERION_LIB})
  add_test(NAME dsp_tests COMMAND dsp_tests)
endif()

if(BUILD_BENCHMARKS)
  add_executable(dsp_benchmarks benchmarks/dsp_benchmark.cpp)
  target_link_libraries(dsp_benchmarks PRIVATE dsp benchmark::benchmark_main)
endif()

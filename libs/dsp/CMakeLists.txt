add_library(dsp STATIC
    src/audio_component.c
    src/one_pole.c
    src/plugin_handler.c
    src/amber.c
)

target_include_directories(dsp PUBLIC include)
target_link_libraries(dsp PRIVATE torchline)

set_target_properties(dsp PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_compile_options(dsp PRIVATE -Wall -Wextra)

# --- Tests ---
find_library(CRITERION_LIB criterion REQUIRED)
find_path(CRITERION_INCLUDE criterion/criterion.h REQUIRED)

add_executable(dsp_tests
    tests/test_component.c
    tests/test_q15.c
    # tests/test_filter.c   # TODO: rewrite against one_pole_create (OnePole is private now)
)
target_include_directories(dsp_tests PRIVATE ${CRITERION_INCLUDE})
target_link_libraries(dsp_tests PRIVATE dsp ${CRITERION_LIB})

add_test(NAME dsp_tests COMMAND dsp_tests)

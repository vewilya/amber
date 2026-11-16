add_library(torchline STATIC
    src/engine.c
)

target_include_directories(torchline
  PUBLIC include
  PRIVATE src
)

set_target_properties(torchline PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_compile_options(torchline PRIVATE -Wall -Wextra)
target_link_libraries(torchline PRIVATE m)  # tanhf, expf

# --- Tests ---
find_library(CRITERION_LIB criterion REQUIRED)
find_path(CRITERION_INCLUDE criterion/criterion.h REQUIRED)

add_executable(torchline_tests
    tests/test_neuron.c
)
target_include_directories(torchline_tests PRIVATE ${CRITERION_INCLUDE})
target_link_libraries(torchline_tests PRIVATE torchline ${CRITERION_LIB})

add_test(NAME torchline_tests COMMAND torchline_tests)

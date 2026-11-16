cmake_minimum_required(VERSION 3.21)

project(Amber LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()   # must come before add_subdirectory, so ctest sees the library tests

add_subdirectory(libs/dsp)
add_subdirectory(libs/torchline)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PORTAUDIO REQUIRED IMPORTED_TARGET portaudio-2.0)

add_executable(amber main.c)

set_target_properties(amber PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_compile_options(amber PRIVATE -Wall -Wextra)
target_link_libraries(amber PRIVATE dsp torchline PkgConfig::PORTAUDIO)

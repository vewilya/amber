find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs")
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_EXTRACT_STATIC YES)
  set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
  doxygen_add_docs(docs libs/dsp/include libs/torchline/include README.md
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endif()

# Generated files are skipped via .clang-format-ignore.
find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
  file(GLOB_RECURSE formattable_files CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/main.c"
    "${PROJECT_SOURCE_DIR}/libs/*/include/*.[ch]*"
    "${PROJECT_SOURCE_DIR}/libs/*/src/*.[ch]*"
    "${PROJECT_SOURCE_DIR}/libs/*/tests/*.[ch]*"
    "${PROJECT_SOURCE_DIR}/libs/*/benchmarks/*.[ch]*")
  add_custom_target(format
    COMMAND "${CLANG_FORMAT_EXE}" -i ${formattable_files}
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
  add_custom_target(format-check
    COMMAND "${CLANG_FORMAT_EXE}" --dry-run --Werror ${formattable_files}
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endif()

if(ENABLE_COVERAGE AND BUILD_TESTS)
  if(APPLE)
    set(llvm_tool_prefix xcrun)
  endif()
  # Criterion runs every test in its own process, so each one writes its own profile (%p = pid).
  set(profile_dir "${CMAKE_BINARY_DIR}/coverage-profiles")
  set(merged_profile "${CMAKE_BINARY_DIR}/coverage.profdata")
  set(test_binaries $<TARGET_FILE:dsp_tests> -object $<TARGET_FILE:torchline_tests>)
  set(covered_sources
    "${PROJECT_SOURCE_DIR}/libs/dsp/src" "${PROJECT_SOURCE_DIR}/libs/dsp/include"
    "${PROJECT_SOURCE_DIR}/libs/torchline/src" "${PROJECT_SOURCE_DIR}/libs/torchline/include")
  add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${profile_dir}"
    COMMAND ${CMAKE_COMMAND} -E env "LLVM_PROFILE_FILE=${profile_dir}/%p.profraw" $<TARGET_FILE:dsp_tests>
    COMMAND ${CMAKE_COMMAND} -E env "LLVM_PROFILE_FILE=${profile_dir}/%p.profraw" $<TARGET_FILE:torchline_tests>
    COMMAND sh -c "${llvm_tool_prefix} llvm-profdata merge -sparse '${profile_dir}'/*.profraw -o '${merged_profile}'"
    COMMAND ${llvm_tool_prefix} llvm-cov report ${test_binaries}
      "-instr-profile=${merged_profile}" ${covered_sources}
    COMMAND ${llvm_tool_prefix} llvm-cov show ${test_binaries}
      "-instr-profile=${merged_profile}" -format=html
      "-output-dir=${CMAKE_BINARY_DIR}/coverage-html" ${covered_sources}
    VERBATIM)
  add_dependencies(coverage dsp_tests torchline_tests)
endif()


option(NEXIS_ENABLE_FUZZING "Build tests with llvm libFuzzer (only on clang)" OFF)

if (NEXIS_ENABLE_FUZZING)
  if (NOT CMAKE_C_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "Tests with llvm libFuzzer requires clang compiler got '${CMAKE_C_COMPILER_ID}'")
  endif()

  if (NEXIS_BUILD_EDITOR)
    message(FATAL_ERROR "Cant build editor when building nexis tests. Use the -DNEXIS_BUILD_EDITOR=OFF flag when building the tests")
  endif()

  add_executable(fuzz_parser parser.c)
  target_compile_features(fuzz_parser PRIVATE c_std_99)
  target_include_directories(fuzz_parser PRIVATE ../include)
  target_link_libraries(fuzz_parser PRIVATE Nexis::Core)

  set(NEXIS_FUZZ_FLAGS -fsanitize=fuzzer,address,undefined -fno-sanitize-recover=all -g -O1)
  target_compile_options(nexis_core PRIVATE ${NEXIS_FUZZ_FLAGS})
  target_compile_options(fuzz_parser PRIVATE ${NEXIS_FUZZ_FLAGS})
  target_link_options(fuzz_parser PRIVATE ${NEXIS_FUZZ_FLAGS})
endif()

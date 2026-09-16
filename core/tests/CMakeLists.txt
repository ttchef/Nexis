
add_executable(fuzz_serializer
  serializer.c
)
target_compile_features(fuzz_serializer PRIVATE c_std_99)
target_include_directories(fuzz_serializer PRIVATE
  ../include
)

target_link_libraries(fuzz_serializer PRIVATE Nexis::Core)

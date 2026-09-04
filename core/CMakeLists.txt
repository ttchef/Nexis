
add_library(nexis_core STATIC
  src/particle.c
)
add_library(Nexis::Core ALIAS nexis_core)

target_include_directories(nexis_core PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(nexis_core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(nexis_core PUBLIC c_std_99)

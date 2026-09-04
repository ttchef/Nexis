
add_library(nexis_core STATIC
  src/particle.cpp
  src/project.cpp
  src/asset_manager.cpp
  src/globals.cpp
  src/utils.cpp
)
add_library(Nexis::Core ALIAS nexis_core)

target_include_directories(nexis_core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_features(nexis_core PUBLIC cxx_std_23)

target_link_libraries(nexis_core PUBLIC
  raylib
)

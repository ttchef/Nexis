
set(NEXIS_EXAMPLE_INCLUDE_PATHS ../core/include ../backends)
set(NEXIS_EXAMPLE_LINK_FLAGS Nexis::Core raylib)

file(COPY ${CMAKE_SOURCE_DIR}/examples/assets DESTINATION ${CMAKE_BINARY_DIR}/examples/)

set(NEXIS_CURRENT_EXAMPLE raylib-game)
add_executable(${NEXIS_CURRENT_EXAMPLE} raylib_game.cpp)
target_include_directories(${NEXIS_CURRENT_EXAMPLE} PRIVATE ${NEXIS_EXAMPLE_INCLUDE_PATHS})
target_link_libraries(${NEXIS_CURRENT_EXAMPLE} ${NEXIS_EXAMPLE_LINK_FLAGS})


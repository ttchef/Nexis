cmake_minimum_required(VERSION 3.15)
project(raylib-game-example)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../../core/include)
target_link_libraries(${PROJECT_NAME} PRIVATE Nexis::Core)


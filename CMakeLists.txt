cmake_minimum_required(VERSION 3.31)
project(SoftbodiesSimulation_ISC VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(my_lib
    cpp/src/utils.cpp
)

target_include_directories(my_lib PUBLIC cpp/include)

add_executable(my_project cpp/src/main.cpp)
target_link_libraries(my_project my_lib)


enable_testing()
add_subdirectory(tests)

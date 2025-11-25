cmake_minimum_required(VERSION 3.20)   # safer, widely available
project(SoftbodiesSimulation_ISC VERSION 0.0.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library target (all .cpp files in src except main.cpp)
#add_library(my_lib
#    cpp/src/Utils.cpp
    # add other .cpp files here
#)
#TODO remove at last (not safe)
file(GLOB SRC_FILES CONFIGURE_DEPENDS cpp/src/*.cpp)
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/cpp/src/main.cpp")
add_library(my_lib ${SRC_FILES})

# Make headers visible to anything linking my_lib
target_include_directories(my_lib PUBLIC cpp/include)

# Executable target (main program)
add_executable(my_project cpp/src/main.cpp)

# Link executable with your library
target_link_libraries(my_project PRIVATE my_lib)

# Testing setup
enable_testing()
add_subdirectory(tests)

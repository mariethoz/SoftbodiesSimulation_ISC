cmake_minimum_required(VERSION 3.13)
project(SoftbodiesSimulation_ISC VERSION 0.1.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable folders in IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ------------------------
# Build simulation library
# ------------------------

file(GLOB SRC_FILES CONFIGURE_DEPENDS cpp/src/*.cpp)

# Remove all demo mains from the library
list(REMOVE_ITEM SRC_FILES
     "${CMAKE_CURRENT_SOURCE_DIR}/cpp/src/main.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/cpp/src/main_particles.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/cpp/src/main_constraint_animation.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/cpp/src/main_constraint_plots copy.cpp")

add_library(my_lib ${SRC_FILES})
target_include_directories(my_lib PUBLIC cpp/include)

# ------------------------
# Build the main executable
# ------------------------

add_executable(my_project cpp/src/main.cpp)
target_link_libraries(my_project PRIVATE my_lib)

# ------------------------
# Testing
# ------------------------
include(CTest)
enable_testing()
add_subdirectory(tests)

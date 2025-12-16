cmake_minimum_required(VERSION 3.13)
project(SoftbodiesSimulation_ISC VERSION 1.0.0 LANGUAGES CXX)

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
     "${CMAKE_CURRENT_SOURCE_DIR}/cpp/src/main_constraint_plots.cpp")

add_library(my_lib ${SRC_FILES})
target_include_directories(my_lib PUBLIC cpp/include)

# ------------------------
# Include nlohmann_json library
# ------------------------

include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(my_lib PUBLIC nlohmann_json::nlohmann_json)

# ------------------------
# Build the main executable
# ------------------------

add_executable(my_project cpp/src/main.cpp)
target_link_libraries(my_project PRIVATE my_lib)
add_executable(particles cpp/src/main_particles.cpp)
target_link_libraries(particles PRIVATE my_lib)
add_executable(softbody_plot cpp/src/main_constraint_plots.cpp)
target_link_libraries(softbody_plot PRIVATE my_lib)
add_executable(softbody_animation cpp/src/main_constraint_animation.cpp)
target_link_libraries(softbody_animation PRIVATE my_lib)

# ------------------------
# Testing
# ------------------------
include(CTest)
enable_testing()
add_subdirectory(tests)

# Download GoogleTest using CMake's FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_runner TestUtils.cpp)
target_link_libraries(test_runner my_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(test_runner)

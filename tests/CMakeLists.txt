# tests/CMakeLists.txt
# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ------------------------
# Test executable
# ------------------------

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "cpp/*.cpp")

add_executable(test_runner ${TEST_SOURCES})
target_link_libraries(test_runner PRIVATE my_lib gtest gtest_main)

# ------------------------
# Register Google Tests
# ------------------------
include(GoogleTest)
gtest_discover_tests(test_runner)

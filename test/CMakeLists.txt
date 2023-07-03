include(${CMAKE_SCRIPTS_DIR}/googletest.cmake)
set(current_target opengl-starter-test)

add_executable(
  ${current_target}
  test_main.cpp
)

target_link_libraries(
  ${current_target}
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(${current_target})

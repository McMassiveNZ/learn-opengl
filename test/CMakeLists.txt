include(gtest)
set(current_target learn-opengl-test)

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

include(gtest)

set(current_target wmcv-log-test)

add_executable(
  ${current_target}
  test_wmcv_log.cpp
  test_wmcv_format.cpp
  test_pch.h
)

target_link_libraries(
  ${current_target}
  wmcv-log
  gtest_main
  gmock_main
)

if(MSVC)
    target_sources(${current_target} PRIVATE test_pch.cpp)
endif()

target_precompile_headers(${current_target} PRIVATE test_pch.h test_pch.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

include(GoogleTest)
gtest_discover_tests(${current_target})

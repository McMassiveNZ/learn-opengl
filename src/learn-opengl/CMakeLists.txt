cmake_minimum_required(VERSION 3.23)

project(learn-opengl VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
OPTION(ENABLE_ALL_REASONABLE_WARNINGS "Enable all possible reasonable warnings" ON )
OPTION(ENABLE_WARNINGS_AS_ERRORS "Warnings are treated as Errors" ON)
OPTION(ENABLE_STATIC_ANALYSIS "Enable Static Analysis Tools" OFF)
OPTION(ENABLE_SANITIZERS "Enable Sanitizer Tools" OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(compilerwarnings)
include(sanitizers)
include(staticanalysis)
include(cpm)

add_subdirectory(src)
add_subdirectory(include)

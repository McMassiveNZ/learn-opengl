set(current_target opengl-starter)

set(SOURCE_FILES)
set(OPENGL_STARTER_SRC
    main.cpp
    window.h
    opengl.h
)
source_group("" FILES ${OPENGL_STARTER_SRC})

if(MSVC)
set(PLATFORM_SRC
    platform/win32_window.cpp
    platform/win32_opengl.cpp
)
else()
set(PLATFORM_SRC
    platform/null_window.cpp
    platform/null_opengl.cpp
)
endif()
source_group(platform FILES ${PLATFORM_SRC})

list(APPEND SOURCE_FILES ${PLATFORM_SRC})
list(APPEND SOURCE_FILES ${OPENGL_STARTER_SRC})

add_executable(
    ${current_target}
    ${SOURCE_FILES}
)

target_include_directories(${current_target} PUBLIC . ${CMAKE_SOURCE_DIR}/external)

if(MSVC)
	target_link_libraries(${current_target} PUBLIC opengl32.lib)
endif()

if( ENABLE_ALL_REASONABLE_WARNINGS )
    MESSAGE("-- Additional Warnings Enabled")
    target_enable_warnings(${current_target})
endif()

if( ENABLE_WARNINGS_AS_ERRORS )
    MESSAGE("-- Warnings as Errors")
    target_warnings_as_errors(${current_target})
endif()

if( ENABLE_SANITIZERS )
    MESSAGE("-- Sanitizers Enabled")
    target_enable_sanitizers(${current_target})
endif()

if( ENABLE_STATIC_ANALYSIS )
    MESSAGE("-- Static Analysis Enabled")
    target_enable_static_analysis(${current_target})
endif()

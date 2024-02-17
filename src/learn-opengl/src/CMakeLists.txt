include(glm)

set(current_target learn-opengl)

add_executable(learn-opengl "")

target_sources( 
    ${current_target}
    PRIVATE
        pch.h
        main.cpp
        shader.cpp
        texture.cpp
        camera.cpp
        clock.cpp
)

if( MSVC )
    target_sources(${current_target} PRIVATE 
        platform/win32_window.cpp
        platform/win32_opengl.cpp
        platform/win32_application.cpp
        platform/win32_input.cpp
        pch.cpp
    )
	target_link_libraries(${current_target} PRIVATE opengl32.lib)
	target_compile_definitions(${current_target} PUBLIC -DWMCV_PLATFORM_WINDOWS)
else()
    target_sources(${current_target} PRIVATE 
        platform/null_window.cpp
        platform/null_opengl.cpp
        platform/null_application.cpp
    )
endif()

target_include_directories(
    ${current_target} 
        PUBLIC 
            .
			${CMAKE_SOURCE_DIR}/external 
			${GLM_SOURCE_DIR}
)

set(SHADERS)
list(APPEND SHADERS
    ${SHADERS_DIR}/cube.vs
    ${SHADERS_DIR}/cube.fs
    ${SHADERS_DIR}/light.vs
    ${SHADERS_DIR}/light.fs
)
source_group(shaders FILES ${SHADERS})
target_sources(${current_target} PRIVATE ${SHADERS})

target_precompile_headers(${current_target} PRIVATE pch.h pch.cpp)
target_link_libraries(${current_target} PRIVATE wmcv-log)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

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

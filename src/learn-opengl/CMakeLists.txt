include(glm)

set(current_target learn-opengl)
add_executable(${current_target} "" )

target_sources(
    ${current_target}
    PRIVATE
		main.cpp
		pch.h
		window.h
		opengl.h
		shader.h
		shader.cpp
		vert.txt
		frag.txt
)

source_group("shaders" FILES vert.txt frag.txt)

if(MSVC)
	target_sources( ${current_target} PRIVATE pch.cpp )
endif()

if(MSVC)
	target_sources( 
        ${current_target} 
        PRIVATE 
			platform/win32_window.cpp
			platform/win32_opengl.cpp
    )
else()
	target_sources( 
        ${current_target} 
        PRIVATE 
			platform/null_window.cpp
			platform/null_opengl.cpp
    )
endif()

if(MSVC)
target_link_libraries(${current_target} PRIVATE opengl32.lib)
target_compile_definitions(${current_target} PUBLIC -DWMCV_PLATFORM_WINDOWS)
endif()

target_include_directories(${current_target} PUBLIC 
    .
    ../log/include
    ${CMAKE_SOURCE_DIR}/external 
    ${GLM_SOURCE_DIR}
)
target_precompiled_header(${current_target} pch.h pch.cpp)
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

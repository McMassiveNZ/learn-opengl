set(current_target wmcv-log)

add_library(
    wmcv-log ""
)

target_sources( 
    wmcv-log
    PRIVATE
        pch.h
        wmcv_log_system.cpp
		wmcv_format.cpp
        wmcv_sink_outputdbgstring.cpp
)

if( MSVC )
    target_sources(wmcv-log PRIVATE pch.cpp)
endif()

target_precompile_headers(wmcv-log PRIVATE pch.h pch.cpp)
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

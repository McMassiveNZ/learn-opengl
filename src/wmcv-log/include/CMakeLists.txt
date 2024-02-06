target_sources(
    wmcv-log
        PRIVATE
            wmcv_log/wmcv_log.h
            wmcv_log/wmcv_log_sink.h
            wmcv_log/wmcv_log_system.h
	wmcv_log/wmcv_format.h
            wmcv_log/sinks/wmcv_sink_outputdbgstring.h
)

target_include_directories(
    wmcv-log
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/wmcv_log
        PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

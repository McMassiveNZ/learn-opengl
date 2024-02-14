target_sources(
    learn-opengl
        PRIVATE
            opengl.h
            shader.h
            texture.h
            window.h
            application.h
            input.h
            camera.h
            clock.h
)

target_include_directories(
    learn-opengl
        PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

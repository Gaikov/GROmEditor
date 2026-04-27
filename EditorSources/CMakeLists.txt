project(GROmEditor)

file(GLOB_RECURSE sources "*.cpp" "*.c" "*.h")

set(LINK_ENGINE_LIBS ${GROM_ENGINE_LIBS} Networking)

if (ANDROID)
    add_library(${GAME_MODULE_NAME} STATIC ${sources})
    target_link_libraries(${GAME_MODULE_NAME} ${LINK_ENGINE_LIBS})
elseif (WEB_ASM)
    add_executable(${GAME_MODULE_NAME} ${sources})

    set_target_properties(index PROPERTIES
            LINK_FLAGS "-sSTACK_SIZE=500000 -sUSE_WEBGL2=0 -sUSE_GLFW=3 -sMAX_WEBGL_VERSION=1 -sMIN_WEBGL_VERSION=1 -sLEGACY_GL_EMULATION=1 --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell_minimal.html --preload-file ${CMAKE_SOURCE_DIR}/assets@/"
    )

    target_link_libraries(${GAME_MODULE_NAME} ${LINK_ENGINE_LIBS})
else()
    add_executable(${GAME_MODULE_NAME} ${sources})
    target_link_libraries(${GAME_MODULE_NAME} ${LINK_ENGINE_LIBS} imgui)
    target_link_options(${GAME_MODULE_NAME} PRIVATE -mwindows)
endif ()

include_directories(./)

project(SceneViewer)

set(SOURCES
        SceneViewerApp.cpp
)

include_directories(
        ./
        ../
)

add_executable(SceneViewer ${SOURCES})

target_link_libraries(SceneViewer ${ENGINE_LIBS})

project(SceneViewer)

set(SOURCES
        SceneViewerApp.cpp
        SVMainView.cpp
        SVMainView.h
)

include_directories(
        ./
        ../
)

add_executable(SceneViewer ${SOURCES})

target_link_libraries(SceneViewer ${ENGINE_LIBS})

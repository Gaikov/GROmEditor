project(SceneViewer)

set(SOURCES
        SceneViewerApp.cpp
        SVMainView.cpp
        SVMainView.h
        SVSceneView.cpp
        SVSceneView.h
        SVUtils.cpp
        SVUtils.h
        SVModel.cpp
        SVModel.h
)

include_directories(
        ./
        ../
)

add_executable(SceneViewer ${SOURCES})

target_link_libraries(SceneViewer ${ENGINE_LIBS} imgui)

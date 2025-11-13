project(SceneViewer)

file(GLOB_RECURSE SOURCES "*.cpp" "*.c" "*.h")

include_directories(
        ./
        ../
)

add_executable(SceneViewer ${SOURCES})

target_link_libraries(SceneViewer ${ENGINE_LIBS} imgui)

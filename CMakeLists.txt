cmake_minimum_required(VERSION 3.13)
project(GROmEditor)

message(STATUS "Hello from GROm!")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

if (WEB_ASM)
    set(GAME_MODULE_NAME index)
else()
    set(GAME_MODULE_NAME GROmEditor)
endif()

set(CMAKE_CXX_STANDARD 20)

if (WEB_ASM)
    add_definitions(-DWEB_ASM)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
else ()
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(DEBUG)
endif ()

include_directories(
        ./
        GROmEngine
        GROmEngine/ThirdParty
)

add_subdirectory(GROmEngine)

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    add_subdirectory(Android/app/src/main/cpp)
endif()

add_subdirectory(EditorSources)

add_dependencies(${GAME_MODULE_NAME} Engine)
#print_all_variables()
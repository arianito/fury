cmake_minimum_required(VERSION 3.2)
project(game)
set(CMAKE_CXX_STANDARD 17)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable build of docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable build of tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable build of examples" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)

add_subdirectory(ext/glfw)
include_directories(${glfw_INCLUDE_DIRS})

file(
        GLOB_RECURSE
        SOURCES
        core/*.cpp
        core/*.h
        ext/imgui/*.cpp
        ext/imgui/*.h
        ext/glad/*.c
        ext/glad/*.h
)

include_directories(ext/glad/include)
include_directories(ext/imgui/include)
include_directories(ext/stb/include)
include_directories(core/include)
set(name app)
add_executable(app ${SOURCES} src/main5.cpp)
target_link_libraries(app glfw)
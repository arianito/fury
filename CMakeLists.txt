cmake_minimum_required(VERSION 3.2)

project(game)

set(CMAKE_CXX_STANDARD 17)

# GRAPHIC LIBRARIES
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable build of docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable build of tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable build of examples" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)
add_subdirectory(ext/glad)
include_directories(${glad_INCLUDE_DIRS})
add_subdirectory(ext/glfw)
include_directories(${glfw_INCLUDE_DIRS})

# CORE FUNCTIONS AND CLASSES
file(GLOB_RECURSE CORE_FILES
    core/*.cpp
    core/*.h
)
include_directories(core/include)


set(name app)
add_executable(app ${CORE_FILES} src/main3.cpp)
target_link_libraries(app glad glfw)
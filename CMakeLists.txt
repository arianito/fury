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
include_directories(core)


function(PFolder path)
    set(name bin_${path})
    file(GLOB_RECURSE P1_FILES
            src/${path}/*.cpp
            src/${path}/*.h
            )
    add_executable(${name} ${CORE_FILES} ${P1_FILES})
    target_link_libraries(${name} glad glfw)
endfunction(PFolder)


# PFolder(z01_rect)
# PFolder(z02_point)
# PFolder(z03_viewport)
PFolder(z04_ok)
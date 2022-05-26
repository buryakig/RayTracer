cmake_minimum_required(VERSION 3.16)
set(CMAKE_SUPPRESS_REGENERATION true)

set(PROJECT_NAME ImAppDemo)

set (CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME} CXX)

include(CMake/CMakeMacros.txt)

include_directories(.)
include_directories(ImApp/submodules/src)
include_directories(ImApp/submodules/src/imgui)
include_directories(ImApp/submodules/src/glfw/include)
include_directories(ImApp/submodules/src/glm)
include_directories(ImApp/submodules/glad/include)
include_directories(ImApp/submodules/src/stb)
include_directories(ImApp/src/)

add_subdirectory(imapp ${CMAKE_CURRENT_BINARY_DIR}/ImApp)
if(NOT EXTERNAL)
	add_subdirectory(sandbox ${CMAKE_CURRENT_BINARY_DIR}/Sandbox)
	target_link_libraries(Sandbox PRIVATE ImApp)

	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
endif()
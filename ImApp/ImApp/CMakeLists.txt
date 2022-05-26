set(LIB_NAME ImApp)

include(../CMake/CMakeMacros.txt)

# Dunno
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Parameters to build GLFW with
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

# Adding submodules CmakeLists to this one
add_subdirectory(submodules/src/glfw)

# Setting macros for glad linking
set(GLAD_SOURCES submodules/glad/src/glad.c submodules/glad/include/glad/glad.h)# 

add_library(glad ${GLAD_SOURCES})

set_property(TARGET glfw          PROPERTY FOLDER "ThirdPartyLibraries")
set_property(TARGET glad          PROPERTY FOLDER "ThirdPartyLibraries")

file(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.c??)
file(GLOB_RECURSE HEADER_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.h)
file(GLOB_RECURSE SHADER_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} res/Shaders/*)

add_library(${LIB_NAME} STATIC ${SRC_FILES} ${HEADER_FILES})

SETUP_GROUPS("${SRC_FILES}")
SETUP_GROUPS("${HEADER_FILES}")

set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDART 20)
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

if(MSVC)
	add_definitions(-D_CONSOLE)
	set_property(TARGET ${LIB_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY  "${CMAKE_SOURCE_DIR}")
endif()

target_link_libraries(${LIB_NAME} glfw glad)

# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (PGK)

find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-1c73e35")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP





include_directories(
	external/AntTweakBar-1.15/include/
	external/glfw-2.7.6/include/
	external/glm-0.9.1/
	external/glew-1.5.8/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_276
	GLEW_158
	ANTTWEAKBAR_151_OGLCORE_GLFW
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)


# Tutorial 4
add_executable(p6
	implementacje.cpp
        main.cpp
	common/shader.cpp
	common/shader.hpp
)
target_link_libraries(p6
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(p6 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
create_target_launcher(p6 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")





cmake_minimum_required(VERSION 3.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE STRING "VSCode Intellisense" FORCE)
set(CMAKE_C_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/amd64/cl.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/amd64/cl.exe")

set(target_name test_fr_comia)
project(${target_name} LANGUAGES C CXX)

if(MSVC)
	set(CMAKE_CXX_FLAGS "/FS /EHsc /FC") # No pdb concurrency, no warnings, full paths for $msCompile problemMatcher.
endif()

set (CMAKE_CXX_STANDARD 11)

set(SOURCE
			test_fr_comia.cpp
	)

#set(HEADERS
#			test_fr_comia.h
#	)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
	message("-- Opencv Found")
endif()

add_executable(${target_name} ${SOURCE}) # ${HEADERS}
# include(C:/dlib-19.8/dlib/cmake)
set(DLIB_RELEASE "C:/dlib-19.8/release_build/install/lib/cmake/dlib")
set(DLIB_DEBUG "C:/dlib-19.8/debug_build/install/lib/cmake/dlib")
find_package(dlib 
	HINTS "${DLIB_DEBUG}"
	NO_PACKAGE_ROOT_PATH
	NO_CMAKE_PATH
	NO_CMAKE_ENVIRONMENT_PATH
	REQUIRED
) # Look for the precompiled installation instead.

set_target_properties(dlib::dlib PROPERTIES
	INTERFACE_COMPILE_OPTIONS ""
)
target_link_libraries(${target_name} ${OpenCV_LIBS} dlib::dlib)
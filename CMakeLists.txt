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

add_executable(${target_name} ${SOURCE}) # ${HEADERS}
# TARGET_LINK_LIBRARIES(${target_name} namepace::library)
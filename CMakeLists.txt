###################
## Project Setup ##
###################
cmake_minimum_required(VERSION 3.23)
set(PROJECT_NAME "librutils")
project(${PROJECT_NAME} LANGUAGES CXX VERSION 1.0.0)

#stop cmake from prepending
set(CMAKE_SHARED_LIBRARY_PREFIX "") 

#build type option
set(LIBRARY_BUILD_TYPE "SHARED" CACHE STRING "STATIC/SHARED")

#################
## VCPKG SETUP ##
#################
if (WIN32)	
	# If we are on a Windows based system, we should use VCPKG!
	option(VCPKG_PATH "Path to root folder of VCPKG")

	#check if a custom vpckg path was provided
	if (${VCPKG_PATH} STREQUAL OFF)
		set(VCPKG_PATH "C:/Program Files (x86)/vcpkg") #if not, set a default path
	endif()

	#include toolchain file
	include("${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")

	#Apply correct include directory according to 
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		#64 bit
		set(ARCHITECTURE "x64")
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
		# 32 bit
		set(ARCHITECTURE "x86")
	endif()

	set(CMAKE_INSTALL_PREFIX ${VCPKG_PATH}/installed/${ARCHITECTURE}-windows/)
endif()

option(RUTILS_BUILD_CORE "Build the CORE library module" ON)
option(RUTILS_BUILD_DPP "Build the DPP library module" OFF)

message("Build librutils-core: ${RUTILS_BUILD_CORE}")
message("Build librutils-dpp: ${RUTILS_BUILD_DPP}")

if (RUTILS_BUILD_CORE STREQUAL ON OR RUTILS_BUILD_CORE STREQUAL True)
	add_subdirectory(librutils-core)
endif()

if (RUTILS_BUILD_DPP STREQUAL ON OR RUTILS_BUILD_DPP STREQUAL True)
	add_subdirectory(librutils-dpp)
endif()
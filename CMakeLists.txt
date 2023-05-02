###################
## Project Setup ##
###################
cmake_minimum_required(VERSION 3.23)
set(PROJECT_NAME "librutils")

option(RUTILS_BUILD_DPP "Build the DPP library module" OFF)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#set include directory for library
set(${PROJECT_NAME}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

add_subdirectory(rutils-core)
target_link_libraries(${PROJECT_NAME} INTERFACE rutils-core)


if (RUTILS_BUILD_DPP STREQUAL ON)
	add_subdirectory(rutils-dpp)
	target_link_libraries(${PROJECT_NAME} INTERFACE rutils-dpp)
endif()
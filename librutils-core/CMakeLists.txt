cmake_minimum_required(VERSION 3.23)

# Set PROJECT_NAME var, which should be used in any subsequent variables that need the project name in the variable name
# note variables will use globals unless there is a more local version of the variable (which is now the PROJECT_NAME)
set(PROJECT_NAME "librutils-core")
project(${PROJECT_NAME} LANGUAGES CXX VERSION 1.0.0)

###########################
## Create Library Target ##
###########################
add_library(${PROJECT_NAME} ${LIBRARY_BUILD_TYPE})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
message("Library build type: ${LIBRARY_BUILD_TYPE}")


###################
## Project Files ##
###################
target_sources(${PROJECT_NAME}
    PRIVATE 
	"src/base64.cpp"
	"src/callback_timer.cpp"
	"src/delta_timer.cpp"
	"src/string_utils.cpp"
    PUBLIC FILE_SET HEADERS 
	BASE_DIRS "include"
    FILES 
	"include/rutils-core/base64.h"
	"include/rutils-core/bitmask_operators.h"
	"include/rutils-core/callback_timer.h"
	"include/rutils-core/delta_timer.h"
	"include/rutils-core/service.h"
	"include/rutils-core/service_locator.h"
	"include/rutils-core/standard_colors.h"
	"include/rutils-core/string_utils.h"
)

# Add project include dirs
target_include_directories(${PROJECT_NAME} PRIVATE 
	"src"
	"include/librutils-core"
	$<$<NOT:$<STREQUAL:${VCPKG_PATH}, OFF>>:${VCPKG_PATH}/installed/${ARCHITECTURE}-windows/include>
)

##########
## Libs ##
##########
# Add libloggeru
find_package(libloggeru CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC libloggeru::libloggeru)

# TODO: deprecate and replace with STD fmt
#add FMT
find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)


####################
## Compiler Flags ##
####################
target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /EHsc /std:c++20>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror -std=c++20>
)

##################
## Installation ##
##################

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
	"${PROJECT_NAME}-config-version.cmake"
	VERSION ${${PROJECT_NAME}_VERSION}
	COMPATIBILITY AnyNewerVersion
)

if (WIN32)
	#if VCPKG not found, don't continue
	if (NOT EXISTS ${VCPKG_PATH})
		message(FATAL_ERROR "Failed to find VCPKG location to install to, CMake will exit." )
	endif()

	#set the LIB_DIR_PREFIX to debug if we are building for debug, (release is no prefix)
	SET(${PROJECT_NAME}_LIB_DIR_PREFIX "")
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		SET(${PROJECT_NAME}_LIB_DIR_PREFIX "debug/")
	endif()
	
	# install the library files in the correct destinations
	install(TARGETS ${PROJECT_NAME} 
		EXPORT ${PROJECT_NAME}-targets
			LIBRARY DESTINATION ${${PROJECT_NAME}_LIB_DIR_PREFIX}
			ARCHIVE DESTINATION ${${PROJECT_NAME}_LIB_DIR_PREFIX}lib
			RUNTIME DESTINATION ${${PROJECT_NAME}_LIB_DIR_PREFIX}bin
			FILE_SET HEADERS DESTINATION include
	)

	# create the package config file and install it in the correct location
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-vcpkg.pc.in" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc" @ONLY)
	install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION "lib/pkgconfig")

	# create the targets.cmake file and place it in the correct dir
	install(EXPORT ${PROJECT_NAME}-targets
		FILE ${PROJECT_NAME}-targets.cmake
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION "share/${PROJECT_NAME}")

	install(FILES "cmake/${PROJECT_NAME}-config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
		DESTINATION "share/${PROJECT_NAME}"
	)

else()
	include(GNUInstallDirs)

	install(TARGETS ${PROJECT_NAME} 
		EXPORT ${PROJECT_NAME}-targets
			LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
			ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
			FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc" @ONLY)
	install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

	install(EXPORT ${PROJECT_NAME}-targets
		FILE ${PROJECT_NAME}-targets.cmake
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

	install(FILES "cmake/${PROJECT_NAME}-config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
	)
endif()
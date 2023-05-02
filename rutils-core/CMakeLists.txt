cmake_minimum_required(VERSION 3.23)

# Set PROJECT_NAME var, which should be used in any subsequent variables that need the project name in the variable name
# note variables will use globals unless there is a more local version of the variable (which is now the PROJECT_NAME)
set(PROJECT_NAME "rutils-core")

add_library(${PROJECT_NAME}
	"base64.cpp"
	"bitmask_operators.h"
	"callback_timer.cpp"
	"delta_timer.cpp"
	"service.h"
	"service_locator.h"
	"standard_colors.h"
	"string.cpp"
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

##########
## Libs ##
##########

#since we are requiring globally installed packages, add VCPKG_INCLUDE_DIR to our target include directories
if(WIN32)
	#since we are requiring globally installed packages, add VCPKG_INCLUDE_DIR to our target include directories
	target_include_directories(${PROJECT_NAME} PUBLIC ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
endif()

# Add libloggeru
find_package(libloggeru CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC libloggeru::libloggeru)

#add FMT
find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
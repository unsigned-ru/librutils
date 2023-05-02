cmake_minimum_required(VERSION 3.23)


# Set PROJECT_NAME var, which should be used in any subsequent variables that need the project name in the variable name
# note variables will use globals unless there is a more local version of the variable (which is now the PROJECT_NAME)
set(PROJECT_NAME "rutils-dpp")


add_library(${PROJECT_NAME}
	"utils.cpp"
	"base_bot.cpp"
	
	"slashcommands.h"
	"slashcommand_handler.cpp"
	"slashcommand_groups.cpp"

	"preconditions/precondition.cpp"
	"preconditions/all_of.cpp"
	"preconditions/any_of.cpp"
	"preconditions/has_channel.cpp"
	"preconditions/has_permissions.cpp"
	"preconditions/has_role.cpp"
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


find_package(dpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC dpp::dpp)
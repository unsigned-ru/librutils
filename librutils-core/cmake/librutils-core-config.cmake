include(CMakeFindDependencyMacro)
include(${CMAKE_CURRENT_LIST_DIR}/librutils-core-targets.cmake)

find_dependency(fmt REQUIRED)
find_dependency(libloggeru REQUIRED)
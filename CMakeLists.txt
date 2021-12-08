# CMakeList.txt : CMake project for TSP, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(TSP VERSION 0.0.1)

include(cmake/CPM.cmake)

# packages
option(CPM_USE_LOCAL_PACKAGES "Try `find_package` before downloading dependencies" ON)
CPMAddPackage(
    NAME matplotplusplus
    GITHUB_REPOSITORY alandefreitas/matplotplusplus
    GIT_TAG origin/master
)

#----------------------------------------------------------------------------
# Setup include directories and compile definitions
#
set (CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-std=c++17") # use -w to turn off warnings
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB_RECURSE CONFIGURE_DEPENDS sources src/*.cpp)
file(GLOB_RECURSE CONFIGURE_DEPENDS headers include/*.h)
#----------------------------------------------------------------------------
# Add source to this project's executable.
add_executable (Tests ${PROJECT_SOURCE_DIR}/Tests.cxx ${sources} src/Address.cpp ${headers} "src/AddressList.cpp" "src/Route.cpp" "src/Graph.cpp" "src/Path.cpp" "src/MultiplePath.cpp" "src/Way.cpp" "src/System.cpp")
# add_executable (TSP ${PROJECT_SOURCE_DIR}/TSP.cxx ${sources} src/Address.cpp ${headers} "src/AddressList.cpp" "src/Route.cpp" "src/Graph.cpp" "src/Path.cpp" "src/MultiplePath.cpp" "src/Way.cpp")
target_link_libraries(Tests PUBLIC matplot)
# target_link_libraries(TSP PUBLIC matplot)
# TODO: Add tests and install targets if needed.

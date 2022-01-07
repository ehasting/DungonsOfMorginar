cmake_minimum_required (VERSION 2.9)
project (DungonsOfMorginar)
#set (CMAKE_CXX_COMPILER "clang++")
#set (CMAKE_C_COMPILER "clang")
message(STATUS "The compiler ${CMAKE_CXX_COMPILER_ID}")

set (CMAKE_CXX_STANDARD 20)
find_package( Threads )
add_subdirectory(fmt-8.1.0 EXCLUDE_FROM_ALL)
include_directories ("${PROJECT_SOURCE_DIR}/src")
set(HEADER_FILES
        src/tools.hpp src/tools.cpp
        src/coremodels/location.cpp src/coremodels/location.hpp
        src/coremodels/dynamicobject.hpp src/coremodels/dynamicobject.cpp
        src/objects/character.hpp src/objects/character.cpp
        src/objects/mouse.hpp src/objects/mouse.cpp
        src/coremodels/locatedobject.cpp src/coremodels/locatedobject.hpp
        src/gameloop.hpp src/gameloop.cpp
        src/map/tile.cpp src/map/tile.hpp
        src/map/mapregions.cpp src/map/mapregions.hpp)



add_executable(DungonsOfMorginarV2 src/main.cpp ${HEADER_FILES})
target_link_libraries( DungonsOfMorginarV2 ${CMAKE_THREAD_LIBS_INIT} fmt::fmt-header-only)
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
        src/models/location.cpp src/models/location.hpp
        src/models/dynamicobject.hpp src/models/dynamicobject.cpp
        src/models/character.hpp src/models/character.cpp
        src/models/mouse.hpp src/models/mouse.cpp
        src/models/locatedobject.cpp src/models/locatedobject.hpp
        src/gameloop.hpp src/gameloop.cpp
        )



add_executable(DungonsOfMorginarV2 src/main.cpp ${HEADER_FILES})
target_link_libraries( DungonsOfMorginarV2 ${CMAKE_THREAD_LIBS_INIT} fmt::fmt-header-only)
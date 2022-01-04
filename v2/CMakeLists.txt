cmake_minimum_required (VERSION 2.6)
project (DungonsOfMorginar)
#set (CMAKE_CXX_COMPILER "clang++")
#set (CMAKE_C_COMPILER "clang")
message(STATUS "The compiler ${CMAKE_CXX_COMPILER_ID}")

set (CMAKE_CXX_STANDARD 17)
find_package( Threads )
include_directories ("${PROJECT_SOURCE_DIR}/src")
set(HEADER_FILES
        src/tools.hpp src/tools.cpp
        src/gameloop.hpp src/gameloop.cpp
        src/models/character.hpp src/models/character.cpp
)

add_executable(DungonsOfMorginarV2 src/main.cpp ${HEADER_FILES})
target_link_libraries( DungonsOfMorginarV2 ${CMAKE_THREAD_LIBS_INIT} )
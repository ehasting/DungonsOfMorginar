cmake_minimum_required (VERSION 2.9)
project (DungonsOfMorginar)
#set (CMAKE_CXX_COMPILER "clang++")
#set (CMAKE_C_COMPILER "clang")
message("-- The compiler ${CMAKE_CXX_COMPILER_ID}")
message("-- CMAKE_HOST_SYSTEM_VERSION ${CMAKE_HOST_SYSTEM_VERSION}")
message("-- CMAKE_SYSTEM_NAME:      ${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")

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
        src/map/mapregions.cpp src/map/mapregions.hpp
        src/core/nonblockingterminal.cpp src/core/nonblockingterminal.hpp
        src/core/keycodes.hpp
        src/core/screenpos.cpp src/core/screenpos.hpp
        src/coremodels/livingobject.cpp src/coremodels/livingobject.hpp
        src/core/livingstats.cpp src/core/livingstats.hpp src/sharedinputhandler.hpp src/core/inputhandler.hpp src/core/iterminal.hpp)


message("--- Adding ${CMAKE_SYSTEM_NAME} related headers")
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(PLATFORM_HEADER_FILES
            src/platform/linuxterminal.cpp src/platform/linuxterminal.hpp)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(PLATFORM_HEADER_FILES
            src/platform/windowsterminal.cpp src/platform/windowsterminal.hpp)
endif()

add_executable(DungonsOfMorginarV2 src/main.cpp ${HEADER_FILES} ${PLATFORM_HEADER_FILES})
target_link_libraries( DungonsOfMorginarV2 ${CMAKE_THREAD_LIBS_INIT} fmt::fmt-header-only)
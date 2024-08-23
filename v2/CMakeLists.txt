cmake_minimum_required (VERSION 3.5 )
project (DungonsOfMorginar)
#set (CMAKE_CXX_COMPILER "clang++")
#set (CMAKE_C_COMPILER "clang")
message("-- The compiler               ${CMAKE_CXX_COMPILER_ID}")
message("-- CMAKE_HOST_SYSTEM_VERSION: ${CMAKE_HOST_SYSTEM_VERSION}")
message("-- CMAKE_SYSTEM_NAME:         ${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_PROCESSOR:    ${CMAKE_SYSTEM_PROCESSOR}")
message("-- CMAKE_SYSTEM:              ${CMAKE_SYSTEM}")

set (CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

message("-- SDL2_LIBRARIES:            ${SDL2_LIBRARIES}")
message("-- SDL2_INCLUDE_DIRS:         ${SDL2_INCLUDE_DIRS}")

include_directories(DungonsOfMorginar  ${SDL2_INCLUDE_DIRS})


find_package(Threads REQUIRED)
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
        src/core/livingstats.cpp src/core/livingstats.hpp
        src/core/inputhandler.hpp src/core/iterminal.hpp
        src/actionparser.cpp src/actionparser.hpp)

message("-- Adding ${CMAKE_SYSTEM_NAME} related headers")

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(PLATFORM_HEADER_FILES
            src/platform/linuxterminal.cpp src/platform/linuxterminal.hpp)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(PLATFORM_HEADER_FILES
            src/platform/windowsterminal.cpp src/platform/windowsterminal.hpp
            src/core/DofM_TextRenderCache.cpp
            src/core/DofM_TextRenderCache.h)
endif()

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)


add_executable(DungonsOfMorginarV2 src/main.cpp ${HEADER_FILES} ${PLATFORM_HEADER_FILES})
target_link_libraries(DungonsOfMorginarV2 ${CMAKE_THREAD_LIBS_INIT} fmt::fmt-header-only)
target_link_libraries(DungonsOfMorginarV2 ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)
target_include_directories(DungonsOfMorginarV2 PRIVATE resources)
add_dependencies(DungonsOfMorginarV2 copy_assets)

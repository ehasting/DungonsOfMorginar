cmake_minimum_required (VERSION 3.1)
project (DungonsOfMorginar)
set (CMAKE_CXX_STANDARD 11)

include_directories ("${PROJECT_SOURCE_DIR}/src")
set(HEADER_FILES
    src/character.cpp
    src/location.cpp src/locations.cpp
    src/action.cpp src/promptparser.cpp src/tools.cpp src/item.cpp src/thegame.cpp
    src/renderengine.cpp
    src/asciirenderengine.cpp)

add_executable(DungonsOfMorginar src/launcher.cpp ${HEADER_FILES})

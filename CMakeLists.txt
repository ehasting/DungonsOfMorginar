cmake_minimum_required (VERSION 2.6)
project (DungonsOfMorginar)
set (CMAKE_CXX_COMPILER "clang++")
set (CMAKE_C_COMPILER "clang")
message(STATUS "The compiler ${CMAKE_CXX_COMPILER_ID}")

macro(use_cxx11)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
    elif (CMAKE_CXX_COMPILER_ID STREQUAL "CLANG")
      set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 11)
  endif ()
endmacro(use_cxx11)

use_cxx11()

include_directories ("${PROJECT_SOURCE_DIR}/src")
set(HEADER_FILES
    src/tools.cpp
    src/character.cpp
    src/hero.cpp
    src/location.cpp
    src/locations.cpp
    src/action.cpp
    src/promptparser.cpp
    src/item.cpp
    src/thegame.cpp
    src/renderengine.cpp
    src/asciirenderengine.cpp
    src/mazegenerator.cpp
    src/monster.cpp
    src/monsters.cpp
    src/fightcalculator.cpp)

add_executable(DungonsOfMorginar src/launcher.cpp ${HEADER_FILES})

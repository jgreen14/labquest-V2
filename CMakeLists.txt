cmake_minimum_required(VERSION 3.10)

# set the project name
project(labquest VERSION 0.3)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBTCOD_INCLUDE_DIR "${LIBTCOD_INCLUDE_DIR}" CACHE PATH "Location of libTCOD include directory")
set(LIBTCOD_LIB "${LIBTCOD_LIB}" CACHE PATH "Location of libTCOD library")

if (WIN32)
    # add main executable
    add_executable(labquest labquest.cpp "node.cpp" "list.cpp" "coord.cpp" "items.cpp" "tiles.cpp" "room.cpp" "level.cpp" "agent.cpp" "player.cpp" "creature.cpp" "io.cpp")

    target_include_directories(labquest PUBLIC
                               "${PROJECT_BINARY_DIR}"
                               "${LIBTCOD_INCLUDE_DIR}"
                               )
    target_link_libraries(labquest "${LIBTCOD_LIB}")
endif()

# add test executable
add_executable(labquest_test labquest_test.cpp "node.cpp" "list.cpp" "coord.cpp" "items.cpp" "tiles.cpp" "room.cpp" "level.cpp" "agent.cpp" "player.cpp" "creature.cpp")

target_include_directories(labquest_test PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
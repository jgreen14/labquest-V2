cmake_minimum_required(VERSION 3.10)

# set the project name
project(labquest VERSION 0.3)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(labquest labquest.cpp "node.cpp" "list.cpp" "coord.cpp" "items.cpp" "tiles.cpp" "room.cpp" "level.cpp" "agent.cpp" "player.cpp" "creature.cpp")

target_include_directories(labquest PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
project(prog_np)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
set(CMAKE_C_FLAGS "-std=gnu11 -g -Wall")

add_executable(${PROJECT_NAME} ${SRC_LIST})

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})

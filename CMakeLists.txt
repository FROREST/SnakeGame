cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 14)

# Поиск SFML
find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)

add_executable(SnakeGame main.cpp)

target_include_directories(SnakeGame PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(SnakeGame PRIVATE sfml-graphics)

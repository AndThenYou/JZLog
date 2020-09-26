cmake_minimum_required(VERSION 2.8.12)

project(JZLog)


add_subdirectory(src/JZLog)
add_subdirectory(demo)

add_dependencies(demo JZLog)
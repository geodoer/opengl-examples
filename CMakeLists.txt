cmake_minimum_required(VERSION 3.22)
project(opengl-examples)

include(${CMAKE_SOURCE_DIR}/cmake/project.cmake)

#
# 工程配置
#
include_directories(${CMAKE_SOURCE_DIR}/include)

#
# 依赖库
#
include(${CMAKE_SOURCE_DIR}/Dependencies/dependencies.cmake)

#
# 子工程
#
add_subdirectory(${CMAKE_SOURCE_DIR}/submodules)

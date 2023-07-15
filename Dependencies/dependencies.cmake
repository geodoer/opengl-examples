message(STATUS "Find Dependencies")

# opengl库
find_package(OpenGL REQUIRED)

# 使用FetchContent引入glfw
include(${CMAKE_CURRENT_LIST_DIR}/glfw.cmake)

# 自定义glew的Find<PackageName>.cmake文件，引入glew库（使用IMPORTED引入glew的预编译版本）
#方法一：直接引入文件
include(${CMAKE_CURRENT_LIST_DIR}/glew-2.2.0/Findglew.cmake)
#方法二：使用find_package module模式，查找Findglew.cmake文件
# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/Dependencies/glew-2.2.0)
# find_package(glew REQUIRED)

# 使用第三方库自带的CMakelists.txt
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glm)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/imgui)
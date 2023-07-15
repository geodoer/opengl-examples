set(TARGET_NAME 15TestProject)
project(${TARGET_NAME})

file(GLOB_RECURSE SrcFiles "${CMAKE_CURRENT_LIST_DIR}/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/*.h")

add_executable(${TARGET_NAME} 
    ${SrcFiles}
)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${TARGET_NAME}
PRIVATE
    OpenGL::GL glfw glew::glew glm imgui
)

Copy_Resource()
SET_TARGET_FOLDER()
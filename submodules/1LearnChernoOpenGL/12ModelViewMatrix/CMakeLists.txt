set(TARGET_NAME 12ModelViewMatrix)
project(${TARGET_NAME})

file(GLOB_RECURSE SrcFiles "${CMAKE_CURRENT_LIST_DIR}/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/*.h")

add_executable(${TARGET_NAME} 
    ${SrcFiles}
)

target_link_libraries(${TARGET_NAME}
PRIVATE
    OpenGL::GL glfw glew::glew glm
)

Copy_Resource()
SET_TARGET_FOLDER()
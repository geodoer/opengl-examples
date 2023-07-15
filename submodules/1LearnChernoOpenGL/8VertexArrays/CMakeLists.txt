set(TARGET_NAME 8VertexArrays)
project(${TARGET_NAME})

add_executable(${TARGET_NAME} 
    ${CMAKE_CURRENT_LIST_DIR}/Application.cpp
)

target_link_libraries(${TARGET_NAME}
PRIVATE
    OpenGL::GL glfw glew::glew
)

Copy_Resource()
SET_TARGET_FOLDER()
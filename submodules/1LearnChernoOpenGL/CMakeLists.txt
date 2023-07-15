# 模块名
set(MODULE_DIR "1LearnChernoOpenGL")

#将res目录拷贝到生成目录
macro(Copy_Resource)
    add_custom_command(
        TARGET ${TARGET_NAME}  POST_BUILD           #此Target的生成后事件（编译成功后触发执行）
        COMMAND ${CMAKE_COMMAND} -E copy_directory  #拷贝目录的命令
            ${CMAKE_CURRENT_SOURCE_DIR}/res         #需要拷贝的目录 
            ${PROJECT_BINARY_DIR}/res               #目标目录（使用的是工程二进制目录，因此可执行的Target，需要定义工程名才能正常使用）
    )
endmacro()

ADD_ALL_SUBDIR()
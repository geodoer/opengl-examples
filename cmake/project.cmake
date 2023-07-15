# 工程相关设置

#
# 工程分组（由Target中回调）
#
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
macro(SET_TARGET_FOLDER)
    #指定此Target的目录，在VS中才能分组
    set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER ${MODULE_DIR})
endmacro()

#
# 添加目录下的所有子文件夹
#
macro(ADD_ALL_SUBDIR)
    # 获取当前目录下的所有子文件夹
    file(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

    # 遍历子文件夹
    foreach(subdirectory ${subdirectories})
        if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory})
            # 调用add_subdirectory添加子目录
            message(STATUS "[add_subdirectory] ${subdirectory}")
            add_subdirectory(${subdirectory})
        endif()
    endforeach()
endmacro()
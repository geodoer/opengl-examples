if(TARGET glfw)
    return() #如果已经存在，则退出
endif()

# 引入FetchContent相关函数
include(FetchContent)

# 通过FetchContent在网上下载源代码
FetchContent_Declare(
    glfw                                        #TARGET名称
    GIT_REPOSITORY https://github.com/glfw/glfw #网址
    GIT_TAG 3.3.8                               #tag
)

# 确保下载成功，并添加到cmake
FetchContent_MakeAvailable(glfw)
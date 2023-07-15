/*
 * 虽然stb_image是一个头文件库
 * 但加了这个cpp文件，就会让代码只编译一次，也就是编译成stb_image.obj文件
 * 如果stb_image被其他.cpp用到，就不会被编译了，只会被链接
 */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
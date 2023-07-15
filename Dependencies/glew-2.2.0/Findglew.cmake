if(TARGET glew::glew)
    return() #如果已经存在，则退出
endif()

#显示在GUI上
set(GLEW_verbose          OFF CACHE BOOL "output information about glew"  FORCE)
set(GLEW_USE_STATIC_LIBS  ON  CACHE BOOL "use the glew static library"    FORCE) 

set(GLEW_FOUND           TRUE)
set(GLEW_VERSION        2.2.0)
set(GLEW_VERSION_MAJOR      2)
set(GLEW_VERSION_MINOR      2)
set(GLEW_VERSION_MICRO      0)

set(GLEW_INCLUDE_DIR  ${CMAKE_CURRENT_LIST_DIR}/include)
set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
if(GLEW_verbose)
  message(STATUS ${GLEW_INCLUDE_DIRS})
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(_arch "x64")
else()
  set(_arch "Win32")
endif()
if(GLEW_verbose)
  message(STATUS ${_arch})
endif()

#此预编译版本只有Relase
find_library(GLEW_SHARED_LIBRARY_RELEASE
             NAMES GLEW glew glew32
             PATHS ${CMAKE_CURRENT_LIST_DIR}/lib/Release/${_arch})
find_library(GLEW_STATIC_LIBRARY_RELEASE
            NAMES GLEW glew glew32s
            PATHS ${CMAKE_CURRENT_LIST_DIR}/lib/Release/${_arch})

if(GLEW_USE_STATIC_LIBS)
  set(GLEW_LIBRARY ${GLEW_STATIC_LIBRARY_RELEASE})
else()
  set(GLEW_LIBRARY ${GLEW_SHARED_LIBRARY_RELEASE})
endif()
set(GLEW_LIBRARIES ${GLEW_LIBRARY})
if(GLEW_verbose)
  message(STATUS ${GLEW_LIBRARY})
endif()

# 创建Target，方便外部链接
add_library(glew::glew UNKNOWN IMPORTED)
set_property(
	TARGET glew::glew 
	PROPERTY 
		IMPORTED_LOCATION ${GLEW_LIBRARY}
) 
target_include_directories(
	glew::glew 
	INTERFACE ${GLEW_INCLUDE_DIR}
)
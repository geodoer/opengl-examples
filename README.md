# OpenGL例子集

## 目录结构

```text
.
├── docs                文档
├── CMakeLists.txt      工程
├── Dependencies        依赖库
└── submodules          课程代码
	├── 1LearnChernoOpenGL  Cherno的LearnOpenGL课程代码
    └── ...                 
```

## 构建

### Windows

```
cmake -B ./build
```

## 依赖库

### GLEW
在本工程中，GLEW采取静态链接（相比动态链接，它不需要拷贝dll，更方便些），因此需要在`#include <GL/glew.h>`之前要加上`#define GLEW_STATIC`。

```cpp
#define GLEW_STATIC
#include <GL/glew.h>
```
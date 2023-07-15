## 传统OpenGL
在"Hi, OpenGL"中，我们使用传统的OpenGL绘制了一个三角形，传统的OpenGL使用`glBegin()`和`glEnd()`即可完成图形的绘制。

```cpp
glBegin(GL_TRIANGLES);
glVertex2f(-0.5f, -0.5f);
glVertex2f( 0.0f,  0.5f);
glVertex2f( 0.5f, -0.5f);
glEnd();
```

## 现代OpenGL
现代OpenGL绘制的流程：

1. 定义一些数据来表示三角形，然后把它们放到显存中
2. 然后发出DrawCall指令。这是一个绘制指令，也就是说：Hey显卡，你的显存中有一堆数据，读取它，并且把它绘制在屏幕上
3. 实际上，我们还需告诉显卡如何读取和解释这些数据，以及如何把它放到我们的屏幕上
4. 着色器负责如何解释这些数据进行绘制

OpenGL具体操作是一个状态机

- 这意味着你无需把它们看成对象或类似的东西，你做的一切都是在设置一系列的状态

### 顶点缓冲区
缓冲区：一块用来存储字节的内存。

OpenGL中的内存缓冲区

- 这块内存实际上是在显卡中的，在VRAM（显存，Video RAM）当中

顶点缓冲区：存储顶点数据的缓冲区

顶点≠位置

- 顶点与位置无关
- 顶点是几何图形上的一个点，它具有很多个属性：位置、纹理坐标、法线、颜色等等，而位置只是其中一个属性
- 如果只有位置属性，那可以说是顶点位置，而不能称为顶点

#### glGenBuffers
在GPU中生成buffer，并获得这些Buffer的ID

1. OpenGL一次可以生成一堆缓冲区，因此在第一个参数当中，指定你需要生成几个缓冲区
2. 这个函数不返回生成的缓冲区id，它是通过第二个参数返回的，因此我们需要提供一个整数来接收缓冲区id
	1. 但因为一次可以生成多个，所以是传入一个无符号整形指针。需适配一堆缓冲区的情况，此时就需要传入一个数组
	2. 基本上你会得到一个数字，比如1、2、3等等，那是你申请实际对象的ID。无论是顶点缓冲区，还是顶点数组、纹理、着色器或者任何其他东西，OpenGL返回给你的都是一个ID

```cpp
unsigned int buffer;
glGenBuffers(1, &buffer);
```

#### glBindBuffer
你可能在GPU中生成了很多buffer，但OpenGL一次操作的buffer个数有限，因此我们要选择buffer。

在OpenGL中选择(selecting)被称之为绑定(binding)

1. 第一个参数是buffer的类型。顶点缓冲区对应的是GL_ARRAY_BUFFER
2. 第二个参数是buffer的ID

```cpp
glBindBuffer(GL_ARRAY_BUFFER, buffer); //这意味着，我需要使用buffer这个缓冲区
```

#### glBufferData
把数据放到缓冲区，有两种方式

1. 不需要马上提供数据，之后在更新的时候再给
2. 在创建缓冲区时，直接给数据

这里是第二种方式，在创建缓冲区、绑定缓冲区之后直接给数据

```cpp
float positions[2 * 3] = {
	-0.5f, -0.5f,
	 0.0f,  0.5f,
	 0.5f, -0.5f
};
glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
```

参数：绘制模式

1. `STREAM`  被修改一次，只能使用几次。
2. `STATIC` 修改一次、使用多次。不会每帧修改buffer，但每帧都用这一个buffer绘制
3. `DYNAMIC` 反复修改、多次使用。每帧修改这些顶点，并且每帧绘制

但这只是提示，如果你申明为`STATIC`，然后每帧还是会更新缓冲区。虽然这违反了提示(hint)，但也能运行，只是速度比较慢。

#### glVertexAttribPointer
上一步之后，数据已经拷贝到GPU当中了。但这些数据只是一些字节，OpenGL还不知道如何正确解读这些字节。因此，我们还需要告诉OpenGL，“内存中有什么？它们又是如何布局的？”。

```cpp
//启用顶点属性数组，索引0，的位置
glEnableVertexAttribArray(0);
//告诉GPU，当前缓冲区的内存布局，即positions的内存布局
//当前，positions中只有位置属性
glVertexAttribPointer(0,  //指定第一个顶点属性的布局
					 2,   //顶点位置是二维的，有两个分量，size=2即可
					 GL_FLOAT,
					 GL_FALSE,
					 2 * sizeof(float), //一个顶点的大小
					 0 //positions中只包含了一个顶点属性，因此就不需要位移量
					 );
```

函数声明：
```cpp
void glVertexAttribPointer(
	GLuint index,
	GLint size,
	GLenum type,
	GLboolean normalized,
	GLsizei stride,
	const GLvoid * pointer);

void glVertexAttribIPointer(
	GLuint index,
	GLint size,
	GLenum type,
	GLsizei stride,
	const GLvoid * pointer);

void glVertexAttribLPointer(
	GLuint index,
	GLint size,
	GLenum type,
	GLsizei stride,
	const GLvoid * pointer);
```

参数：index

- 它是缓冲区中实际属性的索引
- 例如，你可能会指定位置属性的index=0，纹理坐标的index=1，法线的index=2

参数：size

- 顶点属性的分量数，只能是1、2、3、4
- 例如，当前的例子，顶点位置是二维的，有两个分量，size=2即可

参数：type

- 数据类型，如`GL_FLOAT`

参数：normalized

- 一般是不需要的，置为false
- 例如颜色，值在0-255之间，它需要被规范化到0到1之间，此时就可以使用normalized这个参数，将它置为true

参数：stride

- stride就是每个顶点之间的字节数
- 例如，此时顶点具有位置、纹理坐标、法线三个属性。位置是3个float、纹理坐标是2个float、法线是3个float，因此`stride=8*sizeof(float)=32个字节`
- 0是第一个顶点；`0+stride`是第二个顶点；`0+2*stride`是第一个顶点

参数：pointer

- pointer是指向实际属性的指针
- 例如，此时顶点具有位置、纹理坐标、法线三个属性。对于顶点`pointer=0`；对于纹理坐标`pointer=3*4`；对于法线`pointer=3*4+2*4`


#### DrawCall
绘制命令（DrawCall）

1. 没有索引缓冲区时，可以使用`glDrawArray()`函数
2. 如果有索引缓冲区时，可以使用`glDrawElements()`函数

```cpp
int main() {
	//...
	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		//缓冲区offset=0的地方开始
		//绘制3个顶点
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//...
	}

	//...
}
```
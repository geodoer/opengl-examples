#### uniform变量
上一节中，在片元着色中，我们硬编码输入了一个颜色，指定为了红色。

这一节，我们将使用uniform变量，从CPU中传一个颜色到片元着色器，使用这个颜色作为片元的最终颜色。

uniform

- CPU传数据到显卡的一种方式

1.在shader中添加代码
```glsl
//...

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color; //记住u_Color这个名字，在通过OpenGL传值时，需要用到它

void main()
{
	color = u_Color;
}
```

2.在C++代码中传值
```cpp
//...

int main()
{
	//...
	
    /**
     * 交换间隔，交换缓冲区之前等待的帧数，通常称为v-sync
     * 默认情况下，交换间隔为0
     * 这里设置为1，即每帧更新一次
     **/
    glfwSwapInterval(1);

	//...

	int location = glGetUniformLocation(shader, "u_Color"); //获取变量的句柄
	ASSERT(location != -1); //-1意味着我们没有找到指定的uniform变量

	float r = 0.0f;
	float increment = 0.05f
	while(...)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform4f(location, r, 0.3f, 0.8f, 1.0f); //传值
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if(r>1.0f)      increment = -0.05f;
		else if(r<0.0f) increment =  0.05f;

		r += increment;
		
		//...
	}

	//...
	return 0;
}
```
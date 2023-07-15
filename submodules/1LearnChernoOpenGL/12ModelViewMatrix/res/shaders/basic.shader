#shader vertex
#version 330 core

//顶点数据
layout(location = 0)in vec4 position;	//顶点位置（注意，这里是vec4）
layout(location = 1)in vec2 texCoord;	//UV坐标

uniform mat4 u_MVP; //模型视图矩阵，目前只传入了一个投影矩阵

out vec2 v_TexCoord; //输出变量。顶点着色器的输出，将会作为片元着色器的输入

void main()
{
	gl_Position = u_MVP * position; //将坐标做一个投影变换
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0)out vec4 color;

in vec2 v_TexCoord;	//由顶点着色器传入

uniform sampler2D u_Texture;
	//sampler2D为二维的纹理图
	//OpenGL只需设置纹理槽的Id即可，sampler2D即会取到对应的纹理

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);	//对指定纹理坐标进行采样
	color = texColor;
};
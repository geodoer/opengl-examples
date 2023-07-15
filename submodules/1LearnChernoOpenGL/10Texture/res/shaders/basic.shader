#shader vertex
#version 330 core

//顶点数据
layout(location = 0)in vec4 position;	//顶点位置（注意，这里是vec4）
layout(location = 1)in vec2 texCoord;	//UV坐标

out vec2 v_TexCoord; //输出变量。顶点着色器的输出，将会作为片元着色器的输入

void main()
{
	gl_Position = position;
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
#version 450

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 color;
layout (location = 0) out vec4 outColor;

void main()
{
	// 頂点位置
	gl_Position = pos;
	outColor = color;
}

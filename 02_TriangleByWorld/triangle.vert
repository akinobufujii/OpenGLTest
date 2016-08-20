#version 450

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 color;
layout (location = 0) out vec4 outColor;

// ユニフォームバッファ
layout (std140, binding = 0) uniform MATRICES
{
	mat4 g_projection;	// 投影行列
	mat4 g_view;		// 視線行列
	mat4 g_world;		// ワールド行列
};

void main()
{
	outColor = color;
	gl_Position = g_projection * g_view * g_world * pos;
}

#version 450

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec4 inColor;
layout (location = 0) out vec4 outColor;

// ユニフォームバッファ
layout (binding = 0) uniform MATRICES
{
	mat4 g_projection;	// 投影行列
	mat4 g_view;		// 視線行列
	mat4 g_world;		// ワールド行列
};

void main()
{
	outColor = vec4(inNormal, 1.0);
	gl_Position = g_projection * g_view * g_world * inPos;
}

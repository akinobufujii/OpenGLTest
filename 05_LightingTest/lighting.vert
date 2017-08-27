#version 450

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec4 inColor;
layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outUV;

// ユニフォームバッファ
layout (binding = 0) uniform VALUES
{
	mat4 g_projection;	// 投影行列
	mat4 g_view;		// 視線行列
	mat4 g_world;		// ワールド行列
	vec3 g_lightDir;	// ライト方向
};

void main()
{
	mat4 wvpMatrix = g_projection * g_view * g_world;
	vec4 transformNormal = wvpMatrix * vec4(inNormal, 1.0);
	vec3 diffuse = inColor.rgb * clamp(dot(transformNormal.xyz, g_lightDir), 0.0, 1.0);
	outColor = vec4(diffuse, inColor.a);
	outUV = inUV;
	gl_Position = wvpMatrix * inPos;
}

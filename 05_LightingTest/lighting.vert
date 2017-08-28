#version 450

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec4 inColor;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 outLightDir;
layout (location = 4) out vec3 outEyeDir;

// ユニフォームバッファ
layout (binding = 0) uniform VALUES
{
	mat4 g_projection;	// 投影行列
	mat4 g_view;		// 視線行列
	mat4 g_world;		// ワールド行列
	vec4 g_lightDir;	// ライト方向
	vec4 g_eyeDir;		// 視線方向
};

void main()
{
	// ライティング計算はフラグメントシェーダーで行う
	mat4 pvwMatrix = g_projection * g_view * g_world;
	outColor = inColor;
	outUV = inUV;
	outNormal = normalize((g_world * vec4(inNormal, 1.0)).xyz);
	outLightDir = g_lightDir.xyz;
	outEyeDir = g_eyeDir.xyz;
	gl_Position = pvwMatrix * inPos;
}

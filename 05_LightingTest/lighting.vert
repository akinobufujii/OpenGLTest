#version 450

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec4 inColor;
layout (location = 4) in vec3 inTangent;
layout (location = 5) in vec3 inBitangent;

out vec2 modelUV;
out vec4 colorDiffuse;
out vec3 vecLightDir;
out vec3 vecEyeDir;
out mat3 matTBN;

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
	colorDiffuse = inColor;
	modelUV = inUV;
	vecLightDir = g_lightDir.xyz;
	vecEyeDir = g_eyeDir.xyz;
	matTBN = mat3(
		normalize((g_world * vec4(inTangent, 1.0)).xyz),
		normalize((g_world * vec4(inBitangent, 1.0)).xyz),
		normalize((g_world * vec4(inNormal, 1.0)).xyz)
	);
	gl_Position = pvwMatrix * inPos;
}

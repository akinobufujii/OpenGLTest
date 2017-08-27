#version 450

layout (binding = 0) uniform sampler2D samplerColorMap;

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inLightDir;

layout (location = 0) out vec4 outputColor;

void main()
{
	// ランバート計算
	vec3 diffuse = inColor.rgb * max(dot(inNormal.xyz, inLightDir), 0.0);
	outputColor = texture(samplerColorMap, inUV) * vec4(diffuse, inColor.a);
}

#version 450

layout (binding = 0) uniform sampler2D samplerColorMap;

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inLightDir;
layout (location = 4) in vec3 inEyeDir;

layout (location = 0) out vec4 outputColor;

void main()
{
	// ランバート平行光源
	vec3 diffuse = inColor.rgb * max(dot(inNormal, -inLightDir), 0.0);

	// フォン鏡面反射
	vec3 reflectVec = reflect(inLightDir, inNormal);
    float specularLight = pow(max(dot(-inEyeDir, reflectVec), 0.0), 20);

	outputColor = texture(samplerColorMap, inUV) * vec4(diffuse, inColor.a);
	outputColor.rgb += specularLight;
}

#version 450

layout (binding = 0) uniform sampler2D samplerColorMap;
layout (binding = 1) uniform sampler2D samplerMetallicMap;
layout (binding = 2) uniform sampler2D samplerRoughnessMap;

varying vec2 modelUV;
varying vec4 colorDiffuse;
varying vec3 vecNormal;
varying vec3 vecLightDir;
varying vec3 vecEyeDir;

void main()
{
	// ランバート平行光源
	vec3 diffuse = colorDiffuse.rgb * max(dot(vecNormal, -vecLightDir), 0.0);

	// フォン鏡面反射
	vec3 reflectVec = reflect(vecLightDir, vecNormal);
    float specularLight = pow(max(dot(-vecEyeDir, reflectVec), 0.0), 20);

	gl_FragColor = texture(samplerColorMap, modelUV) * vec4(diffuse, colorDiffuse.a);
	gl_FragColor.rgb += vec3(specularLight) * texture(samplerMetallicMap, modelUV).rgb * (1.0 - texture(samplerRoughnessMap, modelUV).rgb);
}

#version 450

layout (binding = 0) uniform sampler2D samplerColorMap;
layout (binding = 1) uniform sampler2D samplerMetallicMap;
layout (binding = 2) uniform sampler2D samplerRoughnessMap;
layout (binding = 3) uniform sampler2D samplerNormalMap;

varying vec2 modelUV;
varying vec4 colorDiffuse;
varying vec3 vecLightDir;
varying vec3 vecEyeDir;
varying mat3 matTBN;

void main()
{
	// 法線算出
	vec3 normal = normalize(texture(samplerNormalMap, modelUV).rgb * 2.0 - 1.0);
	normal = normalize(matTBN * normal);

	// ランバート平行光源
	vec3 diffuse = colorDiffuse.rgb * max(dot(normal, -vecLightDir), 0.0);

	// フォン鏡面反射
	vec3 reflectVec = reflect(vecLightDir, normal);
    float specularLight = pow(max(dot(-vecEyeDir, reflectVec), 0.0), 20);

	// ディフューズ算出
	gl_FragColor = texture(samplerColorMap, modelUV) * vec4(diffuse, colorDiffuse.a);

	// Specularハイライト * メタリック * ラフネス = 最終的なハイライト
	gl_FragColor.rgb += vec3(specularLight) * texture(samplerMetallicMap, modelUV).rgb * (1.0 - texture(samplerRoughnessMap, modelUV).rgb);
}

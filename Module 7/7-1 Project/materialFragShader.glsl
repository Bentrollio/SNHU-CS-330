#version 430 core

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingLightDir2;
in vec3 varyingVertPos;
in vec3 varyingHalfVector; // Blinn-Phong shading
in vec3 varyingHalfVector2;

in vec4 vertexColor; // aka varyingColor
in vec2 tc;
out vec4 FragColor;

struct PositionalLight
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform PositionalLight light2;
uniform Material material;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform vec4 objectColor;

layout (binding = 0) uniform sampler2D samp;
layout (binding = 1) uniform sampler2D samp1;

void main(void)
{	
	// Sample textures
	vec4 mainTextureColor = texture(samp, tc);
	vec4 secondTextureColor = texture(samp1, tc);


	// Normalize the light, normal and view vectors
	vec3 L = normalize(varyingLightDir);
	vec3 L2 = normalize(varyingLightDir2);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);

	vec3 H = normalize(varyingHalfVector);
	vec3 H2 = normalize(varyingHalfVector2);


	// Compute light reflection vector with respect to N:
	//vec3 R = normalize(reflect(-L, N));
	//vec3 R2 = normalize(reflect(-L2, N));
	// Get the angle between the light and surface normal
	float cosTheta = dot(L,N);
	float cosTheta2 = dot(L2, N);
	// angle between the view vector and reflected light:
	float cosPhi = dot(H, N);
	float cosPhi2 = dot(H2, N);

	// Compute ADS contributions (per pixel), and combine to build output color
	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0);
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess * 3.0); // tweak to improve specular highlight

	vec3 ambient2 = ((globalAmbient * material.ambient) + (light2.ambient * material.ambient)).xyz;
	vec3 diffuse2 = light2.diffuse.xyz * material.diffuse.xyz * max(cosTheta2,0.0);
	vec3 specular2 = light2.specular.xyz * material.specular.xyz * pow(max(cosPhi2,0.0), material.shininess * 3.0);

	//FragColor = vec4((ambient + diffuse + specular), 1.0);
	vec3 phong1;
	vec3 phong2;
	vec3 mixPhong1;
	vec3 mixPhong2;

	// Check if the width of the texture at mipmap level 0 is greater than 1 and
	// the there is an alpha level for the second texture

	if ((textureSize(samp, 0).x > 1) && (secondTextureColor.a > 0.0)) {
		phong1 = (ambient + diffuse + specular) * mainTextureColor.xyz;
		phong2 = (ambient2 + diffuse2 + specular2) * mainTextureColor.xyz;
		mixPhong1 = (ambient + diffuse + specular) * secondTextureColor.xyz;
		mixPhong2 = (ambient + diffuse2 +specular2) * secondTextureColor.xyz;
		FragColor = mix(vec4(phong1 + phong2, 1.0), vec4(mixPhong1 + mixPhong2, 1.0), 0.5);
	}
	else if (textureSize(samp, 0).x > 1) {
		phong1 = (ambient + diffuse + specular) * mainTextureColor.xyz;
		phong2 = (ambient2 + diffuse2 + specular2) * mainTextureColor.xyz;
		FragColor = vec4(phong1 + phong2, 1.0);
	}
	else {
	phong1 = (ambient + diffuse + specular);
	phong2 = (ambient2 + diffuse2 + specular2);
	FragColor = vec4(phong1 + phong2, 1.0);

	}

}
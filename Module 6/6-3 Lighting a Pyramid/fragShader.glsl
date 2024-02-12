#version 430 core

in vec3 vertexFragmentNormal; // Outgoing normals to fragment shader
in vec3 vertexFragmentPos; // vertexColor replacement?
in vec2 tc;

//in vec4 vertexColor; // aka varyingColor

out vec4 fragmentColor; // Outgoing color to GPU

uniform mat4 model;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

// Uniform / Global variables for object color, light color, light position and camera/view position
uniform vec4 objectColor;
uniform vec3 ambientColor;
uniform vec3 light1Color;
uniform vec3 light1Position;
uniform vec3 light2Color;
uniform vec3 light2Position;
uniform vec3 viewPosition;
layout (binding = 0) uniform sampler2D samp; // uTexture
uniform vec2 uvScale;
uniform float ambientStrength = 0.1f; // Set ambient or global lighting strength
uniform float specularIntensity1 = 0.8f;
uniform float highlightSize1 = 16.0f;
uniform float specularIntensity2 = 0.8f;
uniform float highlightSize2 = 16.0f;

void main(void)
{
	// Phong lighting model calculations to generate ambient, diffuse, and specular components.

	// Calculate ambient lighting
	vec3 ambient = ambientStrength * ambientColor; // Generate ambient light color

	// Calculate Diffuse lighting
	vec3 norm = normalize(vertexFragmentNormal); // Normalize vectors to 1 unit
	vec3 light1Direction = normalize(light1Position - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on object
	float impact1 = max(dot(norm, light1Direction), 0.0); // Calculate diffuse impact by generating dot product of normal and light
	vec3 diffuse1 = impact1 * light1Color; // Generate diffuse light color
	vec3 light2Direction = normalize(light2Position - vertexFragmentPos);
	float impact2 = max(dot(norm, light2Direction), 0.0);
	vec3 diffuse2 = impact2 * light2Color;

	// Calculate Specular Lighting
	vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
	vec3 reflectDir1 = reflect(-light1Direction, norm); // Calculate reflection vector
	// Calculate specular component
	float specularComponent1 = pow(max(dot(viewDir, reflectDir1), 0.0), highlightSize1);
	vec3 specular1 = specularIntensity1 * specularComponent1 * light1Color;
	vec3 reflectDir2 = reflect(-light2Direction, norm); // Calculate reflection vector
	// Calculate specular component
	float specularComponent2 = pow(max(dot(viewDir, reflectDir2), 0.0), highlightSize2);
	vec3 specular2 = specularIntensity2 * specularComponent2 * light2Color;

	// Calculate Phong result
	// Texture holds the color to be used for all three components
	vec4 textureColor = texture(samp, tc);
	vec3 phong1;
	vec3 phong2;

	if (textureSize(samp, 0).x > 1) {
		 //FragColor = texture(samp, tc);
		 phong1 = (ambient + diffuse1 + specular1) * textureColor.xyz;
		 phong2 = (ambient + diffuse2 + specular2) * textureColor.xyz;
	}
	else {
		phong1 = (ambient + diffuse1 + specular1) * objectColor.xyz;
		phong2 = (ambient + diffuse2 + specular2) * objectColor.xyz;
	}

	fragmentColor = vec4(phong1 + phong2, 1.0); // Send lighting results to GPU

}
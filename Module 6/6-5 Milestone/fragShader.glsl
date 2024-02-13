#version 430 core

in vec4 vertexColor;
in vec2 tc;

out vec4 FragColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform vec4 objectColor;

layout (binding = 0) uniform sampler2D samp;
layout (binding = 1) uniform sampler2D samp1;

void main(void)
{	// Sample textures
	vec4 mainTexture = texture(samp, tc);
	vec4 secondTexture = texture(samp1, tc);

	// Check if the width of the texture at mipmap level 0 is greater than 1 and
	// the there is an alpha level for the second texture
	if ((textureSize(samp, 0).x > 1) && (secondTexture.a > 0.0)) {
		FragColor = mix(mainTexture, secondTexture, 0.25); // blend textures
	}
	//
	else if (textureSize(samp, 0).x > 1) {
		FragColor = mainTexture;
	}
	// No valid textures, just uses the object color.
	else {
		FragColor = objectColor;
	}
}

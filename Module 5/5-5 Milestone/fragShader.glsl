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
{
	// Checks the width of the texture at mipmap level 0
	if (textureSize(samp, 0).x > 1) {
		// Use the texture
		FragColor = texture(samp, tc);
	}
	else if (textureSize(samp1, 0).x > 1) {
		// Use a second texture
		FragColor = mix(texture(samp, tc), texture(samp1, tc), 0.2);
	}
	else {
		// Use color from uniform
		FragColor = objectColor;
	}
}

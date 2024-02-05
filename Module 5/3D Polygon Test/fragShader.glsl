#version 430 core

in vec4 vertexColor;
in vec2 tc;

out vec4 FragColor;

uniform mat4 proj_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

layout (binding = 0) uniform sampler2D samp;
layout (binding = 1) uniform sampler2D samp1;
uniform bool multipleTextures;

void main(void)
{ //FragColor = vertexColor;
  FragColor = texture(samp, tc);
  if (multipleTextures)
  {
	vec4 extraTexture = texture(samp1, tc);
	if (extraTexture.a != 0.0)
		FragColor = extraTexture;
  }
}
#version 430 core

//in vec4 vertexColor;
in vec2 tc;

out vec4 FragColor;

uniform mat4 proj_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

layout (binding = 0) uniform sampler2D samp;

void main(void)
{ //FragColor = vertexColor;
  FragColor = texture(samp, tc);
}
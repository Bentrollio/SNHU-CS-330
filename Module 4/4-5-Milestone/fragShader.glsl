#version 430 core

in vec4 vertexColor;
out vec4 FragColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
	FragColor = vertexColor;
}
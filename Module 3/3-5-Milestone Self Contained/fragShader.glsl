#version 430 core

in vec4 vertexColor;

out vec4 FragColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform vec4 objectColor;

void main(void)
{ //FragColor = vec4(objectColor);
	FragColor = vertexColor;
}
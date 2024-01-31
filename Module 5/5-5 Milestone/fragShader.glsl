#version 430 core

in vec4 vertexColor;
out vec4 FragColor;
//out vec4 FragColor2;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform vec4 objectColor;

void main(void)
{
	//FragColor = vertexColor;
	FragColor = vec4(objectColor);
}
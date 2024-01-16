#version 430 core

in vec4 varyingColor;

out vec4 FragColor;

uniform mat4 proj_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

void main(void)
{ FragColor = varyingColor;
}
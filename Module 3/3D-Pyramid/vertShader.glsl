#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

out vec4 varyingColor;
void main(void)
{ 
	gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
	varyingColor = vec4(position, 1.0) * 0.5 + vec4(2, .5, .5, 1.0);
}
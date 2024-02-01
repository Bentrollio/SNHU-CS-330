#version 430 core

in vec4 vertexColor;
in vec2 tc;

out vec4 FragColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform vec4 objectColor;

layout (binding = 0) uniform sampler2D samp;

void main(void)
{
	//FragColor = vertexColor;
	//FragColor = vec4(objectColor);
	//FragColor2 = texture(samp, tc);
	FragColor = texture(samp, tc);

}

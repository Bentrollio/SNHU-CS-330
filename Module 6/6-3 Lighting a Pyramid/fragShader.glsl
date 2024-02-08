#version 430 core

in vec4 vertexColor; // aka varyingColor
in vec2 tc;
out vec4 FragColor;

struct PositionalLight
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform vec4 objectColor;

layout (binding = 0) uniform sampler2D samp;

void main(void)
{
	FragColor = vertexColor;


/*	if (textureSize(samp, 0).x > 1) {
		 FragColor = texture(samp, tc);
	}
	else {
		FragColor = objectColor;
	}*/

}
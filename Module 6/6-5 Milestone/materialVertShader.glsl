#version 430 core
layout (location = 0) in vec3 position; // Vertex data from vertexattributepointer 0 aka vertPos
layout (location = 1) in vec3 normal; // Normal data from vertexattributerpointer 1 aka vertNormal
layout (location = 2) in vec2 texCoord; // Texture data from vertexattributepointer 2
layout (location = 3) in vec4 color; // Color data

//out vec4 vertexColor; // sends color to fragment shader aka varyingColor in book
out vec2 tc; // texture coordinate output to rasterizer for interpolation

out vec3 varyingNormal; // eye-space vertex normal
out vec3 varyingLightDir; // vector pointing to the light
out vec3 varyingLightDir2;
out vec3 varyingVertPos; // vertex position in eye-space

layout (binding = 0) uniform sampler2D samp; // not used in vertex shader
layout (binding = 1) uniform sampler2D samp1;

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
uniform PositionalLight light2;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix; // for transforming normals

void main(void)
{ 
	//vertexColor = color;
	tc = texCoord;

	varyingVertPos = (mv_matrix * vec4(position, 1.0)).xyz;
	varyingLightDir = light.position - varyingVertPos;
	varyingLightDir2 = light2.position - varyingVertPos;
	varyingNormal = (norm_matrix * vec4(normal,1.0)).xyz;

	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0f);
}
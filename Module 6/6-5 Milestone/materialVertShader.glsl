#version 430 core
layout (location = 0) in vec3 position; // Vertex data from vertexattributepointer 0 aka vertexPos
layout (location = 1) in vec3 normal; // Normal data from vertexattributerpointer 1 aka vertexNormal
layout (location = 2) in vec2 texCoord; // Texture data from vertexattributepointer 2
layout (location = 3) in vec4 color; // Color data

out vec3 vertexFragmentNormal; // Outgoing normals to fragment shader
out vec3 vertexFragmentPos; // vertexColor replacement?

//out vec4 vertexColor; // sends color to fragment shader aka varyingColor in book
out vec2 tc; // texture coordinate output to rasterizer for interpolation

layout (binding = 0) uniform sampler2D samp; // not used in vertex shader
layout (binding = 1) uniform sampler2D samp1; // not used in vertex shader

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform Material material;
uniform mat4 model;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{ 
	//vertexColor = color;


	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0f); // Transforms vertices into clip coordinates

	vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view matrix)

	vertexFragmentNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
	tc = texCoord;
}
#version 430 core
layout (location = 0) in vec3 position; // Vertex data from vertexattributepointer 0
layout (location = 1) in vec4 normal; // Normal data from vertexattributerpointer 1
layout (location = 2) in vec2 texCoord; // Texture data from vertexattributepointer 2
layout (location = 3) in vec4 color; // Color data

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

out vec4 vertexColor; // sends color to fragment shader
out vec2 tc; // texture coordinate output to rasterizer for interpolation

layout (binding = 0) uniform sampler2D samp; // not used in vertex shader

void main(void)
{ 
	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0f);
	vertexColor = color;
	tc = texCoord;
}
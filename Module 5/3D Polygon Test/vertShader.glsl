#version 430 core
layout (location = 0) in vec3 position; // Vertex data from vertexattributepointer 0
layout (location = 1) in vec4 color; // Color data from vertexattributerpointer 1
layout (location = 2) in vec2 texCoord; // Texture data from vertexattributepointer 2
uniform mat4 proj_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

out vec4 vertexColor; // sends color to fragment shader
out vec2 tc; // texture coordinate output to rasterizer for interpolation

layout (binding = 0) uniform sampler2D samp; // not used in vertex shader

void main(void)
{ 
	gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
	vertexColor = color;
	tc = texCoord;
}
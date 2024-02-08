#version 430 core
layout (location = 0) in vec3 position; // Vertex data from vertexattributepointer 0 aka vertPos
layout (location = 1) in vec3 normal; // Normal data from vertexattributerpointer 1 aka vertNormal
layout (location = 2) in vec2 texCoord; // Texture data from vertexattributepointer 2
layout (location = 3) in vec4 color; // Color data

out vec4 vertexColor; // sends color to fragment shader aka varyingColor in book
out vec2 tc; // texture coordinate output to rasterizer for interpolation

layout (binding = 0) uniform sampler2D samp; // not used in vertex shader

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
uniform mat4 norm_matrix; // for transforming normals

void main(void)
{ 
	//vertexColor = color;
	tc = texCoord;

	// Convert verex position to view space
	// Convert normal to view space and calculate view space light vector
	vec4 P = mv_matrix * vec4(position, 1.0);
	vec3 N = normalize((norm_matrix * vec4(normal, 1.0)).xyz);
	vec3 L = normalize(light.position - P.xyz);

	// view vector is equal to the negative of view space vertex position
	vec3 V = normalize(-P.xyz);

	// R is reflection of -L with respect to surface normal N
	vec3 R = reflect(-L,N);

	// ambient diffuse and specular contributions
	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(N,L), 0.0);
	vec3 specular = material.specular.xyz * light.specular.xyz * pow(max(dot(R,V), 0.0f), material.shininess);

	// Send the color output to the fragment shader
	vertexColor = vec4((ambient + diffuse + specular), 1.0);



	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0f);
}
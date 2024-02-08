#include "GLmesh.h"

// Creates the mesh
void createMesh(GLmesh& mesh) { // 5 vertices, 6 triangles, makes pyramid placed at origin
	GLfloat verts[] // Pyramid positions 5 vertices, 6 triangles
	{	// Vertices				//Normals	// texture coordinates
		// front face
		-1.0f, -1.0f, 1.0f,		0.0, 0.447, 0.894,	0.0f, 0.0f, // lower left corner
		1.0f, -1.0f, 1.0f,		0.0, 0.447, 0.894,	1.0f, 0.0f, // lower right corner
		0.0f, 1.0f, 0.0f,		0.0, 0.447, 0.894,	0.5f, 1.0f,	// tippy top

		// right face
		1.0f, -1.0f, 1.0f,		0.894, 0.447, 0.0,	0.0f, 0.0f, // lower left corner
		1.0f, -1.0f, -1.0f,		0.894, 0.447, 0.0,	1.0f, 0.0f, // lower right corner
		0.0f, 1.0f, 0.0f,		0.894, 0.447, 0.0,	0.5f, 1.0f, // tippy top

		// back face
		1.0f, -1.0f, -1.0f,		0.894, 0.447, 0.0,	0.0f, 0.0f, // lower left corner
		-1.0f, -1.0f, -1.0f,	0.894, 0.447, 0.0,	1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.894, 0.447, 0.0,	0.5f, 1.0f,

		// left face
		-1.0f, -1.0f, -1.0f,	0.0, 1.0, 0.0,		0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0, 1.0, 0.0,		1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.0, 1.0, 0.0,		0.5f, 1.0f,

		// base left front
		-1.0f, -1.0f, -1.0f,	0.0, 1.0, 0.0,		0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0, 1.0, 0.0,		1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,		0.0, 1.0, 0.0,		0.0f, 1.0f,

		// base right back
		1.0f, -1.0f, 1.0f,		0.0, 1.0, 0.0,		1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,	0.0, 1.0, 0.0,		0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0, 1.0, 0.0,		1.0f, 0.0f,

	};

	glGenVertexArrays(numVAOs, mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao[0]);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU
		
	// Creates the Vertex Attribute Pointer for the screen coordinates
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerNormal = 3; // Normals for lighting
	const GLuint floatsPerUV = 2; // Texture coordinates


	// Strides between vertex coordinates is 3 (xyz rgba)
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Instructs the GPU on how to handle the vbo data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, ie x and y | data type | deactivate normalization | 0
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void destroyMesh(GLmesh& mesh) {

	glDeleteVertexArrays(numVAOs, mesh.vao);
	glDeleteBuffers(numVBOs, mesh.vbo);
}
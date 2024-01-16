#include "GLmesh.h"

// Creates the mesh
void createMesh(GLmesh& mesh) {

	// Specifies Normalized Device Coordinates for triangle vertices
	GLfloat verts[] =
	{
		// The two triangles will be drawn using indices.
		// Left triangle indices: 0, 1, 2
		// Right triangle indices: 3, 2, 4

		// index 0
		-1.0f, 1.0f, 0.0f,	// top-first third of screen
		1.0f, 0.0f, 0.0f, 1.0f, // red

		// index 1
		-1.0f, 0.0f, 0.0f, // bottom-left of screen
		0.0f, 0.0f, 1.0f, 1.0f, // blue

		// index 2
		-0.5f, 0.0f, 0.0f,	// bottom-center of screen
		0.0f, 1.0f, 0.0f, 1.0f, // green

		// index 3
		0.0f, 0.0f, 0.0f,	// top-second third of the screen
		1.0f, 0.0f, 0.0f, 1.0f, // red

		// index 4
		0.0f, -1.0f, 0.0f,	// bottom-right of screen
		0.0f, 1.0f, 0.0f, 1.0f // green
	};

	glGenVertexArrays(numVAOs, mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao[0]);

	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU

	// Creates a buffer object for the indices
	GLushort indices[] = { 0, 1, 2, 3, 2, 4 }; // Using index 2 twice
	mesh.numIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Creates the Vertex Attribute Pointer for the screen coordinates
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 4; // (r, g, b, a)

	// Strides between vertex coordinates is 6 (xyz rgba)
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor);

	// Instructs the GPU on how to handle the vbo data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, ie x and y | data type | deactivate normalization | 0
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// Parameters: attribPointerPosition 1 | floats per color is 4, ie rgba | data type | deactivate normalization 
	// | 6 strides till next color | 2 floats til beginning of each color
	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);
}

void destroyMesh(GLmesh& mesh) {

	glDeleteVertexArrays(numVAOs, mesh.vao);
	glDeleteBuffers(numVBOs, mesh.vbo);
}
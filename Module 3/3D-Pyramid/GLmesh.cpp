#include "GLmesh.h"

// Creates the mesh
void createMesh(GLmesh& mesh) { // 5 vertices, 6 triangles, makes pyramid placed at origin
	GLfloat verts[] // Pyramid positions 5 vertices, 6 triangles
	{
		-1.0f, -1.0f, 1.0f, // Bottom front left vertex 0
		1.0f, -1.0f, -1.0f, // Bottom rear right vertex 1
		0.0f, 1.0f, 0.0f, // Top center point vertex 2
		1.0f, -1.0f, 1.0f, // Bottom front right vertex 3
		-1.0f, -1.0f, -1.0f, // Bottom rear left vertex 4
	};

	glGenVertexArrays(numVAOs, mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao[0]);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU
	
	GLushort indices[] = {
		0, 2, 3, // Triangle 1
		0, 2, 4, // Triangle 2
		1, 2, 3, // Triangle 3
		1, 2, 4, // Triangle 4
		0, 1, 3, // Triangle 5
		0, 1, 4 // Triangle 6
		
	};

	mesh.numIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Creates the Vertex Attribute Pointer for the screen coordinates
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 0; // (r, g, b, a) THIS IS SET TO 0 FOR NOW

	// Strides between vertex coordinates is 3 (xyz rgba)
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor);

	// Instructs the GPU on how to handle the vbo data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, ie x and y | data type | deactivate normalization | 0
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// Parameters: attribPointerPosition 1 | floats per color is temporarily 0, ie rgba | data type | deactivate normalization 
	// | 0 strides till next color | 2 floats til beginning of each color
	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);
}

void destroyMesh(GLmesh& mesh) {

	glDeleteVertexArrays(numVAOs, mesh.vao);
	glDeleteBuffers(numVBOs, mesh.vbo);
}
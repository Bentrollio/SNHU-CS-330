///////////////////////////////////////////////////////////////////////////////
// meshes.cpp
// ========
// create meshes for various 3D primitives: plane, pyramid, cube, cylinder, torus, sphere
//
//  ORIGINAL AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Modified and Adapted by Alex Baires
///////////////////////////////////////////////////////////////////////////////

#include "meshes.h"

#include <vector>

namespace
{
	const double M_PI = 3.14159265358979323846f;
	const double M_PI_2 = 1.571428571428571;
}

/**
 * CreateMeshes()
 * 
 * Creates 3D Meshes: plane, pyramid, cube, cylinder, torus, sphere
 * 
 */

void Meshes::CreateMeshes()
{
	CreateCubeMesh(cubeMesh);
	CreatePlaneMesh(planeMesh);
	/*CreatePrismMesh(prismMesh);
	CreateBoxMesh(boxMesh);
	CreateConeMesh(coneMesh);
	CreateCylinderMesh(cylinderMesh);
	CreateTaperedCylinderMesh(taperedCylinderMesh);
	CreatePyramid3Mesh(pyramid3Mesh);*/
	CreatePyramid4Mesh(pyramid4Mesh);
	/*CreateSphereMesh(sphereMesh);
	CreateTorusMesh(torusMesh);*/

}

void Meshes::DestroyMeshes()
{
	DestroyMesh(cubeMesh);
	DestroyMesh(planeMesh);
	DestroyMesh(prismMesh);
	DestroyMesh(boxMesh);
	DestroyMesh(coneMesh);
	DestroyMesh(cylinderMesh);
	DestroyMesh(pyramid3Mesh);
	DestroyMesh(pyramid4Mesh);
	DestroyMesh(sphereMesh);
	DestroyMesh(torusMesh);
}

/**
 * CreateCubeMesh(GLmesh &mesh)
 *
 * Creates a cube mesh and stores it in a VAO/VBO
 * 
 * Correct drawing command:
 * glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_SHORT, NULL);
 *
 */

void Meshes::CreateCubeMesh(GLmesh& mesh) {

	// Specifies Normalized Device Coordinates for triangle vertices
	GLfloat verts[] = // vertexPositions in the book
	{	
		// Vertices				// Colors
		-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 1.0f, // Top rear left vertex 0
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f, 1.0f, // Bottom rear left vertex 1
		1.0f, -1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 1.0f, // Bottom rear right vertex 2
		1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 1.0f, // Top rear right vertex 3
		1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f, // Bottom front right vertex 4
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f, // Top front right vertex 5
		-1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f, // Bottom front left vertex 6
		-1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f, // Top front left vertex 7

	};

	glGenVertexArrays(numVAOs, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU


	// Creates a buffer object for the indices
	GLushort indices[] = {
		0, 1, 3, // Triangle 1
		1, 2, 3, // Triangle 2
		0, 1, 7, // Triangle 3
		0, 3, 7, // Triangle 4
		3, 5, 7, // Triangle 5
		1, 6, 7, // Triangle 6
		1, 2, 4, // Triangle 7
		1, 4, 6, // Triangle 8
		4, 6, 7, // Triangle 9
		4, 5, 7, // Triangle 10
		3, 4, 5, // Triangle 11
		2, 3, 4  // Triangle 12
	};

	// Float values per type
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 4; // (r, g, b, a) THIS IS SET TO 0 FOR NOW
	const GLuint floatsPerNormal = 0;
	const GLuint floatsPerUV = 0;

	// Store the vertex and index count inside the mesh
	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV));
	mesh.numIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

void Meshes::CreatePlaneMesh(GLmesh& mesh) {

	// Specifies Normalized Device Coordinates for triangle vertices
	GLfloat verts[]
	{	
		// Vertices				// Colors
		-1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, -1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, -1.0f,		1.0f, 0.0f, 0.0f, 1.0f,

	};

	glGenVertexArrays(numVAOs, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU
	
	// Index data
	GLushort indices[] 
	{
		0,1,2,
		0,3,2
	};

	// Float values per type
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 4; // (r, g, b, a) THIS IS SET TO 0 FOR NOW
	const GLuint floatsPerNormal = 0;
	const GLuint floatsPerUV = 0;

	// Store the vertex and index count inside the mesh
	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV));
	mesh.numIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

void Meshes::CreatePyramid4Mesh(GLmesh& mesh) {
	
	GLfloat verts[] 
	{	// Vertices				// Colors
		-1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f, 
		1.0f, -1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,	0.5f, 1.0f, 0.5f, 1.0f,
	};

	glGenVertexArrays(numVAOs, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU

	GLushort indices[]
	{
		0, 2, 3, // Triangle 1
		0, 2, 4, // Triangle 2
		1, 2, 3, // Triangle 3
		1, 2, 4, // Triangle 4
		0, 1, 3, // Triangle 5
		0, 1, 4 // Triangle 6
	};

	// Float values per type
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 4; // (r, g, b, a) THIS IS SET TO 0 FOR NOW
	const GLuint floatsPerNormal = 0;
	const GLuint floatsPerUV = 0;

	// Store the vertex and index count inside the mesh
	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV));
	mesh.numIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

void Meshes::DestroyMesh(GLmesh &mesh) {

	glDeleteVertexArrays(numVAOs, &mesh.vao);
	glDeleteBuffers(numVBOs, mesh.vbo);
}
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

using namespace std;

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
	CreatePrismMesh(prismMesh);
	CreateConeMesh(coneMesh);
	CreateCylinderMesh(cylinderMesh);
	CreateTaperedCylinderMesh(taperedCylinderMesh);
	CreateTaperedPolygonMesh(taperedPolygonMesh);
	CreatePyramid3Mesh(pyramid3Mesh);
	CreatePyramid4Mesh(pyramid4Mesh);
	CreateSphereMesh(sphereMesh);
	CreateWingMesh(wingMesh);
}

void Meshes::DestroyMeshes()
{
	DestroyMesh(cubeMesh);
	DestroyMesh(planeMesh);
	DestroyMesh(prismMesh);
	DestroyMesh(boxMesh);
	DestroyMesh(coneMesh);
	DestroyMesh(cylinderMesh);
	DestroyMesh(taperedPolygonMesh);
	DestroyMesh(pyramid3Mesh);
	DestroyMesh(pyramid4Mesh);
	DestroyMesh(sphereMesh);
	DestroyMesh(wingMesh);
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
		// Vertex				// Normals				//Texture
		// Cube's Rear side left triangle
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,	// top left corner
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f,	// bottom left corner
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f,	// bottom right corner

		// Cube's Rear side right triangle
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f,	// bottom right corner
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f, -1.0f,		0.0f, 1.0f,	// top right corner
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,// top left corner 

		// Cube's Right side right triangle
		1.0f, -1.0f, -1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,	// bottom right corner
		1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// bottom left corner
		1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	// top right corner

		// Cube's Right side left triangle
		1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// bottom left corner
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,	// top left corner
		1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	// top right corner

		// Cube's Front side right triangle
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,	// bottom right corner
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// bottom left corner
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	// top right corner

		// Cube's Front side left triangle
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// bottom left corner
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,	// top left corner
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	// top right corner

		// Cube's Left side right triangle
		-1.0f, -1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,	// bottom right corner
		-1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// bottom left corner
		-1.0f, 1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	// top right corner

		// Cube's Left side left triangle
		-1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// bottom left corner
		-1.0f, 1.0f, -1.0f,		-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,	// top left corner
		-1.0f, 1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	// top right corner

		// Cube's Bottom side right triangle (facing viewer)
		-1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,	// top left corner
		1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f,	// top right corner
		1.0f, -1.0f, -1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f,	// bottom right corner

		// Cube's Bottom side left triangle (facing viewer)
		1.0f, -1.0f, -1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f,	// bottom right corner
		-1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f,	// bottom left corner
		-1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,	// top left corner

		// Cube's Top side right triangle (facing viewer)
		-1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// top left corner
		1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,	// top right corner
		1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,// bottom right corner

		// Cube's Top side left triangle (facing viewer)
		1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// bottom right corner
		-1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,	// bottom left corner
		-1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// top left corner

	};

	glGenVertexArrays(numVAOs, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU

	// Float values per type
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 0; // (r, g, b, a) THIS IS SET TO 0 FOR NOW
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// Store the vertex and index count inside the mesh
	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV));
	//mesh.numIndices = sizeof(indices) / sizeof(indices[0]);

	// Strides between vertex coordinates is 3 (xyz rgba)
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV);

	// Instructs the GPU on how to handle the vbo data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, ie x and y | data type | deactivate normalization | 0
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// Parameters: attribPointerPosition 1 | floats per color is temporarily 0, ie rgba | data type | deactivate normalization 
	// | 0 strides till next color | 2 floats til beginning of each color
	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void Meshes::CreatePlaneMesh(GLmesh& mesh) {

	// Specifies Normalized Device Coordinates for triangle vertices
	GLfloat verts[]
	{	
		// Vertices				//Normals			// Texture coordinates	// Colors
		-1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,				/*0.0f, 0.50196078f, 1.0f, 1.0f*/
		1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,				/*0.0f, 0.50196078f, 1.0f, 1.0f,*/
		1.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,				/*0.0f, 0.50196078f, 1.0f, 1.0f,*/
		-1.0f, 0.0f, -1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,				/*0.0f, 0.50196078f, 1.0f, 1.0f,*/

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
	const GLuint floatsPerColor = 0; // (r, g, b, a) THIS IS SET TO 0 FOR NOW
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// Store the vertex and index count inside the mesh
	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV));
	mesh.numIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates is 3 (xyz rgba)
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV);

	// Instructs the GPU on how to handle the vbo data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, ie x and y | data type | deactivate normalization | 0
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// Parameters: attribPointerPosition 1 | floats per color is temporarily 0, ie rgba | data type | deactivate normalization 
	// | 0 strides till next color | 2 floats til beginning of each color
	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);


}

void Meshes::CreatePrismMesh(GLmesh& mesh) {
	// Vertex Data
	GLfloat verts[] = {
		//Positions				//Normals
		// ------------------------------------------------------

		//Back Face				//Negative Z Normal  
		0.5f, 0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,

		//Bottom Face			//Negative Y Normal
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		0.0f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		0.5f, 1.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,

		//Left Face/slanted		//Normals
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,
		-0.5f, 0.5f,  -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 1.0f,
		0.0f, 0.5f,  0.5f,		0.894427180f,  0.0f,  -0.447213590f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,
		0.0f, -0.5f,  0.5f,		0.894427180f,  0.0f,  -0.447213590f,	1.0f, 0.0f,
		0.0f, 0.5f,  0.5f,		0.894427180f,  0.0f,  -0.447213590f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,

		//Right Face/slanted	//Normals
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		1.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,
		0.0f, -0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		1.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,

		//Top Face				//Positive Y Normal		//Texture Coords.
		0.5f, 0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		0.0f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		0.5f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
	};

	// Float values per type
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerNormal = 3; // (r, g, b)
	const GLuint floatsPerUV = 2;

	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

	glGenVertexArrays(numVAOs, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU

	// Strides between vertex coordinates
	GLuint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Instructs the GPU on how to handle the vbo data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, ie x and y | data type | deactivate normalization | 0
		// Instructs the GPU on how to handle the vbo data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, ie x and y | data type | deactivate normalization | 0
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// Parameters: attribPointerPosition 1 | floats per color is temporarily 0, ie rgba | data type | deactivate normalization 
	// | 0 strides till next color | 2 floats til beginning of each color
	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void Meshes::CreateConeMesh(GLmesh& mesh) {
	GLfloat verts[] = {
		// cone bottom			// normals			// texture coords
		1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f,1.0f,
		.98f, 0.0f, -0.17f,		0.0f, -1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 0.0f, -0.34f,		0.0f, -1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 0.0f, -0.64f,		0.0f, -1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 0.0f, -0.77f,		0.0f, -1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 0.0f, -0.94f,		0.0f, -1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 0.0f, -0.98f,		0.0f, -1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 0.0f, -0.98f,	0.0f, -1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 0.0f, -0.94f,	0.0f, -1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 0.0f, -0.77f,	0.0f, -1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 0.0f, -0.64f,	0.0f, -1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 0.0f, -0.34f,	0.0f, -1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 0.0f, -0.17f,	0.0f, -1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.983f,

		// cone sides		// normals									// texture coords
		1.0f, 0.0f, 0.0f,		-0.993150651f, 0.0f, 0.116841137f, 		0.0f,0.0f,
		0.0f, 1.0f, 0.0f,		-0.993150651f, 0.0f, 0.116841137f, 		0.5f, 1.0f,
		.98f, 0.0f, -0.17f,		-0.993150651f, 0.0f, 0.116841137f, 		0.0277,0.0,
		.98f, 0.0f, -0.17f,		-0.973417103f, 0.0f, 0.229039446f, 		0.0277,0.0,
		0.0f, 1.0f, 0.0f,		-0.973417103f, 0.0f, 0.229039446f, 		0.5f, 1.0f,
		.94f, 0.0f, -0.34f,		-0.973417103f, 0.0f, 0.229039446f, 		0.0554,0.0f,
		.94f, 0.0f, -0.34f,		-0.916157305f, 0.0f, 0.400818795f, 		0.0554,0.0f,
		0.0f, 1.0f, 0.0f,		-0.916157305f, 0.0f, 0.400818795f, 		0.5f, 1.0f,
		.87f, 0.0f, -0.5f,		-0.916157305f, 0.0f, 0.400818795f, 		0.0831,0.0f,
		.87f, 0.0f, -0.5f,		-0.813733339f, 0.0f, 0.581238329f, 		0.0831,0.0f,
		0.0f, 1.0f, 0.0f,		-0.813733339f, 0.0f, 0.581238329f, 		0.5f, 1.0f,
		.77f, 0.0f, -0.64f,		-0.813733339f, 0.0f, 0.581238329f, 		0.1108f, 0.0f,
		.77f, 0.0f, -0.64f,		-0.707106769f, 0.0f, 0.707106769f, 		0.1108f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.707106769f, 0.0f, 0.707106769f, 		0.5f, 1.0f,
		.64f, 0.0f, -0.77f,		-0.707106769f, 0.0f, 0.707106769f, 		0.1385f, 0.0f,
		.64f, 0.0f, -0.77f,		-0.581238329f, 0.0f, 0.813733339f, 		0.1385f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.581238329f, 0.0f, 0.813733339f, 		0.5f, 1.0f,
		.5f, 0.0f, -0.87f,		-0.581238329f, 0.0f, 0.813733339f, 		0.1662f, 0.0f,
		.5f, 0.0f, -0.87f,		-0.400818795f, 0.0f, 0.916157305f, 		0.1662f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.400818795f, 0.0f, 0.916157305f, 		0.5f, 1.0f,
		.34f, 0.0f, -0.94f,		-0.400818795f, 0.0f, 0.916157305f, 		0.1939f, 0.0f,
		.34f, 0.0f, -0.94f,		-0.229039446f, 0.0f, 0.973417103f, 		0.1939f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.229039446f, 0.0f, 0.973417103f, 		0.5f, 1.0f,
		.17f, 0.0f, -0.98f,		-0.229039446f, 0.0f, 0.973417103f, 		0.2216f, 0.0f,
		.17f, 0.0f, -0.98f,		-0.116841137f, 0.0f, 0.993150651f, 		0.2216f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.116841137f, 0.0f, 0.993150651f, 		0.5f, 1.0f,
		0.0f, 0.0f, -1.0f,		-0.116841137f, 0.0f, 0.993150651f, 		0.2493f, 0.0f,
		0.0f, 0.0f, -1.0f,		0.116841137f, 0.0f, 0.993150651f, 		0.2493f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.116841137f, 0.0f, 0.993150651f, 		0.5f, 1.0f,
		-.17f, 0.0f, -0.98f,	0.116841137f, 0.0f, 0.993150651f, 		0.277f, 0.0f,
		-.17f, 0.0f, -0.98f,	0.229039446f, 0.0f, 0.973417103f, 		0.277f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.229039446f, 0.0f, 0.973417103f, 		0.5f, 1.0f,
		-.34f, 0.0f, -0.94f,	0.229039446f, 0.0f, 0.973417103f, 		0.3047f, 0.0f,
		-.34f, 0.0f, -0.94f,	0.400818795f, 0.0f, 0.916157305f, 		0.3047f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.400818795f, 0.0f, 0.916157305f, 		0.5f, 1.0f,
		-.5f, 0.0f, -0.87f,		0.400818795f, 0.0f, 0.916157305f, 		0.3324f, 0.0f,
		-.5f, 0.0f, -0.87f,		0.581238329f, 0.0f, 0.813733339f, 		0.3324f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.581238329f, 0.0f, 0.813733339f, 		0.5f, 1.0f,
		-.64f, 0.0f, -0.77f,	0.581238329f, 0.0f, 0.813733339f, 		0.3601f, 0.0f,
		-.64f, 0.0f, -0.77f,	0.707106769f, 0.0f, 0.707106769f, 		0.3601f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.707106769f, 0.0f, 0.707106769f, 		0.5f, 1.0f,
		-.77f, 0.0f, -0.64f,	0.707106769f, 0.0f, 0.707106769f, 		0.3878f, 0.0f,
		-.77f, 0.0f, -0.64f,	0.813733339f, 0.0f, 0.581238329f, 		0.3878f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.813733339f, 0.0f, 0.581238329f, 		0.5f, 1.0f,
		-.87f, 0.0f, -0.5f,		0.813733339f, 0.0f, 0.581238329f, 		0.4155f, 0.0f,
		-.87f, 0.0f, -0.5f,		0.916157305f, 0.0f, 0.400818795f, 		0.4155f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.916157305f, 0.0f, 0.400818795f, 		0.5f, 1.0f,
		-.94f, 0.0f, -0.34f,	0.916157305f, 0.0f, 0.400818795f, 		0.4432f, 0.0f,
		-.94f, 0.0f, -0.34f,	0.973417103f, 0.0f, 0.229039446f, 		0.4432f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.973417103f, 0.0f, 0.229039446f, 		0.5f, 1.0f,
		-.98f, 0.0f, -0.17f,	0.973417103f, 0.0f, 0.229039446f, 		0.4709f, 0.0f,
		-.98f, 0.0f, -0.17f,	0.993150651f, 0.0f, 0.116841137f, 		0.4709f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.993150651f, 0.0f, 0.116841137f, 		0.5f, 1.0f,
		-1.0f, 0.0f, 0.0f,		0.993150651f, 0.0f, 0.116841137f, 		0.4986f, 0.0f,
		-1.0f, 0.0f, 0.0f,		0.993150651f, 0.0f, -0.116841137f, 		0.4986f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.993150651f, 0.0f, -0.116841137f, 		0.5f, 1.0f,
		-.98f, 0.0f, 0.17f,		0.993150651f, 0.0f, -0.116841137f, 		0.5263f, 0.0f,
		-.98f, 0.0f, 0.17f,		0.973417103f, 0.0f, -0.229039446f, 		0.5263f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.973417103f, 0.0f, -0.229039446f, 		0.5f, 1.0f,
		-.94f, 0.0f, 0.34f,		0.973417103f, 0.0f, -0.229039446f, 		0.554f, 0.0f,
		-.94f, 0.0f, 0.34f,		0.916157305f, 0.0f, -0.400818795f, 		0.554f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.916157305f, 0.0f, -0.400818795f, 		0.5f, 1.0f,
		-.87f, 0.0f, 0.5f,		0.916157305f, 0.0f, -0.400818795f, 		0.5817f, 0.0f,
		-.87f, 0.0f, 0.5f,		0.813733339f, 0.0f, -0.581238329f, 		0.5817f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.813733339f, 0.0f, -0.581238329f, 		0.5f, 1.0f,
		-.77f, 0.0f, 0.64f,		0.813733339f, 0.0f, -0.581238329f, 		0.6094f, 0.0f,
		-.77f, 0.0f, 0.64f,		0.707106769f, 0.0f, -0.707106769f, 		0.6094f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.707106769f, 0.0f, -0.707106769f, 		0.5f, 1.0f,
		-.64f, 0.0f, 0.77f,		0.707106769f, 0.0f, -0.707106769f, 		0.6371f, 0.0f,
		-.64f, 0.0f, 0.77f,		0.581238329f, 0.0f, -0.813733339f, 		0.6371f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.581238329f, 0.0f, -0.813733339f, 		0.5f, 1.0f,
		-.5f, 0.0f, 0.87f,		0.581238329f, 0.0f, -0.813733339f, 		0.6648f, 0.0f,
		-.5f, 0.0f, 0.87f,		0.400818795f, 0.0f, -0.916157305f, 		0.6648f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.400818795f, 0.0f, -0.916157305f, 		0.5f, 1.0f,
		-.34f, 0.0f, 0.94f,		0.400818795f, 0.0f, -0.916157305f, 		0.6925f, 0.0f,
		-.34f, 0.0f, 0.94f,		0.229039446f, 0.0f, -0.973417103f, 		0.6925f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.229039446f, 0.0f, -0.973417103f, 		0.5f, 1.0f,
		-.17f, 0.0f, 0.98f,		0.229039446f, 0.0f, -0.973417103f, 		0.7202f, 0.0f,
		-.17f, 0.0f, 0.98f,		0.116841137f, 0.0f, -0.993150651f, 		0.7202f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.116841137f, 0.0f, -0.993150651f, 		0.5f, 1.0f,
		0.0f, 0.0f, 1.0f,		0.116841137f, 0.0f, -0.993150651f, 		0.7479f, 0.0f,
		0.0f, 0.0f, 1.0f,		-0.116841137f, 0.0f, -0.993150651f, 	0.7479f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.116841137f, 0.0f, -0.993150651f, 	0.5f, 1.0f,
		.17f, 0.0f, 0.98f,		-0.116841137f, 0.0f, -0.993150651f, 	0.7756f, 0.0f,
		.17f, 0.0f, 0.98f,		-0.229039446f, 0.0f, -0.973417103f, 	0.7756f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.229039446f, 0.0f, -0.973417103f, 	0.5f, 1.0f,
		.34f, 0.0f, 0.94f,		-0.229039446f, 0.0f, -0.973417103f, 	0.8033f, 0.0f,
		.34f, 0.0f, 0.94f,		-0.400818795f, 0.0f, -0.916157305f, 	0.8033f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.400818795f, 0.0f, -0.916157305f, 	0.5f, 1.0f,
		.5f, 0.0f, 0.87f,		-0.400818795f, 0.0f, -0.916157305f, 	0.831f, 0.0f,
		.5f, 0.0f, 0.87f,		-0.581238329f, 0.0f, -0.813733339f, 	0.831f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.581238329f, 0.0f, -0.813733339f, 	0.5f, 1.0f,
		.64f, 0.0f, 0.77f,		-0.581238329f, 0.0f, -0.813733339f, 	0.8587f, 0.0f,
		.64f, 0.0f, 0.77f,		-0.707106769f, 0.0f, -0.707106769f, 	0.8587f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.707106769f, 0.0f, -0.707106769f, 	0.5f, 1.0f,
		.77f, 0.0f, 0.64f,		-0.707106769f, 0.0f, -0.707106769f, 	0.8864f, 0.0f,
		.77f, 0.0f, 0.64f,		-0.813733339f, 0.0f, -0.581238329f, 	0.8864f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.813733339f, 0.0f, -0.581238329f, 	0.5f, 1.0f,
		.87f, 0.0f, 0.5f,		-0.813733339f, 0.0f, -0.581238329f, 	0.9141f, 0.0f,
		.87f, 0.0f, 0.5f,		-0.916157305f, 0.0f, -0.400818795f, 	0.9141f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.916157305f, 0.0f, -0.400818795f, 	0.5f, 1.0f,
		.94f, 0.0f, 0.34f,		-0.916157305f, 0.0f, -0.400818795f, 	0.9418f, 0.0f,
		.94f, 0.0f, 0.34f,		-0.973417103f, 0.0f, -0.229039446f, 	0.9418f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.973417103f, 0.0f, -0.229039446f, 	0.5f, 1.0f,
		.98f, 0.0f, 0.17f,		-0.973417103f, 0.0f, -0.229039446f, 	0.9695f, 0.0f,
		.98f, 0.0f, 0.17f,		-0.993150651f, 0.0f, -0.116841137f, 	0.9695f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.993150651f, 0.0f, -0.116841137f, 	0.5f, 1.0f,
		1.0f, 0.0f, 0.0f,		-0.993150651f, 0.0f, -0.116841137f, 	0.0f, 0.0f
	};

	// Total float values per each type
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// Store vertex and index count
	mesh.numVertices = sizeof(verts) / sizeof(verts[0] * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.numIndices = 0;

	// Create VAO

	glGenVertexArrays(numVAOs, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);

	// Create VBOs
	glGenBuffers(numVBOs, mesh.vbo);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU

	// Strides
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);


}
void Meshes::CreateTaperedPolygonMesh(GLmesh& mesh) {

	// Specifies Normalized Device Coordinates for triangle vertices
GLfloat verts[] = // vertexPositions in the book
{
	// Vertex				//Normals				//Texture
	// Tapered Polygon's Rear side right triangle

	-0.75f, 1.0f, -0.75f,	-0.699854212f, -0.610435009f, 0.371390669f,			1.0f, 1.0f,// top left corner 
	0.75f, 1.0f, -0.75f,	-0.699854212f, -0.610435009f, 0.371390669f,		0.0f, 1.0f,	// top right corner
	1.0f, -1.0f, -1.0f,		-0.699854212, -0.610435009, 0.371390669,	0.0f, 0.0f,	// bottom right corner

	// Tapered Polygon's Rear side left triangle
	-0.75f, 1.0f, -0.75f,	-0.699854212f, -0.610435009f, 0.371390669f,		1.0f, 1.0f,	// top left corner
	1.0f, -1.0f, -1.0f,		-0.699854212f, -0.610435009f, 0.371390669f,			0.0f, 0.0f,	// bottom right corner
	-1.0f, -1.0f, -1.0f,	-0.699854212f, -0.610435009f, 0.371390669f,			1.0f, 0.0f,	// bottom left corner

	// Tapered Polygon's Right side right triangle
	1.0f, -1.0f, -1.0f,		0.894427191, 0.0, 0.447213595,		1.0f, 0.0f,	// bottom right corner
	0.75f, 1.0f, -0.75f,	0.894427191, 0.0, 0.447213595,		1.0f, 1.0f,	// top right corner
	1.0f, -1.0f, 1.0f,		0.894427191, 0.0, 0.447213595,		0.0f, 0.0f,	// bottom left corner

	// Tapered Polygon's Right side left triangle
	0.75f, 1.0f, -0.75f,	0.894427191, 0.0, 0.447213595,		1.0f, 1.0f,	// top right corner
	0.75f, 1.0f, 0.75f,		0.894427191, 0.0, 0.447213595,		0.0f, 1.0f,	// top left corner
	1.0f, -1.0f, 1.0f,		0.894427191, 0.0, 0.447213595,		0.0f, 0.0f,	// bottom left corner

	// Tapered Polygon's Front side right triangle
	0.75f, 1.0f, 0.75f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,	// top right corner
	-1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,	// bottom left corner
	1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,	// bottom right corner

	// Tapered Polygon's Front side left triangle
	0.75f, 1.0f, 0.75f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,	// top right corner
	-0.75f, 1.0f, 0.75f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,	// top left corner
	-1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,	// bottom left corner

	// Tapered Polygon's Left side right triangle
	-0.75f, 1.0f, 0.75f,	-0.894427191, 0.0, 0.447213595,		1.0f, 1.0f,	// top right corner
	-1.0f, -1.0f, -1.0f,	-0.894427191, 0.0, 0.447213595,		0.0f, 0.0f,	// bottom left corner
	-1.0f, -1.0f, 1.0f,		-0.894427191, 0.0, 0.447213595,		1.0f, 0.0f,	// bottom right corner

	// Tapered Polygon's Left side left triangle
	-0.75f, 1.0f, 0.75f,	-0.894427191, 0.0, 0.447213595,			1.0f, 1.0f,	// top right corner
	-0.75f, 1.0f, -0.75f,	-0.894427191, 0.0, 0.447213595,			0.0f, 1.0f,	// top left corner
	-1.0f, -1.0f, -1.0f,	-0.894427191, 0.0, 0.447213595,			0.0f, 0.0f,	// bottom left corner

	// Tapered Polygon's Bottom side right triangle (facing viewer)
	1.0f, -1.0f, -1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,	// bottom right corner
	1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 1.0f,	// top right corner
	-1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,	// top left corner

	// Tapered Polygon's Bottom side left triangle (facing viewer)
	-1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,	// top left corner
	-1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f,	// bottom left corner
	1.0f, -1.0f, -1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,	// bottom right corner

	// Tapered Polygon's Top side right triangle (facing viewer)
	0.75f, 1.0f, -0.75f,	0.0f, 1.0f, 0.0f,		1.0f, 1.0f,	// top right corner
	-0.75f, 1.0f, -0.75f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,	// top left corner
	0.75f, 1.0f, 0.75f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,// bottom right corner

	// Tapered Polygon's Top side left triangle (facing viewer)
	-0.75f, 1.0f, -0.75f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,	// top left corner
	-0.75f, 1.0f, 0.75f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,	// bottom left corner
	0.75f, 1.0f, 0.75f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,	// bottom right corner

};

	glGenVertexArrays(numVAOs, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU

	// Float values per type
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 0; // (r, g, b, a) THIS IS SET TO 0 FOR NOW
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// Store the vertex and index count inside the mesh
	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV));

	// Strides between vertex coordinates is 3 (xyz rgba)
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV);

	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void Meshes::CreatePyramid3Mesh(GLmesh& mesh) {

	// Vertex data
	GLfloat verts[]{
		// Vertex Positions		// Normals			// Texture coords
		//left side
		0.0f, 0.5f, 0.0f,		-0.894427180f, 0.0f, -0.447213590f,	0.5f, 1.0f,		//top point	
		0.0f, -0.5f, -0.5f,		-0.894427180f, 0.0f, -0.447213590f,	0.0f, 0.0f,		//back center
		-0.5f, -0.5f, 0.5f,		-0.894427180f, 0.0f, -0.447213590f,	1.0f, 0.0f,     //front bottom left
		0.0f, 0.5f, 0.0f,		-0.894427180f, 0.0f, -0.447213590f,	0.5f, 1.0f,		//top point	
		//right side
		0.0f, 0.5f, 0.0f,		0.894427180f, 0.0f, -0.447213590f,	0.5f, 1.0f,		//top point	
		0.5f, -0.5f, 0.5f,		0.894427180f, 0.0f, -0.447213590f,	0.0f, 0.0f,     //front bottom right
		0.0f, -0.5f, -0.5f,		0.894427180f, 0.0f, -0.447213590f,	1.0f, 0.0f,		//back center	
		0.0f, 0.5f, 0.0f,		0.894427180f, 0.0f, -0.447213590f,	0.5f, 1.0f,		//top point	
		//front side
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.5f, 1.0f,		//top point			
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,     //front bottom left	
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,     //front bottom right
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.5f, 1.0f,		//top point	
		//bottom side
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,     //front bottom left
		0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f,     //front bottom right
		0.0f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,		//back center	
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,     //front bottom left
	};

	// Float values per type
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 3; // (r, g, b)
	const GLuint floatsPerUV = 2;

	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerUV));

	glGenVertexArrays(numVAOs, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerUV);

	// Instructs the GPU on how to handle the vbo data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, ie x and y | data type | deactivate normalization | 0
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// Parameters: attribPointerPosition 1 | floats per color is temporarily 0, ie rgba | data type | deactivate normalization 
	// | 0 strides till next color | 2 floats til beginning of each color
	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerColor)));
	glEnableVertexAttribArray(2);

}

void Meshes::CreatePyramid4Mesh(GLmesh& mesh) {
	
	GLfloat verts[] {
		// Pyramid positions 5 vertices, 6 triangles
		// Vertex Positions		// Normals				// Texture coords
			//bottom side
		- 0.5f, -0.5f, 0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,     //front bottom left
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f,		//back bottom left
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,		//back bottom right	
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,     //front bottom left
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,     //front bottom left
		0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 1.0f,     //front bottom right
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,		//back bottom right	
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,     //front bottom left
		//back side
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, -1.0f,		0.5f, 1.0f,		//top point	
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f,		//back bottom right	
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f,		//back bottom left
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, -1.0f,		0.5f, 1.0f,		//top point	
		//left side
		0.0f, 0.5f, 0.0f,		-1.0f, 0.0f, 0.0f,		0.5f, 1.0f,		//top point	
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		//back bottom left	
		-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,     //front bottom left
		0.0f, 0.5f, 0.0f,		-1.0f, 0.0f, 0.0f,		0.5f, 1.0f,		//top point	
		//right side
		0.0f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		0.5f, 1.0f,		//top point	
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,     //front bottom right
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,		//back bottom right	
		0.0f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		0.5f, 1.0f,		//top point	
		//front side
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.5f, 1.0f,		//top point			
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,     //front bottom left	
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,     //front bottom right
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.5f, 1.0f,		//top point
};


	glGenVertexArrays(numVAOs, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU



	// Float values per type
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 0; // (r, g, b, a) THIS IS SET TO 0 FOR NOW
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2; // Texture coordinates

	// Store the vertex and index count inside the mesh
	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV));
	//mesh.numIndices = sizeof(indices) / sizeof(indices[0]);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates is 3 (xyz rgba)
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV);

	// Instructs the GPU on how to handle the vbo data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, ie x and y | data type | deactivate normalization | 0
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// Parameters: attribPointerPosition 1 | floats per color is temporarily 0, ie rgba | data type | deactivate normalization 
	// | 0 strides till next color | 2 floats til beginning of each color
	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void Meshes::CreateSphereMesh(GLmesh& mesh) {

	static Sphere sphere;
	std::vector<int> ind = sphere.getIndices();
	std::vector<glm::vec3> vert = sphere.getVertices();
	std::vector<glm::vec2> tex = sphere.getTexCoords();
	std::vector<glm::vec3> norm = sphere.getNormals();

	std::vector<float> pvalues; // vertex positions
	std::vector<float> tvalues; // texture coordinates
	std::vector<float> nvalues;// normal vectors

	int numIndices = sphere.getNumIndices();
	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vert[ind[i]].x));
		pvalues.push_back((vert[ind[i]].y));
		pvalues.push_back((vert[ind[i]].z));

		tvalues.push_back((tex[ind[i]].s));
		tvalues.push_back((tex[ind[i]].t));

		nvalues.push_back((norm[ind[i]].x));
		nvalues.push_back((norm[ind[i]].y));
		nvalues.push_back((norm[ind[i]].z));
	}

	mesh.numVertices = sphere.getNumVertices();
	mesh.numIndices = sphere.getNumIndices();
	glGenVertexArrays(numVAOs, &mesh.vao);
	glBindVertexArray(mesh.vao);
	glGenBuffers(numVBOs, mesh.vbo);

	// put vertices into buffer 0
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO
	glEnableVertexAttribArray(0);

	// put normals into buffer #1
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO
	glEnableVertexAttribArray(1);

	// put texture coordinates into buffer #2
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO
	glEnableVertexAttribArray(2);

}

void Meshes::CreateTaperedCylinderMesh(GLmesh& mesh)
{
	GLfloat verts[] = {
		// cylinder bottom		// normals			// texture coords
		1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f,1.0f,
		.98f, 0.0f, -0.17f,		0.0f, -1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 0.0f, -0.34f,		0.0f, -1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 0.0f, -0.64f,		0.0f, -1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 0.0f, -0.77f,		0.0f, -1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 0.0f, -0.94f,		0.0f, -1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 0.0f, -0.98f,		0.0f, -1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 0.0f, -0.98f,	0.0f, -1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 0.0f, -0.94f,	0.0f, -1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 0.0f, -0.77f,	0.0f, -1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 0.0f, -0.64f,	0.0f, -1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 0.0f, -0.34f,	0.0f, -1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 0.0f, -0.17f,	0.0f, -1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder top			// normals			// texture coords
		0.5f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f,1.0f,
		.49f, 1.0f, -0.085f,	0.0f, 1.0f, 0.0f,	0.41f, 0.983f,
		.47f, 1.0f, -0.17f,		0.0f, 1.0f, 0.0f,	0.33f, 0.96f,
		.435f, 1.0f, -0.25f,	0.0f, 1.0f, 0.0f,	0.25f, 0.92f,
		.385f, 1.0f, -0.32f,	0.0f, 1.0f, 0.0f,	0.17f, 0.87f,
		.32f, 1.0f, -0.385f,	0.0f, 1.0f, 0.0f,	0.13f, 0.83f,
		.25f, 1.0f, -0.435f,	0.0f, 1.0f, 0.0f,	0.08f, 0.77f,
		.17f, 1.0f, -0.47f,		0.0f, 1.0f, 0.0f,	0.04f, 0.68f,
		.085f, 1.0f, -0.49f,	0.0f, 1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 1.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f,0.5f,
		-.085f, 1.0f, -0.49f,	0.0f, 1.0f, 0.0f,	0.017f, 0.41f,
		-.17f, 1.0f, -0.47f,	0.0f, 1.0f, 0.0f,	0.04f, 0.33f,
		-.25f, 1.0f, -0.435f,	0.0f, 1.0f, 0.0f,	0.08f, 0.25f,
		-.32f, 1.0f, -0.385f,	0.0f, 1.0f, 0.0f,	0.13f, 0.17f,
		-.385f, 1.0f, -0.32f,	0.0f, 1.0f, 0.0f,	0.17f, 0.13f,
		-.435f, 1.0f, -0.25f,	0.0f, 1.0f, 0.0f,	0.25f, 0.08f,
		-.47f, 1.0f, -0.17f,	0.0f, 1.0f, 0.0f,	0.33f, 0.04f,
		-.49f, 1.0f, -0.085f,	0.0f, 1.0f, 0.0f,	0.41f, 0.017f,
		-0.5f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f, 0.0f,
		-.49f, 1.0f, 0.085f,	0.0f, 1.0f, 0.0f,	0.6f, 0.017f,
		-.47f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.68f, 0.04f,
		-.435f, 1.0f, 0.25f,	0.0f, 1.0f, 0.0f,	0.77f, 0.08f,
		-.385f, 1.0f, 0.32f,	0.0f, 1.0f, 0.0f,	0.83f, 0.13f,
		-.32f, 1.0f, 0.385f,	0.0f, 1.0f, 0.0f,	0.87f, 0.17f,
		-.25f, 1.0f, 0.435f,	0.0f, 1.0f, 0.0f,	0.92f, 0.25f,
		-.17f, 1.0f, 0.47f,		0.0f, 1.0f, 0.0f,	0.96f, 0.33f,
		-.085f, 1.0f, 0.49f,	0.0f, 1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.5f,
		.085f, 1.0f, 0.49f,		0.0f, 1.0f, 0.0f,	0.983f, 0.6f,
		.17f, 1.0f, 0.47f,		0.0f, 1.0f, 0.0f,	0.96f, 0.68f,
		.25f, 1.0f, 0.435f,		0.0f, 1.0f, 0.0f,	0.92f, 0.77f,
		.32f, 1.0f, 0.385f,		0.0f, 1.0f, 0.0f,	0.87f, 0.83f,
		.385f, 1.0f, 0.32f,		0.0f, 1.0f, 0.0f,	0.83f, 0.87f,
		.435f, 1.0f, 0.25f,		0.0f, 1.0f, 0.0f,	0.77f, 0.92f,
		.47f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.68f, 0.96f,
		.49f, 1.0f, 0.085f,		0.0f, 1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder body		// normals				// texture coords
		0.5f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.25,1.0,
		1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0,0.0,
		.98f, 0.0f, -0.17f,		1.0f, 0.0f, 0.0f,		0.0277,0.0,
		0.5f, 1.0f, 0.0f,		0.92f, 0.0f, -0.08f,	0.25,1.0,
		.49f, 1.0f, -0.085f,	0.92f, 0.0f, -0.08f,	0.2635,1.0,
		.98f, 0.0f, -0.17f,		0.92f, 0.0f, -0.08f,	0.0277,0.0,
		.94f, 0.0f, -0.34f,		0.83f, 0.0f, -0.17f,	0.0554,0.0,
		.49f, 1.0f, -0.085f,	0.83f, 0.0f, -0.17f,	0.2635,1.0,
		.47f, 1.0f, -0.17f,		0.83f, 0.0f, -0.17f,	0.277,1.0,
		.94f, 0.0f, -0.34f,		0.75f, 0.0f, -0.25f,	0.0554,0.0,
		.87f, 0.0f, -0.5f,		0.75f, 0.0f, -0.25f,	0.0831,0.0,
		.47f, 1.0f, -0.17f,		0.75f, 0.0f, -0.25f,	0.277,1.0,
		.435f, 1.0f, -0.25f,	0.67f, 0.0f, -0.33f,	0.2905,1.0,
		.87f, 0.0f, -0.5f,		0.67f, 0.0f, -0.33f,	0.0831,0.0,
		.77f, 0.0f, -0.64f,		0.67f, 0.0f, -0.33f,	0.1108,0.0,
		.435f, 1.0f, -0.25f,	0.58f, 0.0f, -0.42f,	0.2905,1.0,
		.385f, 1.0f, -0.32f,	0.58f, 0.0f, -0.42f,	0.304,1.0,
		.77f, 0.0f, -0.64f,		0.58f, 0.0f, -0.42f,	0.1108,0.0,
		.64f, 0.0f, -0.77f,		0.5f, 0.0f, -0.5f,		0.1385,0.0,
		.385f, 1.0f, -0.32f,	0.5f, 0.0f, -0.5f,		0.304,1.0,
		.32f, 1.0f, -0.385f,	0.5f, 0.0f, -0.5f,		0.3175,1.0,
		.64f, 0.0f, -0.77f,		0.42f, 0.0f, -0.58f,	0.1385,0.0,
		.5f, 0.0f, -0.87f,		0.42f, 0.0f, -0.58f,	0.1662,0.0,
		.32f, 1.0f, -0.385f,	0.42f, 0.0f, -0.58f,	0.3175, 1.0,
		.25f, 1.0f, -0.435f,	0.33f, 0.0f, -0.67f,	0.331, 1.0,
		.5f, 0.0f, -0.87f,		0.33f, 0.0f, -0.67f,	0.1662, 0.0,
		.34f, 0.0f, -0.94f,		0.33f, 0.0f, -0.67f,	0.1939, 0.0,
		.25f, 1.0f, -0.435f,	0.25f, 0.0f, -0.75f,	0.331, 1.0,
		.17f, 1.0f, -0.47f,		0.25f, 0.0f, -0.75f,	0.3445, 1.0,
		.34f, 0.0f, -0.94f,		0.25f, 0.0f, -0.75f,	0.1939, 0.0,
		.17f, 0.0f, -0.98f,		0.17f, 0.0f, -0.83f,	0.2216, 0.0,
		.17f, 1.0f, -0.47f,		0.17f, 0.0f, -0.83f,	0.3445, 1.0,
		.085f, 1.0f, -0.49f,	0.17f, 0.0f, -0.83f,	0.358, 1.0,
		.17f, 0.0f, -0.98f,		0.08f, 0.0f, -0.92f,	0.2216, 0.0,
		0.0f, 0.0f, -1.0f,		0.08f, 0.0f, -0.92f,	0.2493, 0.0,
		.085f, 1.0f, -0.49f,	0.08f, 0.0f, -0.92f,	0.358, 1.0,
		0.0f, 1.0f, -0.5f,		0.0f, 0.0f, -1.0f,		0.3715, 1.0,
		0.0f, 0.0f, -1.0f,		0.0f, 0.0f, -1.0f,		0.2493, 0.0,
		-.17f, 0.0f, -0.98f,	0.0f, 0.0f, -1.0f,		0.277, 0.0,
		0.0f, 1.0f, -0.5f,		0.08f, 0.0f, -1.08f,	0.3715, 1.0,
		-.085f, 1.0f, -0.49f,	-0.08f, 0.0f, -0.92f,	0.385, 1.0,
		-.17f, 0.0f, -0.98f,	-0.08f, 0.0f, -0.92f,	0.277, 0.0,
		-.34f, 0.0f, -0.94f,	-0.08f, 0.0f, -0.92f,	0.3047, 0.0,
		-.085f, 1.0f, -0.49f,	-0.08f, 0.0f, -0.92f,	0.385, 1.0,
		-.17f, 1.0f, -0.47f,	-0.17f, 0.0f, -0.83f,	0.3985, 1.0,
		-.34f, 0.0f, -0.94f,	-0.17f, 0.0f, -0.83f,	0.3047, 0.0,
		-.5f, 0.0f, -0.87f,		-0.17f, 0.0f, -0.83f,	0.3324, 0.0,
		-.17f, 1.0f, -0.47f,	-0.25f, 0.0f, -0.75f,	0.3985, 1.0,
		-.25f, 1.0f, -0.435f,	-0.25f, 0.0f, -0.75f,	0.412, 1.0,
		-.5f, 0.0f, -0.87f,		-0.25f, 0.0f, -0.75f,	0.3324, 0.0,
		-.64f, 0.0f, -0.77f,	-0.33f, 0.0f, -0.67f,	0.3601, 0.0,
		-.25f, 1.0f, -0.435f,	-0.33f, 0.0f, -0.67f,	0.412, 1.0,
		-.32f, 1.0f, -0.385f,	-0.33f, 0.0f, -0.67f,	0.4255, 1.0,
		-.64f, 0.0f, -0.77f,	-0.42f, 0.0f, -0.58f,	0.3601, 0.0,
		-.77f, 0.0f, -0.64f,	-0.42f, 0.0f, -0.58f,	0.3878, 0.0,
		-.32f, 1.0f, -0.385f,	-0.42f, 0.0f, -0.58f,	0.4255, 1.0,
		-.385f, 1.0f, -0.32f,	-0.5f, 0.0f, -0.5f,		0.439, 1.0,
		-.77f, 0.0f, -0.64f,	-0.5f, 0.0f, -0.5f,		0.3878, 0.0,
		-.87f, 0.0f, -0.5f,		-0.5f, 0.0f, -0.5f,		0.4155, 0.0,
		-.385f, 1.0f, -0.32f,	-0.58f, 0.0f, -0.42f,	0.439, 1.0,
		-.435f, 1.0f, -0.25f,	-0.58f, 0.0f, -0.42f,	0.4525, 1.0,
		-.87f, 0.0f, -0.5f,		-0.58f, 0.0f, -0.42f,	0.4155, 0.0,
		-.94f, 0.0f, -0.34f,	-0.67f, 0.0f, -0.33f,	0.4432, 0.0,
		-.435f, 1.0f, -0.25f,	-0.67f, 0.0f, -0.33f,	0.4525, 1.0,
		-.47f, 1.0f, -0.17f,	-0.67f, 0.0f, -0.33f,	0.466, 1.0,
		-.94f, 0.0f, -0.34f,	-0.75f, 0.0f, -0.25f,	0.4432, 0.0,
		-.98f, 0.0f, -0.17f,	-0.75f, 0.0f, -0.25f,	0.4709, 0.0,
		-.47f, 1.0f, -0.17f,	-0.75f, 0.0f, -0.25f,	0.466, 1.0,
		-.49f, 1.0f, -0.085f,	-0.83f, 0.0f, -0.17f,	0.4795, 1.0,
		-.98f, 0.0f, -0.17f,	-0.83f, 0.0f, -0.17f,	0.4709, 0.0,
		-1.0f, 0.0f, 0.0f,		-0.83f, 0.0f, -0.17f,	0.4986, 0.0,
		-.49f, 1.0f, -0.085f,	-0.92f, 0.0f, -0.08f,	0.4795, 1.0,
		-0.5f, 1.0f, 0.0f,		-0.92f, 0.0f, -0.08f,	0.493, 1.0,
		-1.0f, 0.0f, 0.0f,		-0.92f, 0.0f, -0.08f,	0.4986, 0.0,
		-.98f, 0.0f, 0.17f,		-1.0f, 0.0f, 0.0f,		0.5263, 0.0,
		-0.5f, 1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,		0.493, 1.0,
		-.49f, 1.0f, 0.085f,	-1.0f, 0.0f, 0.0f,		0.5065, 1.0,
		-.98f, 0.0f, 0.17f,		-0.92f, 0.0f, 0.08f,	0.5263, 0.0,
		-.94f, 0.0f, 0.34f,		-0.92f, 0.0f, 0.08f,	0.554, 0.0,
		-.49f, 1.0f, 0.085f,	-0.92f, 0.0f, 0.08f,	0.5065, 1.0,
		-.47f, 1.0f, 0.17f,		-0.83f, 0.0f, 0.17f,	0.52, 1.0,
		-.94f, 0.0f, 0.34f,		-0.83f, 0.0f, 0.17f,	0.554, 0.0,
		-.87f, 0.0f, 0.5f,		-0.83f, 0.0f, 0.17f,	0.5817, 0.0,
		-.47f, 1.0f, 0.17f,		-0.75f, 0.0f, 0.25f,	0.52, 1.0,
		-.435f, 1.0f, 0.25f,	-0.75f, 0.0f, 0.25f,	0.5335, 1.0,
		-.87f, 0.0f, 0.5f,		-0.75f, 0.0f, 0.25f,	0.5817, 0.0,
		-.77f, 0.0f, 0.64f,		-0.67f, 0.0f, 0.33f,	0.6094, 0.0,
		-.435f, 1.0f, 0.25f,	-0.67f, 0.0f, 0.33f,	0.5335, 1.0,
		-.385f, 1.0f, 0.32f,	-0.67f, 0.0f, 0.33f,	0.547, 1.0,
		-.77f, 0.0f, 0.64f,		-0.58f, 0.0f, 0.42f,	0.6094, 0.0,
		-.64f, 0.0f, 0.77f,		-0.58f, 0.0f, 0.42f,	0.6371, 0.0,
		-.385f, 1.0f, 0.32f,	-0.58f, 0.0f, 0.42f,	0.547, 1.0,
		-.32f, 1.0f, 0.385f,	-0.5f, 0.0f, 0.5f,		0.5605, 1.0,
		-.64f, 0.0f, 0.77f,		-0.5f, 0.0f, 0.5f,		0.6371, 0.0,
		-.5f, 0.0f, 0.87f,		-0.5f, 0.0f, 0.5f,		0.6648, 0.0,
		-.32f, 1.0f, 0.385f,	-0.42f, 0.0f, 0.58f,	0.5605, 1.0,
		-.25f, 1.0f, 0.435f,	-0.42f, 0.0f, 0.58f,	0.574, 1.0,
		-.5f, 0.0f, 0.87f,		-0.42f, 0.0f, 0.58f,	0.6648, 0.0,
		-.34f, 0.0f, 0.94f,		-0.33f, 0.0f, 0.67f,	0.6925, 0.0,
		-.25f, 1.0f, 0.435f,	-0.33f, 0.0f, 0.67f,	0.574, 1.0,
		-.17f, 1.0f, 0.47f,		-0.33f, 0.0f, 0.67f,	0.5875, 1.0,
		-.34f, 0.0f, 0.94f,		-0.25f, 0.0f, 0.75f,	0.6925, 0.0,
		-.17f, 0.0f, 0.98f,		-0.25f, 0.0f, 0.75f,	0.7202, 0.0,
		-.17f, 1.0f, 0.47f,		-0.25f, 0.0f, 0.75f,	0.5875, 1.0,
		-.085f, 1.0f, 0.49f,	-0.17f, 0.0f, 0.83f,	0.601, 1.0,
		-.17f, 0.0f, 0.98f,		-0.17f, 0.0f, 0.83f,	0.7202, 0.0,
		0.0f, 0.0f, 1.0f,		-0.17f, 0.0f, 0.83f,	0.7479, 0.0,
		-.085f, 1.0f, 0.49f,	-0.08f, 0.0f, 0.92f,	0.601, 1.0,
		0.0f, 1.0f, 0.5f,		-0.08f, 0.0f, 0.92f,	0.6145, 1.0,
		0.0f, 0.0f, 1.0f,		-0.08f, 0.0f, 0.92f,	0.7479, 0.0,
		.17f, 0.0f, 0.98f,		-0.0f, 0.0f, 1.0f,		0.7756, 0.0,
		0.0f, 1.0f, 0.5f,		-0.0f, 0.0f, 1.0f,		0.6145, 1.0,
		.085f, 1.0f, 0.49f,		-0.0f, 0.0f, 1.0f,		0.628, 1.0,
		.17f, 0.0f, 0.98f,		0.08f, 0.0f, 0.92f,		0.7756, 0.0,
		.34f, 0.0f, 0.94f,		0.08f, 0.0f, 0.92f,		0.8033, 0.0,
		.085f, 1.0f, 0.49f,		0.08f, 0.0f, 0.92f,		0.628, 1.0,
		.17f, 1.0f, 0.47f,		0.17f, 0.0f, 0.83f,		0.6415, 1.0,
		.34f, 0.0f, 0.94f,		0.17f, 0.0f, 0.83f,		0.8033, 0.0,
		.5f, 0.0f, 0.87f,		0.17f, 0.0f, 0.83f,		0.831, 0.0,
		.17f, 1.0f, 0.47f,		0.25f, 0.0f, 0.75f,		0.6415, 1.0,
		.25f, 1.0f, 0.435f,		0.25f, 0.0f, 0.75f,		0.655, 1.0,
		.5f, 0.0f, 0.87f,		0.25f, 0.0f, 0.75f,		0.831, 0.0,
		.64f, 0.0f, 0.77f,		0.33f, 0.0f, 0.67f,		0.8587, 0.0,
		.25f, 1.0f, 0.435f,		0.33f, 0.0f, 0.67f,		0.655, 1.0,
		.32f, 1.0f, 0.385f,		0.33f, 0.0f, 0.67f,		0.6685, 1.0,
		.64f, 0.0f, 0.77f,		0.42f, 0.0f, 0.58f,		0.8587, 0.0,
		.77f, 0.0f, 0.64f,		0.42f, 0.0f, 0.58f,		0.8864, 0.0,
		.32f, 1.0f, 0.385f,		0.42f, 0.0f, 0.58f,		0.6685, 1.0,
		.385f, 1.0f, 0.32f,		0.5f, 0.0f, 0.5f,		0.682, 1.0,
		.77f, 0.0f, 0.64f,		0.5f, 0.0f, 0.5f,		0.8864, 0.0,
		.87f, 0.0f, 0.5f,		0.5f, 0.0f, 0.5f,		0.9141, 0.0,
		.385f, 1.0f, 0.32f,		0.58f, 0.0f, 0.42f,		0.682, 1.0,
		.435f, 1.0f, 0.25f,		0.58f, 0.0f, 0.42f,		0.6955, 1.0,
		.87f, 0.0f, 0.5f,		0.58f, 0.0f, 0.42f,		0.9141, 0.0,
		.94f, 0.0f, 0.34f,		0.67f, 0.0f, 0.33f,		0.9418, 0.0,
		.435f, 1.0f, 0.25f,		0.67f, 0.0f, 0.33f,		0.6955, 1.0,
		.47f, 1.0f, 0.17f,		0.67f, 0.0f, 0.33f,		0.709, 1.0,
		.94f, 0.0f, 0.34f,		0.75f, 0.0f, 0.25f,		0.9418, 0.0,
		.98f, 0.0f, 0.17f,		0.75f, 0.0f, 0.25f,		0.9695, 0.0,
		.47f, 1.0f, 0.17f,		0.75f, 0.0f, 0.25f,		0.709, 0.0,
		.49f, 1.0f, 0.085f,		0.83f, 0.0f, 0.17f,		0.7225, 1.0,
		.98f, 0.0f, 0.17f,		0.83f, 0.0f, 0.17f,		0.9695, 0.0,
		1.0f, 0.0f, 0.0f,		0.83f, 0.0f, 0.17f,		1.0, 0.0,
		.49f, 1.0f, 0.085f,		0.92f, 0.0f, 0.08f,		0.7225, 1.0,
		0.5f, 1.0f, 0.0f,		0.92f, 0.0f, 0.08f,		0.75, 1.0,
		1.0f, 0.0f, 0.0f,		0.92f, 0.0f, 0.08f,		1.0, 0.0
	};

	// total float values per each type
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// store vertex and index count
	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.numIndices = 0;

	// Create VAO
	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create VBO
	glGenBuffers(1, mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void Meshes::CreateWingMesh(GLmesh& mesh) {

	// Specifies Normalized Device Coordinates for triangle vertices
	GLfloat verts[] = // vertexPositions in the book
	{
		// Vertex				//Normals				//Texture
		// Tapered Polygon's Rear side left triangle
		-0.5f, 1.0f, -0.5f,	-0.699854212f, -0.610435009f, 0.371390669f,		1.0f, 1.0f,	// top left corner
		-0.5f, 0.0f, -1.0f,	-0.699854212f, -0.610435009f, 0.371390669f,			1.0f, 0.0f,	// bottom left corner
		0.5, 0.0f, -1.0f,		-0.699854212f, -0.610435009f, 0.371390669f,			0.0f, 0.0f,	// bottom right corner

		// Tapered Polygon's Rear side right triangle
		0.5f, 0.0f, -1.0f,		-0.699854212, -0.610435009, 0.371390669,	0.0f, 0.0f,	// bottom right corner
		0.5f, 1.0f, -0.5f,	-0.699854212f, -0.610435009f, 0.371390669f,		0.0f, 1.0f,	// top right corner
		-0.5f, 1.0f, -0.5f,	-0.699854212f, -0.610435009f, 0.371390669f,			1.0f, 1.0f,// top left corner 

		// Tapered Polygon's Right side right triangle
		0.5f, 0.0f, -1.0f,		0.894427191, 0.0, 0.447213595,		1.0f, 0.0f,	// bottom right corner
		0.5f, 0.0f, 1.0f,		0.894427191, 0.0, 0.447213595,		0.0f, 0.0f,	// bottom left corner
		0.5f, 1.0f, -0.5f,	0.894427191, 0.0, 0.447213595,		1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Right side left triangle
		0.5f, 0.0f, 1.0f,		0.894427191, 0.0, 0.447213595,		0.0f, 0.0f,	// bottom left corner
		0.5f, 1.0f, 0.5f,		0.894427191, 0.0, 0.447213595,		0.0f, 1.0f,	// top left corner
		0.5f, 1.0f, -0.5f,	0.894427191, 0.0, 0.447213595,		1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Front side right triangle
		0.5f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,	// bottom right corner
		-0.5f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,	// bottom left corner
		0.5f, 1.0f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Front side left triangle
		-0.5f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,	// bottom left corner
		-0.5f, 1.0f, 0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,	// top left corner
		0.5f, 1.0f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Left side right triangle
		-0.5f, 0.0f, 1.0f,		-0.894427191, 0.0, 0.447213595,		1.0f, 0.0f,	// bottom right corner
		-0.5f, 0.0f, -1.0f,	-0.894427191, 0.0, 0.447213595,		0.0f, 0.0f,	// bottom left corner
		-0.5f, 1.0f, 0.5f,	-0.894427191, 0.0, 0.447213595,		1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Left side left triangle
		-0.5f, 0.0f, -1.0f,	-0.894427191, 0.0, 0.447213595,			0.0f, 0.0f,	// bottom left corner
		-0.5f, 1.0f, -0.5f,	-0.894427191, 0.0, 0.447213595,			0.0f, 1.0f,	// top left corner
		-0.5f, 1.0f, 0.5f,	-0.894427191, 0.0, 0.447213595,			1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Bottom side right triangle (facing viewer)
		-0.5f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,	// top left corner
		0.5f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 1.0f,	// top right corner
		0.5f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,	// bottom right corner

		// Tapered Polygon's Bottom side left triangle (facing viewer)
		0.5f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,	// bottom right corner
		-0.5f, 0.0f, -1.0f,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f,	// bottom left corner
		-0.5f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,	// top left corner

		// Tapered Polygon's Top side right triangle (facing viewer)
		-0.5f, 1.0f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,	// top left corner
		0.5f, 1.0f, -0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 1.0f,	// top right corner
		0.5f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,// bottom right corner

		// Tapered Polygon's Top side left triangle (facing viewer)
		0.5f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,	// bottom right corner
		-0.5f, 1.0f, 0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,	// bottom left corner
		-0.5f, 1.0f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,	// top left corner

	//// SYMMETRICAL BOTTOM SIDE OF MESH

				// Vertex				//Normals				//Texture
		// Tapered Polygon's Rear side left triangle
		-0.5f, -1.0f, -0.5f,	-0.699854212f, -0.610435009f, 0.371390669f,		1.0f, 1.0f,	// top left corner
		-0.5f, 0.0f, -1.0f,	-0.699854212f, -0.610435009f, 0.371390669f,			1.0f, 0.0f,	// bottom left corner
		0.5, 0.0f, -1.0f,		-0.699854212f, -0.610435009f, 0.371390669f,			0.0f, 0.0f,	// bottom right corner

		// Tapered Polygon's Rear side right triangle
		0.5f, 0.0f, -1.0f,		-0.699854212, -0.610435009, 0.371390669,	0.0f, 0.0f,	// bottom right corner
		0.5f, -1.0f, -0.5f,	-0.699854212f, -0.610435009f, 0.371390669f,		0.0f, 1.0f,	// top right corner
		-0.5f, -1.0f, -0.5f,	-0.699854212f, -0.610435009f, 0.371390669f,			1.0f, 1.0f,// top left corner 

		// Tapered Polygon's Right side right triangle
		0.5f, 0.0f, -1.0f,		0.894427191, 0.0, 0.447213595,		1.0f, 0.0f,	// bottom right corner
		0.5f, 0.0f, 1.0f,		0.894427191, 0.0, 0.447213595,		0.0f, 0.0f,	// bottom left corner
		0.5f, -1.0f, -0.5f,	0.894427191, 0.0, 0.447213595,		1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Right side left triangle
		0.5f, 0.0f, 1.0f,		0.894427191, 0.0, 0.447213595,		0.0f, 0.0f,	// bottom left corner
		0.5f, -1.0f, 0.5f,		0.894427191, 0.0, 0.447213595,		0.0f, 1.0f,	// top left corner
		0.5f, -1.0f, -0.5f,	0.894427191, 0.0, 0.447213595,		1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Front side right triangle
		0.5f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,	// bottom right corner
		-0.5f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,	// bottom left corner
		0.5f, -1.0f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Front side left triangle
		-0.5f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,	// bottom left corner
		-0.5f, -1.0f, 0.5f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,	// top left corner
		0.5f, -1.0f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Left side right triangle
		-0.5f, 0.0f, 1.0f,		-0.894427191, 0.0, 0.447213595,		1.0f, 0.0f,	// bottom right corner
		-0.5f, 0.0f, -1.0f,	-0.894427191, 0.0, 0.447213595,		0.0f, 0.0f,	// bottom left corner
		-0.5f, -1.0f, 0.5f,	-0.894427191, 0.0, 0.447213595,		1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Left side left triangle
		-0.5f, 0.0f, -1.0f,	-0.894427191, 0.0, 0.447213595,			0.0f, 0.0f,	// bottom left corner
		-0.5f, -1.0f, -0.5f,	-0.894427191, 0.0, 0.447213595,			0.0f, 1.0f,	// top left corner
		-0.5f, -1.0f, 0.5f,	-0.894427191, 0.0, 0.447213595,			1.0f, 1.0f,	// top right corner

		// Tapered Polygon's Bottom side right triangle (facing viewer)
		-0.5f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,	// top left corner
		0.5f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 1.0f,	// top right corner
		0.5f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,	// bottom right corner

		// Tapered Polygon's Bottom side left triangle (facing viewer)
		0.5f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,	// bottom right corner
		-0.5f, 0.0f, -1.0f,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f,	// bottom left corner
		-0.5f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,		0.0f, 1.0f,	// top left corner

		// Tapered Polygon's Top side right triangle (facing viewer)
		-0.5f, -1.0f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,	// top left corner
		0.5f, -1.0f, -0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 1.0f,	// top right corner
		0.5f, -1.0f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,// bottom right corner

		// Tapered Polygon's Top side left triangle (facing viewer)
		0.5f, -1.0f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,	// bottom right corner
		-0.5f, -1.0f, 0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,	// bottom left corner
		-0.5f, -1.0f, -0.5f,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,	// top left corner
	};

	glGenVertexArrays(numVAOs, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);
	glGenBuffers(numVBOs, mesh.vbo); // Creates buffers

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU

	// Float values per type
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 0; // (r, g, b, a) THIS IS SET TO 0 FOR NOW
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// Store the vertex and index count inside the mesh
	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV));

	// Strides between vertex coordinates is 3 (xyz rgba)
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV);

	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

///////////////////////////////////////////////////
//	UCreateCylinderMesh(GLMesh&)
//
//	mesh: reference to mesh structure for storing data
//
//	Create a cylinder mesh and store it in a VAO/VBO
//
//  Correct triangle drawing commands:
//
//	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
//	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
//	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides
///////////////////////////////////////////////////
void Meshes::CreateCylinderMesh(GLmesh& mesh)
{
	GLfloat verts[] = {
		// cylinder bottom		// normals			// texture coords
		1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f,1.0f,
		.98f, 0.0f, -0.17f,		0.0f, -1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 0.0f, -0.34f,		0.0f, -1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 0.0f, -0.64f,		0.0f, -1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 0.0f, -0.77f,		0.0f, -1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 0.0f, -0.94f,		0.0f, -1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 0.0f, -0.98f,		0.0f, -1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 0.0f, -0.98f,	0.0f, -1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 0.0f, -0.94f,	0.0f, -1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 0.0f, -0.77f,	0.0f, -1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 0.0f, -0.64f,	0.0f, -1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 0.0f, -0.34f,	0.0f, -1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 0.0f, -0.17f,	0.0f, -1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder top			// normals			// texture coords
		1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f,1.0f,
		.98f, 1.0f, -0.17f,		0.0f, 1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 1.0f, -0.34f,		0.0f, 1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 1.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 1.0f, -0.64f,		0.0f, 1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 1.0f, -0.77f,		0.0f, 1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 1.0f, -0.87f,		0.0f, 1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 1.0f, -0.94f,		0.0f, 1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 1.0f, -0.98f,		0.0f, 1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 1.0f, -0.98f,	0.0f, 1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 1.0f, -0.94f,	0.0f, 1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 1.0f, -0.87f,		0.0f, 1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 1.0f, -0.77f,	0.0f, 1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 1.0f, -0.64f,	0.0f, 1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 1.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 1.0f, -0.34f,	0.0f, 1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 1.0f, -0.17f,	0.0f, 1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 1.0f, 0.34f,		0.0f, 1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 1.0f, 0.64f,		0.0f, 1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 1.0f, 0.77f,		0.0f, 1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 1.0f, 0.87f,		0.0f, 1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 1.0f, 0.94f,		0.0f, 1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 1.0f, 0.98f,		0.0f, 1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 1.0f, 0.98f,		0.0f, 1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 1.0f, 0.94f,		0.0f, 1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 1.0f, 0.87f,		0.0f, 1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 1.0f, 0.77f,		0.0f, 1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 1.0f, 0.64f,		0.0f, 1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 1.0f, 0.34f,		0.0f, 1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder body		// normals				// texture coords
		1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0,1.0,
		1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0,0.0,
		.98f, 0.0f, -0.17f,		1.0f, 0.0f, 0.0f,		0.0277,0.0,
		1.0f, 1.0f, 0.0f,		0.92f, 0.0f, -0.08f,	0.0,1.0,
		.98f, 1.0f, -0.17f,		0.92f, 0.0f, -0.08f,	0.0277,1.0,
		.98f, 0.0f, -0.17f,		0.92f, 0.0f, -0.08f,	0.0277,0.0,
		.94f, 0.0f, -0.34f,		0.83f, 0.0f, -0.17f,	0.0554,0.0,
		.98f, 1.0f, -0.17f,		0.83f, 0.0f, -0.17f,	0.0277,1.0,
		.94f, 1.0f, -0.34f,		0.83f, 0.0f, -0.17f,	0.0554,1.0,
		.94f, 0.0f, -0.34f,		0.75f, 0.0f, -0.25f,	0.0554,0.0,
		.87f, 0.0f, -0.5f,		0.75f, 0.0f, -0.25f,	0.0831,0.0,
		.94f, 1.0f, -0.34f,		0.75f, 0.0f, -0.25f,	0.0554,1.0,
		.87f, 1.0f, -0.5f,		0.67f, 0.0f, -0.33f,	0.0831,1.0,
		.87f, 0.0f, -0.5f,		0.67f, 0.0f, -0.33f,	0.0831,0.0,
		.77f, 0.0f, -0.64f,		0.67f, 0.0f, -0.33f,	0.1108,0.0,
		.87f, 1.0f, -0.5f,		0.58f, 0.0f, -0.42f,	0.0831,1.0,
		.77f, 1.0f, -0.64f,		0.58f, 0.0f, -0.42f,	0.1108,1.0,
		.77f, 0.0f, -0.64f,		0.58f, 0.0f, -0.42f,	0.1108,0.0,
		.64f, 0.0f, -0.77f,		0.5f, 0.0f, -0.5f,		0.1385,0.0,
		.77f, 1.0f, -0.64f,		0.5f, 0.0f, -0.5f,		0.1108,1.0,
		.64f, 1.0f, -0.77f,		0.5f, 0.0f, -0.5f,		0.1385,1.0,
		.64f, 0.0f, -0.77f,		0.42f, 0.0f, -0.58f,	0.1385,0.0,
		.5f, 0.0f, -0.87f,		0.42f, 0.0f, -0.58f,	0.1662,0.0,
		.64f, 1.0f, -0.77f,		0.42f, 0.0f, -0.58f,	0.1385, 1.0,
		.5f, 1.0f, -0.87f,		0.33f, 0.0f, -0.67f,	0.1662, 1.0,
		.5f, 0.0f, -0.87f,		0.33f, 0.0f, -0.67f,	0.1662, 0.0,
		.34f, 0.0f, -0.94f,		0.33f, 0.0f, -0.67f,	0.1939, 0.0,
		.5f, 1.0f, -0.87f,		0.25f, 0.0f, -0.75f,	0.1662, 1.0,
		.34f, 1.0f, -0.94f,		0.25f, 0.0f, -0.75f,	0.1939, 1.0,
		.34f, 0.0f, -0.94f,		0.25f, 0.0f, -0.75f,	0.1939, 0.0,
		.17f, 0.0f, -0.98f,		0.17f, 0.0f, -0.83f,	0.2216, 0.0,
		.34f, 1.0f, -0.94f,		0.17f, 0.0f, -0.83f,	0.1939, 1.0,
		.17f, 1.0f, -0.98f,		0.17f, 0.0f, -0.83f,	0.2216, 1.0,
		.17f, 0.0f, -0.98f,		0.08f, 0.0f, -0.92f,	0.2216, 0.0,
		0.0f, 0.0f, -1.0f,		0.08f, 0.0f, -0.92f,	0.2493, 0.0,
		.17f, 1.0f, -0.98f,		0.08f, 0.0f, -0.92f,	0.2216, 1.0,
		0.0f, 1.0f, -1.0f,		0.0f, 0.0f, -1.0f,		0.2493, 1.0,
		0.0f, 0.0f, -1.0f,		0.0f, 0.0f, -1.0f,		0.2493, 0.0,
		-.17f, 0.0f, -0.98f,	0.0f, 0.0f, -1.0f,		0.277, 0.0,
		0.0f, 1.0f, -1.0f,		0.08f, 0.0f, -1.08f,	0.2493, 1.0,
		-.17f, 1.0f, -0.98f,	-0.08f, 0.0f, -0.92f,	0.277, 1.0,
		-.17f, 0.0f, -0.98f,	-0.08f, 0.0f, -0.92f,	0.277, 0.0,
		-.34f, 0.0f, -0.94f,	-0.08f, 0.0f, -0.92f,	0.3047, 0.0,
		-.17f, 1.0f, -0.98f,	-0.08f, 0.0f, -0.92f,	0.277, 1.0,
		-.34f, 1.0f, -0.94f,	-0.17f, 0.0f, -0.83f,	0.3047, 1.0,
		-.34f, 0.0f, -0.94f,	-0.17f, 0.0f, -0.83f,	0.3047, 0.0,
		-.5f, 0.0f, -0.87f,		-0.17f, 0.0f, -0.83f,	0.3324, 0.0,
		-.34f, 1.0f, -0.94f,	-0.25f, 0.0f, -0.75f,	0.3047, 1.0,
		-.5f, 1.0f, -0.87f,		-0.25f, 0.0f, -0.75f,	0.3324, 1.0,
		-.5f, 0.0f, -0.87f,		-0.25f, 0.0f, -0.75f,	0.3324, 0.0,
		-.64f, 0.0f, -0.77f,	-0.33f, 0.0f, -0.67f,	0.3601, 0.0,
		-.5f, 1.0f, -0.87f,		-0.33f, 0.0f, -0.67f,	0.3324, 1.0,
		-.64f, 1.0f, -0.77f,	-0.33f, 0.0f, -0.67f,	0.3601, 1.0,
		-.64f, 0.0f, -0.77f,	-0.42f, 0.0f, -0.58f,	0.3601, 0.0,
		-.77f, 0.0f, -0.64f,	-0.42f, 0.0f, -0.58f,	0.3878, 0.0,
		-.64f, 1.0f, -0.77f,	-0.42f, 0.0f, -0.58f,	0.3601, 1.0,
		-.77f, 1.0f, -0.64f,	-0.5f, 0.0f, -0.5f,		0.3878, 1.0,
		-.77f, 0.0f, -0.64f,	-0.5f, 0.0f, -0.5f,		0.3878, 0.0,
		-.87f, 0.0f, -0.5f,		-0.5f, 0.0f, -0.5f,		0.4155, 0.0,
		-.77f, 1.0f, -0.64f,	-0.58f, 0.0f, -0.42f,	0.3878, 1.0,
		-.87f, 1.0f, -0.5f,		-0.58f, 0.0f, -0.42f,	0.4155, 1.0,
		-.87f, 0.0f, -0.5f,		-0.58f, 0.0f, -0.42f,	0.4155, 0.0,
		-.94f, 0.0f, -0.34f,	-0.67f, 0.0f, -0.33f,	0.4432, 0.0,
		-.87f, 1.0f, -0.5f,		-0.67f, 0.0f, -0.33f,	0.4155, 1.0,
		-.94f, 1.0f, -0.34f,	-0.67f, 0.0f, -0.33f,	0.4432, 1.0,
		-.94f, 0.0f, -0.34f,	-0.75f, 0.0f, -0.25f,	0.4432, 0.0,
		-.98f, 0.0f, -0.17f,	-0.75f, 0.0f, -0.25f,	0.4709, 0.0,
		-.94f, 1.0f, -0.34f,	-0.75f, 0.0f, -0.25f,	0.4432, 1.0,
		-.98f, 1.0f, -0.17f,	-0.83f, 0.0f, -0.17f,	0.4709, 1.0,
		-.98f, 0.0f, -0.17f,	-0.83f, 0.0f, -0.17f,	0.4709, 0.0,
		-1.0f, 0.0f, 0.0f,		-0.83f, 0.0f, -0.17f,	0.4986, 0.0,
		-.98f, 1.0f, -0.17f,	-0.92f, 0.0f, -0.08f,	0.4709, 1.0,
		-1.0f, 1.0f, 0.0f,		-0.92f, 0.0f, -0.08f,	0.4986, 1.0,
		-1.0f, 0.0f, 0.0f,		-0.92f, 0.0f, -0.08f,	0.4986, 0.0,
		-.98f, 0.0f, 0.17f,		-1.0f, 0.0f, 0.0f,		0.5263, 0.0,
		-1.0f, 1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,		0.4986, 1.0,
		-.98f, 1.0f, 0.17f,		-1.0f, 0.0f, 0.0f,		0.5263, 1.0,
		-.98f, 0.0f, 0.17f,		-0.92f, 0.0f, 0.08f,	0.5263, 0.0,
		-.94f, 0.0f, 0.34f,		-0.92f, 0.0f, 0.08f,	0.554, 0.0,
		-.98f, 1.0f, 0.17f,		-0.92f, 0.0f, 0.08f,	0.5263, 1.0,
		-.94f, 1.0f, 0.34f,		-0.83f, 0.0f, 0.17f,	0.554, 1.0,
		-.94f, 0.0f, 0.34f,		-0.83f, 0.0f, 0.17f,	0.554, 0.0,
		-.87f, 0.0f, 0.5f,		-0.83f, 0.0f, 0.17f,	0.5817, 0.0,
		-.94f, 1.0f, 0.34f,		-0.75f, 0.0f, 0.25f,	0.554, 1.0,
		-.87f, 1.0f, 0.5f,		-0.75f, 0.0f, 0.25f,	0.5817, 1.0,
		-.87f, 0.0f, 0.5f,		-0.75f, 0.0f, 0.25f,	0.5817, 0.0,
		-.77f, 0.0f, 0.64f,		-0.67f, 0.0f, 0.33f,	0.6094, 0.0,
		-.87f, 1.0f, 0.5f,		-0.67f, 0.0f, 0.33f,	0.5817, 1.0,
		-.77f, 1.0f, 0.64f,		-0.67f, 0.0f, 0.33f,	0.6094, 1.0,
		-.77f, 0.0f, 0.64f,		-0.58f, 0.0f, 0.42f,	0.6094, 0.0,
		-.64f, 0.0f, 0.77f,		-0.58f, 0.0f, 0.42f,	0.6371, 0.0,
		-.77f, 1.0f, 0.64f,		-0.58f, 0.0f, 0.42f,	0.6094, 1.0,
		-.64f, 1.0f, 0.77f,		-0.5f, 0.0f, 0.5f,		0.6371, 1.0,
		-.64f, 0.0f, 0.77f,		-0.5f, 0.0f, 0.5f,		0.6371, 0.0,
		-.5f, 0.0f, 0.87f,		-0.5f, 0.0f, 0.5f,		0.6648, 0.0,
		-.64f, 1.0f, 0.77f,		-0.42f, 0.0f, 0.58f,	0.6371, 1.0,
		-.5f, 1.0f, 0.87f,		-0.42f, 0.0f, 0.58f,	0.6648, 1.0,
		-.5f, 0.0f, 0.87f,		-0.42f, 0.0f, 0.58f,	0.6648, 0.0,
		-.34f, 0.0f, 0.94f,		-0.33f, 0.0f, 0.67f,	0.6925, 0.0,
		-.5f, 1.0f, 0.87f,		-0.33f, 0.0f, 0.67f,	0.6648, 1.0,
		-.34f, 1.0f, 0.94f,		-0.33f, 0.0f, 0.67f,	0.6925, 1.0,
		-.34f, 0.0f, 0.94f,		-0.25f, 0.0f, 0.75f,	0.6925, 0.0,
		-.17f, 0.0f, 0.98f,		-0.25f, 0.0f, 0.75f,	0.7202, 0.0,
		-.34f, 1.0f, 0.94f,		-0.25f, 0.0f, 0.75f,	0.6925, 1.0,
		-.17f, 1.0f, 0.98f,		-0.17f, 0.0f, 0.83f,	0.7202, 1.0,
		-.17f, 0.0f, 0.98f,		-0.17f, 0.0f, 0.83f,	0.7202, 0.0,
		0.0f, 0.0f, 1.0f,		-0.17f, 0.0f, 0.83f,	0.7479, 0.0,
		-.17f, 1.0f, 0.98f,		-0.08f, 0.0f, 0.92f,	0.7202, 1.0,
		0.0f, 1.0f, 1.0f,		-0.08f, 0.0f, 0.92f,	0.7479, 1.0,
		0.0f, 0.0f, 1.0f,		-0.08f, 0.0f, 0.92f,	0.7479, 0.0,
		.17f, 0.0f, 0.98f,		-0.0f, 0.0f, 1.0f,		0.7756, 0.0,
		0.0f, 1.0f, 1.0f,		-0.0f, 0.0f, 1.0f,		0.7479, 1.0,
		.17f, 1.0f, 0.98f,		-0.0f, 0.0f, 1.0f,		0.7756, 1.0,
		.17f, 0.0f, 0.98f,		0.08f, 0.0f, 0.92f,		0.7756, 0.0,
		.34f, 0.0f, 0.94f,		0.08f, 0.0f, 0.92f,		0.8033, 0.0,
		.17f, 1.0f, 0.98f,		0.08f, 0.0f, 0.92f,		0.7756, 1.0,
		.34f, 1.0f, 0.94f,		0.17f, 0.0f, 0.83f,		0.8033, 1.0,
		.34f, 0.0f, 0.94f,		0.17f, 0.0f, 0.83f,		0.8033, 0.0,
		.5f, 0.0f, 0.87f,		0.17f, 0.0f, 0.83f,		0.831, 0.0,
		.34f, 1.0f, 0.94f,		0.25f, 0.0f, 0.75f,		0.8033, 1.0,
		.5f, 1.0f, 0.87f,		0.25f, 0.0f, 0.75f,		0.831, 1.0,
		.5f, 0.0f, 0.87f,		0.25f, 0.0f, 0.75f,		0.831, 0.0,
		.64f, 0.0f, 0.77f,		0.33f, 0.0f, 0.67f,		0.8587, 0.0,
		.5f, 1.0f, 0.87f,		0.33f, 0.0f, 0.67f,		0.831, 1.0,
		.64f, 1.0f, 0.77f,		0.33f, 0.0f, 0.67f,		0.8587, 1.0,
		.64f, 0.0f, 0.77f,		0.42f, 0.0f, 0.58f,		0.8587, 0.0,
		.77f, 0.0f, 0.64f,		0.42f, 0.0f, 0.58f,		0.8864, 0.0,
		.64f, 1.0f, 0.77f,		0.42f, 0.0f, 0.58f,		0.8587, 1.0,
		.77f, 1.0f, 0.64f,		0.5f, 0.0f, 0.5f,		0.8864, 1.0,
		.77f, 0.0f, 0.64f,		0.5f, 0.0f, 0.5f,		0.8864, 0.0,
		.87f, 0.0f, 0.5f,		0.5f, 0.0f, 0.5f,		0.9141, 0.0,
		.77f, 1.0f, 0.64f,		0.58f, 0.0f, 0.42f,		0.8864, 1.0,
		.87f, 1.0f, 0.5f,		0.58f, 0.0f, 0.42f,		0.9141, 1.0,
		.87f, 0.0f, 0.5f,		0.58f, 0.0f, 0.42f,		0.9141, 0.0,
		.94f, 0.0f, 0.34f,		0.67f, 0.0f, 0.33f,		0.9418, 0.0,
		.87f, 1.0f, 0.5f,		0.67f, 0.0f, 0.33f,		0.9141, 1.0,
		.94f, 1.0f, 0.34f,		0.67f, 0.0f, 0.33f,		0.9418, 1.0,
		.94f, 0.0f, 0.34f,		0.75f, 0.0f, 0.25f,		0.9418, 0.0,
		.98f, 0.0f, 0.17f,		0.75f, 0.0f, 0.25f,		0.9695, 0.0,
		.94f, 1.0f, 0.34f,		0.75f, 0.0f, 0.25f,		0.9418, 0.0,
		.98f, 1.0f, 0.17f,		0.83f, 0.0f, 0.17f,		0.9695, 1.0,
		.98f, 0.0f, 0.17f,		0.83f, 0.0f, 0.17f,		0.9695, 0.0,
		1.0f, 0.0f, 0.0f,		0.83f, 0.0f, 0.17f,		1.0, 0.0,
		.98f, 1.0f, 0.17f,		0.92f, 0.0f, 0.08f,		0.9695, 1.0,
		1.0f, 1.0f, 0.0f,		0.92f, 0.0f, 0.08f,		1.0, 1.0,
		1.0f, 0.0f, 0.0f,		0.92f, 0.0f, 0.08f,		1.0, 0.0
	};

	// total float values per each type
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	// store vertex and index count
	mesh.numVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));
	mesh.numIndices = 0;

	// Create VAO
	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// Create VBO
	glGenBuffers(1, mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void Meshes::DestroyMesh(GLmesh &mesh) {

	glDeleteVertexArrays(numVAOs, &mesh.vao);
	glDeleteBuffers(numVBOs, mesh.vbo);
}
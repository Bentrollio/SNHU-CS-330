#pragma once
// GLmesh.h
#ifndef GLMESH_H
#define GLMESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// macros used to simplify adjusting vertex array and vertex buffer objects
#define numVAOs 1 
#define numVBOs 3

struct GLmesh 
{
	GLuint vao[numVAOs]; // Handle for the Vertex Array Object
	GLuint vbo[numVBOs]; // Handle for the Vertex Buffer Objects
	GLuint numVertices; // Number of vertices for the mesh
	GLuint numIndices; // Number of indices of the mesh
};

void createMesh(GLmesh& mesh);

void destroyMesh(GLmesh& mesh);


#endif /* GLMESH_H */
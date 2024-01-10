#pragma once
// GLmesh.h
#ifndef GLMESH_H
#define GLMESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// macros used to simplify adjusting vertex array and vertex buffer objects
#define numVAOs 1 
#define numVBOs 2

struct GLmesh 
{
	GLuint vao; // Handle for the Vertex Array Object
	GLuint vbo[numVBOs]; // Handle for the Vertex Buffer Objects
	GLuint numIndices; // number of vertices of the mesh
};

void createMesh(GLmesh& mesh);

void destroyMesh(GLmesh& mesh);


#endif /* GLMESH_H */
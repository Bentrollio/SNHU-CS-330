#pragma once
// GLmesh.h
#ifndef GLMESH_H
#define GLMESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct GLmesh 
{
	GLuint vao; // Handle for the Vertex Array Object
	GLuint vbos[2]; // Handle for the Vertex Buffer Objects
	GLuint numIndices; // number of vertices of the mesh
};

#endif /* GLMESH_H */
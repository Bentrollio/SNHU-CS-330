#pragma once
// GLmesh.h
#ifndef GLMESH_H
#define GLMESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct GLmesh 
{
	GLuint vao;
	GLuint vbo;
	GLuint numVertices;
};

#endif /* GLMESH_H */
/*
 * Sphere.h
 * 
 * Procedurally generates a sphere mesh object.
 * 
 * Adapted from the book:
 * Computer Graphics Programming in OpenGL with C++ 2nd Edition
 * By V. Scott Gordon and John Clevenger
 * 
 *
 */

#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include <vector>
#include "utils.h"


 // macros used to simplify adjusting vertex array and vertex buffer objects
//#define numVAOs 1 
#define numVBOs 3

class Sphere
{

private:
	int numVertices;
	int numIndices;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	void init(int);
	float toRadians(float degrees);

public:
	//GLuint vao; // Handle for the Vertex Array Object
	//GLuint vbo[numVBOs]; // Handle for the Vertex Buffer Objects
	Sphere(); // default constructor
	Sphere(int prec);
	int getNumVertices();
	int getNumIndices();
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
};

#endif /* SPHERE_H*/
///////////////////////////////////////////////////////////////////////////////
// meshes.h
// ========
// create meshes for various 3D primitives: plane, pyramid, cube, cylinder, torus, sphere
//
//  ORIGINAL AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
// 
//	Modified and Adapted by Alex Baires
///////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef MESHES_H
#define MESHES_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Sphere.h"


// macros used to simplify adjusting vertex array and vertex buffer objects
#define numVAOs 1 
#define numVBOs 3

class Meshes
{

		// Stores the GL data relative to a given mesh
		struct GLmesh
		{
			GLuint vao; // Handle for the Vertex Array Object
			GLuint vbo[numVBOs]; // Handle for the Vertex Buffer Objects
			GLuint numVertices; // Number of vertices for the mesh
			GLuint numIndices; // Number of indices of the mesh
		};

public:
	
		GLmesh boxMesh;
		GLmesh cubeMesh;
		GLmesh coneMesh;
		GLmesh cylinderMesh;
		GLmesh taperedCylinderMesh;
		GLmesh taperedPolygonMesh;
		GLmesh planeMesh;
		GLmesh prismMesh;
		GLmesh sphereMesh;
		GLmesh pyramid3Mesh;
		GLmesh pyramid4Mesh;
		GLmesh torusMesh;
		GLmesh wingMesh;

		void CreateMeshes();
		void DestroyMeshes();

private:
		void CreateCubeMesh(GLmesh &mesh);
		void CreatePlaneMesh(GLmesh &mesh);
		void CreatePrismMesh(GLmesh& mesh);
		void CreateConeMesh(GLmesh &mesh);
		void CreateCylinderMesh(GLmesh& mesh);
		void CreateTaperedCylinderMesh(GLmesh &mesh);
		void CreateTaperedPolygonMesh(GLmesh& mesh);
		void CreatePyramid3Mesh(GLmesh &mesh);
		void CreatePyramid4Mesh(GLmesh& mesh);
		void CreateSphereMesh(GLmesh& mesh);
		void CreateWingMesh(GLmesh& mesh);
		void DestroyMesh(GLmesh& mesh);

};

#endif /* MESHES_H*/
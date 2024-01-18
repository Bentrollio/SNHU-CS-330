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


// macros used to simplify adjusting vertex array and vertex buffer objects
#define numVAOs 1 
#define numVBOs 2

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
		GLmesh planeMesh;
		GLmesh prismMesh;
		GLmesh sphereMesh;
		GLmesh pyramid3Mesh;
		GLmesh pyramid4Mesh;
		GLmesh torusMesh;

		void CreateMeshes();
		void DestroyMeshes();

private:
		// FIX ME: Might want to be private.
		void CreateCubeMesh(GLmesh &mesh);
		void CreatePlaneMesh(GLmesh &mesh);
		/*void CreatePrismMesh(GLmesh& mesh);
		void CreateBoxMesh(GLmesh &mesh);
		void CreateConeMesh(GLmesh &mesh);
		void CreateCylinderMesh(GLmesh &mesh);
		void CreateTaperedCylinderMesh(GLmesh &mesh);
		void CreateTorusMesh(GLmesh &mesh);
		void CreatePyramid3Mesh(GLmesh &mesh);*/
		void CreatePyramid4Mesh(GLmesh& mesh);
		void CreateSphereMesh(GLmesh& mesh);

		void DestroyMesh(GLmesh& mesh);

		//void CalculateTriangleNormal(glm::vec3 px, glm::vec3 py, glm::vec3 pz);

};

#endif /* MESHES_H*/
/*
 * SNHU CS-330
 * Module 3 Milestone: Beginning a 3D Scene
 *
 * Alex Baires
 * 1-28-24
 *
 */

#include "utils.h"
#include "meshes.h"

#include <cmath>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

float cameraX, cameraY, cameraZ;
float pyrLocX, pyrLocY, pyrLocZ;
float cubeLocX, cubeLocY, cubeLocZ;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//GLmesh mesh; // Triangle mesh data
Meshes meshes;
GLuint renderingProgram;

// Variables to be used in display() function to prevent allocation during rendering
GLuint projLoc, viewLoc, modelLoc, mvLoc;
int width, height;
glm::mat4 pMat, vMat, mMat, mvMat, scale, rotation, translation;

// Hierarchal Matrix Stack for Parent-Child Objects
stack<glm::mat4> mvStack;


// Places application-specific initialization tasks
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram(); // Reads from and compiles GLSL shader files

	// build perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	pMat = glm::perspective(glm::radians(60.0f), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// camera positioning
	cameraX = 0.0f;
	cameraY = 4.0f;
	cameraZ = 14.0f;

	// pyramid location coordinates
	pyrLocX = 4.0f;
	pyrLocY = 2.75f;
	pyrLocZ = 8.0f;

	// cube location coordinates
	cubeLocX = 4.0f;
	cubeLocY = 0.75f;
	cubeLocZ = 8.0f; 

	//createMesh(mesh); // Creates VAO and VBO for pyramid mesh
	meshes.CreateMeshes();
}

// Draws to GLFW display window
void display(GLFWwindow* window, double currentTime) { // AKA urender function in tutorial

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// adjust OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //ENABLES WIREFRAME

	glUseProgram(renderingProgram); // loads compiled shaders into openGL pipeline

	// get the uniform variables for the projection, model and view matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix"); // projection

	// View matrix calculated once and used for all objects
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mvStack.push(vMat);

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// --------------DRAWS THE PLANE-----------------
	mvStack.push(mvStack.top());
	glBindVertexArray(meshes.planeMesh.vao);
	//mvStack.push(mvStack.top());
	// 1. Scale object
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(6.0f, 1.0f, 6.0f));
	// 2. Rotate object
	rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	// 3. Place plane at origin
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // FIX ME: planeLoc variable?

	mMat = translation * rotation * scale;
	mvStack.top() *= mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.planeMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.planeMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO
	glEnableVertexAttribArray(0); // Enables VAO

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.planeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle
	glBindVertexArray(0);
	mvStack.pop();

	// --------------DRAWS THE PYRAMID-----------------
	mvStack.push(mvStack.top()); // Makes copy of view matrix
	glBindVertexArray(meshes.pyramid4Mesh.vao);
	// 1. Scale object 
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.75f, 0.5f));
	// 2. Rotate shape by 30 degrees along y axis. Used glm::radians as an argument to convert degrees to radians
	rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// 3. Place object
	mMat = rotation * scale;
	mvStack.top() *= mMat;
	mvStack.push(mvStack.top()); // Makes PARENT copy of model matrix with scale and rotation
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX, pyrLocY, pyrLocZ));
	mvStack.top() *= translation;

	// copy model and view matrices to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.pyramid4Mesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.pyramid4Mesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO
	glEnableVertexAttribArray(0); // Enables VAO

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.pyramid4Mesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle
	glBindVertexArray(0);
	mvStack.pop(); // Pops the Pyramid MV matrix

	// --------------DRAWS THE CUBE-----------------
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Makes copy of PARENT model matrix with scale and rotation
	// Scale to Shrink object height slightly
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	// Places the object
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));

	// Concatenates cube scale and translation matrices to the Parent Pyramid MV matrix
	mvStack.top() *= translation * scale;

	// copy projection, model and view matrices to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.cubeMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.cubeMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO
	glEnableVertexAttribArray(0); // Enables VAO

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.cubeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

	glBindVertexArray(0);
	mvStack.pop(); // Erases child MV matrix
	mvStack.pop(); // Erases parent MV matrix
	mvStack.pop(); // Erases view matrix
	
}

int main(void) {

	// GLFW: Initialize and configure
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Using OpenGL 4.3 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW: Window creation
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3-5 Milestone - Beginning a 3D Scene", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLEW: load all OpenGL function pointers
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);

	init(window);

	// Rendering loop
	while (!glfwWindowShouldClose(window)) {

		// input
		processInput(window);
		
		display(window, glfwGetTime()); // glfwGetTime gets elapsed time since GLFW was initialized

		// glfw swap buffers and poll IO events
		glfwSwapBuffers(window); // Paints window
		glfwPollEvents(); // Handles window-related events like key-presses
	}

	meshes.DestroyMeshes();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

}
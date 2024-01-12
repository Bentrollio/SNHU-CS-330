/*
 * 3D Practice
 * 
 * Creating a 3D cube.
 * 
 * 
 */

#include "utils.h"
#include "GLmesh.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

GLmesh mesh; // Triangle mesh data
GLuint renderingProgram;

// Variables to be used in display() function to prevent allocation during rendering
GLuint mvLoc, projLoc, viewLoc, modelLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, scale, rotation, translation;

// Places application-specific initialization tasks
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram(); // Reads from and compiles GLSL shader files

	// camera positioning
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 3.0f;
	cubeLocX = 0.0f;
	cubeLocY = -2.0f; // shift down Y to reveal perspective
	cubeLocZ = 0.0f; 

	createMesh(mesh); // Creates VAO and VBO

}

// Draws to GLFW display window
void display(GLFWwindow* window, double currentTime) { // FIX ME AKA URENDER

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderingProgram); // loads compiled shaders into openGL pipeline

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix"); // model view matrix
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix"); // projection
	modelLoc = glGetUniformLocation(renderingProgram, "model_matrix");
	viewLoc = glGetUniformLocation(renderingProgram, "view_matrix");


	// build perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	//pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
	pMat = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

	// build view matrix, model matrix, and model-view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	//glm::mat4 vMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	//mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));

	// 1. Scale object by 2
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.75f, 0.75));
	// 2. Rotate shape by 90 degrees along y axis
	rotation = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0.0, 1.0f, 0.0f));
	// 3. Place object at origin
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	//mvMat = vMat * mMat;
	mMat = translation * rotation * scale;

	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(vMat));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// adjust OpenGL settings and draw model
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //ENABLES WIREFRAME

	// Draw triangle
	glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle
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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3-3 Assignment - Building a 3D Pyramid", NULL, NULL);
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

	destroyMesh(mesh);
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

}
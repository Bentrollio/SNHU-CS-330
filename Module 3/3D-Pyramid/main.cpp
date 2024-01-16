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
GLuint projLoc, viewLoc, modelLoc;
int width, height;
glm::mat4 pMat, vMat, mMat, scale, rotation, translation;

// Places application-specific initialization tasks
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram(); // Reads from and compiles GLSL shader files

	// camera positioning
	cameraX = 0.0f;
	cameraY = -0.2f;
	cameraZ = 5.0f;

	// pyramid location coordinates
	cubeLocX = 0.0f;
	cubeLocY = 0.0f;
	cubeLocZ = 0.0f; 

	createMesh(mesh); // Creates VAO and VBO for pyramid mesh

}

// Draws to GLFW display window
void display(GLFWwindow* window, double currentTime) { // AKA urender function in tutorial

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderingProgram); // loads compiled shaders into openGL pipeline

	// get the uniform variables for the projection, model and view matrices
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix"); // projection
	modelLoc = glGetUniformLocation(renderingProgram, "model_matrix"); // model
	viewLoc = glGetUniformLocation(renderingProgram, "view_matrix"); // view

	// build perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	
	pMat = glm::perspective(1.0472f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// build view matrix, model matrix, and model-view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	// 1. Scale object by 1 (I built my mesh with different vertices than the tutorial)
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	// 2. Rotate shape by 25 degrees along y axis (to match screenshot of the rubric. Used glm::radians as an argument to convert 25 degrees to radians
	rotation = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// 3. Place object at the origin (0, 0, 0)
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));

	//mvMat = vMat * mMat;
	mMat = translation * rotation * scale;

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
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //ENABLES WIREFRAME

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
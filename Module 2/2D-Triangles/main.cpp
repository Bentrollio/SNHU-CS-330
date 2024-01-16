/*
 * SNHU CS-330 
 * Module 2 Assignment - 2D Triangles
 * 
 * Alex Baires
 * 1-21-24
 * 
 */

#include "utils.h"
#include "GLmesh.h"

using namespace std;

GLmesh mesh; // Triangle mesh data
GLuint renderingProgram;

// Places application-specific initialization tasks
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram(); // Reads from and compiles GLSL shader files
	createMesh(mesh); // Creates VAO and VBO
}

// Draws to GLFW display window
void display(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram); // loads compiled shaders into openGL pipeline
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Activate VBOs contained within mesh
	glBindVertexArray(mesh.vao[0]);

	// Draw triangle
	glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

	// Deactivate VAO
	glBindVertexArray(0);

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "2-3 Assignment - 2D Triangles", NULL, NULL);
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
/*
 * SNHU CS-330 
 * Module 2 Assignment - 2D Triangles
 * 
 * Alex Baires
 * 1-21-24
 * 
 */

#include "utils.h"

#define numVAOs 1

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];

// Places application-specific initialization tasks
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

// Draws to GLFW display window
void display(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram); // loads compiled shaders into openGL pipeline
	glPointSize(30.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3); // initiates pipeline processing
}

int main(void) {

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);

	init(window);

	// Rendering loop
	while (!glfwWindowShouldClose(window)) {
		
		display(window, glfwGetTime()); // glfwGetTime gets elapsed time since GLFW was initialized
		glfwSwapBuffers(window); // Paints window
		glfwPollEvents(); // Handles window-related events like key-presses
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

}
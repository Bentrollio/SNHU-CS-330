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

#define numVAOs 1

using namespace std;

GLmesh mesh; // Triangle mesh data
GLuint renderingProgram;
//FIXME GLuint vao[numVAOs];

// Places application-specific initialization tasks
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	//FIXME: glGenVertexArrays(numVAOs, vao);
	//FIXME: glBindVertexArray(vao[0]);
}

// Draws to GLFW display window
void display(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram); // loads compiled shaders into openGL pipeline
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//glDrawArrays(GL_TRIANGLES, 0, 3); // initiates pipeline processing

	//Activate VBOs contained within mesh
	glBindVertexArray(mesh.vao);

	// Draw triangle
	glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

	// Deactivate VAO
	glBindVertexArray(0);

}

int main(void) {

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(600, 600, "2-3 Assignment - 2D Triangles", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, UResizeWindow);

	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);

	init(window);

	// Create mesh
	UCreateMesh(mesh); // Creates the VBO



	// Rendering loop
	while (!glfwWindowShouldClose(window)) {

		// input
		UProcessInput(window);
		
		display(window, glfwGetTime()); // glfwGetTime gets elapsed time since GLFW was initialized

		// glfw swap buffers and poll IO events
		glfwSwapBuffers(window); // Paints window
		glfwPollEvents(); // Handles window-related events like key-presses
	}

	UDestroyMesh(mesh);
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

}
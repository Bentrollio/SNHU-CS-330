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
#include <stack> // For matrix stack
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

float cameraX, cameraY, cameraZ;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//GLmesh mesh; // Triangle mesh data
Meshes meshes;
GLuint renderingProgram;

// Variables to be used in display() function to prevent allocation during rendering
GLuint projLoc, viewLoc, modelLoc, objectColorLoc, mvLoc;
int width, height;
glm::mat4 pMat, vMat, mMat, scale, rotation, translation;

// Hierarchal Matrix Stack for Parent-Child Objects
stack<glm::mat4> mvStack;

// Places application-specific initialization tasks
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram(); // Reads from and compiles GLSL shader files

	// build perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	pMat = glm::perspective(glm::radians(90.0f), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// camera positioning
	cameraX = 0.0f;
	cameraY = 4.0f;
	cameraZ = 10.0f;

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

	// get the uniform variables for the model view matrix
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix"); // projection

	// View matrix calculated once and used for all objects
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	// Copy view matrix to stack
	mvStack.push(vMat);
	
	// Copy projection matrix to the uniform variable
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// --------------DRAWS THE PLANE-----------------
	glBindVertexArray(meshes.planeMesh.vao);

	mvStack.push(mvStack.top()); // Places a copy of the view matrix at top of stack to add model info to
	// 1. Places plane at origin
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Positions the plane

	// 2. Rotates Plane
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scales Plane
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(15.0f, 1.0f, 10.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// Associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.planeMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.planeMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draws the plane
	glDrawElements(GL_TRIANGLES, meshes.planeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle
	glBindVertexArray(0);

	mvStack.pop(); // Removes Plane transforms from stack

	/**************************************************
	 * START of RENDERING CRYSTAL OBJECT
	 *
	 * Uses Pyramid and Cube Objects.
	 **************************************************
	 */

	// --------------DRAWS THE PYRAMID (PARENT)-----------------
	glBindVertexArray(meshes.pyramid4Mesh.vao);

	mvStack.push(mvStack.top()); // copies view matrix for manipulation
	// 1. Place Pyramid
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 3.0f, 3.0f)); // Positions the pyramid
	mvStack.push(mvStack.top()); // Copies view * (PYRAMID) position to top of stack

	// 2. Rotates Pyramid 85 degrees along the y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(85.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mvStack.push(mvStack.top()); // Copies view * PYRAMID(position * rotation) to top

	// 3. Scales Pyramid
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.25f, 0.5f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.pyramid4Mesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.pyramid4Mesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draws the pyramd (4-sided)
	glDrawElements(GL_TRIANGLES, meshes.pyramid4Mesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle
	glBindVertexArray(0);

	mvStack.pop(); // Removes PYRAMID scale

	// --------------DRAWS THE CUBE (CHILD OF PYRAMID)-----------------
	glBindVertexArray(meshes.cubeMesh.vao);

	mvStack.push(mvStack.top()); // Makes copy of PYRAMID(position * rotation)
	// 1. Place Cube relative to Pyramid
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -2.25, 0.0));

	// 2. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.0f, 0.5f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.cubeMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.cubeMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draws the cube
	glDrawElements(GL_TRIANGLES, meshes.cubeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

	glBindVertexArray(0);

	mvStack.pop();
	mvStack.pop(); 
	mvStack.pop(); // All that remains in stack is view matrix

	// **** END of RENDERING CRYSTAL OBJECT ****

	/**************************************************
	 * START of RENDERING SOLAR SYSTEM GLOBE
	 *
	 * Version 1 - Uses Sphere and Cube Objects
	 * ** I plan to make a prism with slanted sides in the final**
	 **************************************************
	 */

	 // --------------DRAWS THE SPHERE (PARENT)-----------------
	glBindVertexArray(meshes.sphereMesh.vao);

	mvStack.push(mvStack.top()); // copies the view matrix to the top for manipulation
	// 1. Position Sphere
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.75f, 4.2f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	// Have the vertex shader color this object
	//glProgramUniform4f(renderingProgram, objectColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.sphereMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.sphereMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draws the sphere
	glDrawElements(GL_TRIANGLES, meshes.sphereMesh.numIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the VAO
	glBindVertexArray(0);

	// --------------DRAWS THE CUBE (CHILD OF SPHERE)-----------------
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copies Sphere position

	// 1. Place Cube Relative to Sphere
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	// 2. Rotate Cube by 45 degrees on y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// 3. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.3f, 0.75f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.cubeMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.cubeMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draws the cube
	glDrawElements(GL_TRIANGLES, meshes.cubeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

	glBindVertexArray(0);

	mvStack.pop();
	mvStack.pop(); // All that remains is the view matrix

	/**************************************************
	 * START of RENDERING THE BOOK
	 **************************************************
	 */
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // copies view matrix for manipulation

	// 1. Position cube
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, -3.0f));

	// 2. Rotate the cube slightly clockwise
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0, 1.0f, 0.0f));

	// 3. Scale the cube to be booklike, double in width, taller in height, same depth
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 5.0f, 1.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.cubeMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.cubeMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draws the cube
	glDrawElements(GL_TRIANGLES, meshes.cubeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

	glBindVertexArray(0);

	mvStack.pop(); // All that remains is the view matrix


	/**************************************************
	 * START of PRISM test
	 **************************************************
	 */
	glBindVertexArray(meshes.prismMesh.vao);
	mvStack.push(mvStack.top()); // copies view matrix for manipulation

	// 1. Position the prism
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 1.0f, 0.0f));

	// 2. Rotate the prism
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scale the prism
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attriibute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.prismMesh.vbo[0]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draws the prism
	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.prismMesh.numVertices);

	glBindVertexArray(0);
	mvStack.pop(); // All that remains is the view matrix
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
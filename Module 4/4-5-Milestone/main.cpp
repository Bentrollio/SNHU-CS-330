/*
 * SNHU CS-330
 * Module 4 Milestone: Interactivity in a 3D Scene
 *
 * Alex Baires
 * 2-4-24
 *
 */

#include "utils.h"
#include "meshes.h"

#include <cmath>
#include <stack> // For matrix stack

Camera camera(glm::vec3(2.0f, 4.0f, 15.0f)); // Needs work.

using namespace std;

//GLmesh mesh; // Triangle mesh data
Meshes meshes;
GLuint renderingProgram;

// Timing
float deltaTime = 0.0f; // time between current time and last frame
float lastFrame = 0.0f;

// Variables to be used in display() function to prevent allocation during rendering
GLuint projLoc, viewLoc, modelLoc, objectColorLoc, mvLoc;
int width, height;
glm::mat4 pMat, vMat, mMat;

// Hierarchal Matrix Stack for Parent-Child Objects
stack<glm::mat4> mvStack;

// Places application-specific initialization tasks
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram(); // Reads from and compiles GLSL shader files

	glfwGetFramebufferSize(window, &width, &height);

	// Mouse Events
	glfwSetCursorPosCallback(window, glfwMousePositionCallbackWrapper);
	glfwSetScrollCallback(window, glfwMouseScrollCallbackWrapper);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallbackWrapper);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	vMat = camera.GetViewMatrix();
	// Projection matrix (DEFAULT)
	pMat = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f);
	// Orthographic matrix (If user presses P key)
	activateOrtho(window, pMat);
	
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
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// Associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.planeMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.planeMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draw triangles
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
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 2.76f, 3.0f)); // Positions the pyramid
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

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.pyramid4Mesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle
	glBindVertexArray(0);

	mvStack.pop(); // Removes PYRAMID scale

	// --------------DRAWS THE CUBE (CHILD OF PYRAMID)-----------------
	glBindVertexArray(meshes.cubeMesh.vao);

	mvStack.push(mvStack.top()); // Makes copy of PYRAMID(position * rotation)
	// 1. Place Cube relative to Pyramid
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -2.0, 0.0));

	// 2. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.75f, 0.5f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.cubeMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.cubeMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.cubeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

	glBindVertexArray(0);

	mvStack.pop();
	mvStack.pop(); 
	mvStack.pop(); // All that remains in stack is view matrix

	// **** END of RENDERING CRYSTAL OBJECT ****

	/**************************************************
	 * START of RENDERING SOLAR SYSTEM GLOBE
	 **************************************************
	 */

	 // --------------DRAWS THE SPHERE (PARENT)-----------------
	glBindVertexArray(meshes.sphereMesh.vao);

	mvStack.push(mvStack.top()); // copies the view matrix to the top for manipulation
	// 1. Position Sphere
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 4.2f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.sphereMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.sphereMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.sphereMesh.numIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the VAO
	glBindVertexArray(0);

	// --------------DRAWS THE TAPERED POLYGON PEDESTAL (CHILD OF SPHERE)-----------------
	glBindVertexArray(meshes.taperedPolygonMesh.vao);
	mvStack.push(mvStack.top()); // Copies Sphere position

	// 1. Place Cube Relative to Sphere
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.17f, 0.0f));

	// 2. Rotate Cube by 45 degrees on y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// 3. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.3f, 0.75f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.taperedPolygonMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.taperedPolygonMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.taperedPolygonMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

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
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, -3.0f));

	// 2. Rotate the cube slightly clockwise
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0, 1.0f, 0.0f));

	// 3. Scale the cube to be booklike, double in width, taller in height, smaller depth
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 3.0f, 0.50f));

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
	 * DRAWS THE MINI TRAFFIC CONE
	 **************************************************
	 */
	 // --------------DRAWS THE CONE (PARENT)-----------------
	glBindVertexArray(meshes.coneMesh.vao);
	mvStack.push(mvStack.top()); // copies view matrix for manipulation

	// 1. Position the cone
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.11f, 8.0f));

	// 2. Rotate the cone
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scale the cone
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 2.0f, 1.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attriibute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.coneMesh.vbo[0]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draws the cone
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36); // bottom
	glDrawArrays(GL_TRIANGLE_STRIP, 36, 108); // sides

	glBindVertexArray(0);

	// --------------DRAWS THE SQUARE BASE OF TRAFFIC CONE (CHILD OF CONE)-----------------
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copies Sphere position

	// 1. Place Cube Relative to cone
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// 2. Rotate Cube by 45 degrees on y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scale Cube to Cone
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 0.05f, 1.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, meshes.cubeMesh.vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.cubeMesh.vbo[1]);
	glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, 0); // Specifies format of vertex info in VAO

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.cubeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

	glBindVertexArray(0);

	mvStack.pop();
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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "4-5 Milestone: Interactivity in a 3D Scene", NULL, NULL);
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

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window, camera, deltaTime);
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
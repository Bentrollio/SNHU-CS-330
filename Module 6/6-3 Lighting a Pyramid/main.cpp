/*
 * SNHU CS-330
 * Module 6 Assignment - Lighting a Pyramid
 *
 * Alex Baires
 * 2-11-24
 *
 */

#include "utils.h"
#include "GLmesh.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

using namespace std;

// Pyramid Location Variables
float pyrLocX, pyrLocY, pyrLocZ;

GLmesh mesh; // Triangle mesh data
GLuint renderingProgram;

// Timing
float gDeltatime = 0.0f; // Time between current time and last frame
float gLastFrame = 0.0f;

// Variables to be used in display() function to prevent allocation during rendering
GLuint projLoc, mvLoc, objectColorLoc, nLoc;
int width, height;
glm::mat4 pMat, vMat, mMat, scale, rotation, translation, mvMat, invTrMat;

// Textures
GLuint brickTexture;

// Lighting variables
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;
glm::vec3 currentLightPos, lightPosV; // light position as Vector3f, in both model and view space
float lightPos[3]; // light position as float array

// initial light location
glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);

// white light properties
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// gold material properties
float* matAmb = goldAmbient();
float* matDif = goldDiffuse();
float* matSpe = goldSpecular();
float matShi = goldShininess();

void installLights(glm::mat4 vMatrix) {
	// Convert light's position to view space and save it in a float array
	lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = lightPosV.x;
	lightPos[1] = lightPosV.y;
	lightPos[2] = lightPosV.z;

	// Get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	mAmbLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mDiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mSpecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mShiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	// Set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mAmbLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mDiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mSpecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mShiLoc, matShi);
}

// Places application-specific initialization tasks
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram(); // Reads from and compiles GLSL shader files

	// build perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	// Mouse events
	glfwSetCursorPosCallback(window, glfwMousePositionCallbackWrapper);
	glfwSetScrollCallback(window, glfwMouseScrollCallbackWrapper);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallbackWrapper);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	pMat = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f);

	// pyramid location coordinates
	pyrLocX = 0.0f;
	pyrLocY = 0.0f;
	pyrLocZ = 0.0f; 

	createMesh(mesh); // Creates VAO and VBO for pyramid mesh
	brickTexture = loadTexture("brick1.jpg");
}

// Draws to GLFW display window
void display(GLFWwindow* window, double currentTime) { // AKA urender function in tutorial

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// adjust OpenGL settings and draw model
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(renderingProgram); // loads compiled shaders into openGL pipeline

	// get the uniform variables for the projection, model and view matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix"); // projection
	// color uniform variable for non-textured objects
	objectColorLoc = glGetUniformLocation(renderingProgram, "objectColor");
	// lighting normal uniform location variable
	nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");

	// *** build view matrix, model matrix, and model-view matrix.
	// View Matrix calculated once
	vMat = camera.GetViewMatrix();
	// Projection matrix
	pMat = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f);
	// Orthographic matrix (If user presses P key)
	activateOrtho(window, pMat);
	// Copy projection matrix to the uniform variable
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));


	// --------------DRAWS THE PYRAMID-----------------
	glBindVertexArray(mesh.vao[0]);
	glProgramUniform4f(renderingProgram, objectColorLoc, 1.0f, 0.0f, 0.50196078f, 1.0f);

	// 1. Scale object by 1 (I built my mesh with different vertices than the tutorial)
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	// 2. Rotate shape by 25 degrees along y axis (to match screenshot of the rubric. Used glm::radians as an argument to convert 25 degrees to radians
	rotation = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// 3. Place object at the origin (0, 0, 0)
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX, pyrLocY, pyrLocZ));

	mMat = translation * rotation * scale;

	// Set up lights based pm current lights position
	currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
	installLights(vMat);

	// Build the MODEL-VIEW matrix by concatenating matrixes v and m
	mvMat = vMat * mMat;

	// build the inverse-transpose of the MV matrix, for transforming normal vectors
	invTrMat = glm::transpose(glm::inverse(mvMat));

	// Copy model-view matrix to the uniform variable for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]);

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brickTexture);
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "6-3 Assignment - Lighting a Pyramid", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLEW: load all OpenGL function pointers
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);
	init(window);

	glUniform1i(glGetUniformLocation(renderingProgram, "samp"), 0);

	// Rendering loop
	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		gDeltatime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

		// input
		processInput(window, camera, gDeltatime);
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
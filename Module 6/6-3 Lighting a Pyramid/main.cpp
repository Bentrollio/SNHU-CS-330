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
GLuint lightingProgram;

// Timing
float gDeltatime = 0.0f; // Time between current time and last frame
float gLastFrame = 0.0f;

// Variables to be used in display() function to prevent allocation during rendering
GLuint projLoc, mvLoc, modelLoc, objectColorLoc, viewPosLoc, ambStrLoc, ambColLoc, light1ColLoc, light1PosLoc,
light2ColLoc, light2PosLoc, specInt1Loc, highlghtSz1Loc, specInt2Loc, highlghtSz2Loc;
int width, height;
glm::mat4 pMat, vMat, mMat, scale, rotation, translation, mvMat;

// Textures
GLuint brickTexture;

//// Lighting variables
void installLights() {

	viewPosLoc = glGetUniformLocation(renderingProgram, "viewPosition");
	ambStrLoc = glGetUniformLocation(renderingProgram, "ambientStrength");
	ambColLoc = glGetUniformLocation(renderingProgram, "ambientColor");
	light1ColLoc = glGetUniformLocation(renderingProgram, "light1Color");
	light1PosLoc = glGetUniformLocation(renderingProgram, "light1Position");
	light2ColLoc = glGetUniformLocation(renderingProgram, "light2Color");
	light2PosLoc = glGetUniformLocation(renderingProgram, "light2Position");
	specInt1Loc = glGetUniformLocation(renderingProgram, "specularIntensity1");
	highlghtSz1Loc = glGetUniformLocation(renderingProgram, "highlightSize1");
	specInt2Loc = glGetUniformLocation(renderingProgram, "specularIntensity2");
	highlghtSz2Loc = glGetUniformLocation(renderingProgram, "highlightSize2");

	// Set ambient lighting strength
	glUniform1f(ambStrLoc, 0.4f);
	// Set ambient color
	glUniform3f(ambColLoc, 0.1f, 0.1f, 0.1f);
	glUniform3f(light1ColLoc, 1.0f, 0.2f, 0.2f);
	glUniform3f(light1PosLoc, -1.0f, 1.0f, -1.0f);
	glUniform3f(light2ColLoc, 0.2f, 1.0f, 0.2f);
	glUniform3f(light2PosLoc, 1.0f, 1.0f, -1.0f);

	// Set specular intensity
	glUniform1f(specInt1Loc, .8f);
	glUniform1f(specInt2Loc, .8f);
	// Set specular highlight size
	glUniform1f(highlghtSz1Loc, 2.0f);
	glUniform1f(highlghtSz2Loc, 2.0f);
}

// Places application-specific initialization tasks
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram("vertShader.glsl", "fragShader.glsl"); // Reads from and compiles GLSL shader files
	lightingProgram = createShaderProgram("lightVertShader.glsl", "lightFragShader.glsl");

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
	modelLoc = glGetUniformLocation(renderingProgram, "model"); // model only
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix"); // model-view matrix
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix"); // projection
	// color uniform variable for non-textured objects
	objectColorLoc = glGetUniformLocation(renderingProgram, "objectColor");

	// *** build view matrix, model matrix, and model-view matrix.
	// View Matrix calculated once
	vMat = camera.GetViewMatrix();
	// Projection matrix
	pMat = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f);
	// Orthographic matrix (If user presses P key)
	activateOrtho(window, pMat);
	// Copy projection matrix to the uniform variable
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	installLights();

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

	// Build the MODEL-VIEW matrix by concatenating matrixes v and m
	mvMat = vMat * mMat;

	// Copy model-view matrix to the uniform variable for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindVertexArray(0);

	// Use lighting program
	glUseProgram(lightingProgram);

	// get the uniform variables for the projection, model-view matrices
	modelLoc = glGetUniformLocation(lightingProgram, "model"); // model only
	mvLoc = glGetUniformLocation(lightingProgram, "mv_matrix"); // model-view matrix
	projLoc = glGetUniformLocation(lightingProgram, "proj_matrix"); // projection

	// Copy model-view matrix to the uniform variable for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	// --------------DRAWS THE FIRST LIGHTING PYRAMID-----------------
	glBindVertexArray(mesh.vao[0]);
	//glProgramUniform4f(lightingProgram, objectColorLoc, 1.0f, 0.0f, 0.50196078f, 1.0f);

	// 1. Scale object by 1 (I built my mesh with different vertices than the tutorial)
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	// 2. Rotate shape 11 degrees along x axis
	rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-11.459f), glm::vec3(1.0f, 0.0f, 0.0f));
	// 3. Place object at origin
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, -1.0f));

	mMat = translation * rotation * scale;

	// Build the MODEL-VIEW matrix by concatenating matrixes v and m
	mvMat = vMat * mMat;

	// Copy model-view matrix to the uniform variable for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
	glBindVertexArray(0);

	// --------------DRAWS THE Second LIGHTING PYRAMID-----------------
	glBindVertexArray(mesh.vao[0]);
	//glProgramUniform4f(lightingProgram, objectColorLoc, 1.0f, 0.0f, 0.50196078f, 1.0f);

	// 1. Scale object by 1 (I built my mesh with different vertices than the tutorial)
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	// 2. Rotate shape 11 degrees along x axis
	rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-11.459f), glm::vec3(1.0f, 0.0f, 0.0f));
	// 3. Place object at origin
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -1.0f));

	mMat = translation * rotation * scale;

	// Build the MODEL-VIEW matrix by concatenating matrixes v and m
	mvMat = vMat * mMat;

	// Copy model-view matrix to the uniform variable for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
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
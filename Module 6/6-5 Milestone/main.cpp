/*
 * SNHU CS-330
 * Module 6 Milestone: Lighting Complex Objects
 *
 * Alex Baires
 * 2-18-24
 *
 */

#include "utils.h"
#include "meshes.h"
#include <cmath>
#include <stack> // For matrix stack

Camera camera(glm::vec3(0.0f, 4.0f, 20.0f)); // Needs work.

using namespace std;

// Different shape meshes
Meshes meshes;

// Shader programs
GLuint renderingProgram, lightedPyramidShaders, materialShaders;

// Timing
float deltaTime = 0.0f; // time between current time and last frame
float lastFrame = 0.0f;

// Variables to be used in display() function to prevent allocation during rendering
GLuint projLoc, viewLoc, modelLoc, mvLoc, objectColorLoc, viewPosLoc, ambStrLoc, ambColLoc, light1ColLoc, light1PosLoc,
light2ColLoc, light2PosLoc, specInt1Loc, highlghtSz1Loc, specInt2Loc, highlghtSz2Loc;

// FIX ME: Material Variables
GLuint mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;
int width, height;

//// PEARL Material properties
//float* matAmb = pearlAmbient();
//float* matDif = pearlDiffuse();
//float* matSpe = pearlSpecular();
//float matShi = pearlShininess();

// JADE Material properties
float* matAmb = jadeAmbient();
float* matDif = jadeDiffuse();
float* matSpe = jadeSpecular();
float matShi = jadeShininess();

// Render matrices
glm::mat4 pMat, vMat;

// Lighting matrices to denote light sources
glm::mat4 mMat, mvMat, scale, rotation, translation;

// UBO to condense uniforms
GLuint ubo;


// Texture variables;
GLuint seleniteBaseTexture, seleniteTipTexture, fabricTexture, fabricRoughnessTexture, grungeTexture, plasticTexture, walmartLogoTexture,
blackRubberBaseTexture;

// Lighting Variables for general objects
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

	// Set the camera view location
	glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

	// Set ambient lighting strength
	glUniform1f(ambStrLoc, 0.3f);
	// Set ambient color
	glUniform3f(ambColLoc, 0.7f, 0.7f, 0.7f); // White natural light
	//Set color of the first light
	glUniform3f(light1ColLoc, 0.2f, 0.2f, 1.0f); // Blue light from a monitor
	// Set position of the first light
	glUniform3f(light1PosLoc, -10.0f, 5.0f, -15.0f);
	// Set color of the second light
	glUniform3f(light2ColLoc, 1.0f, 0.78823529, 0.39215686); // 3900k "yellow" light bulb
	// Set position of the second light
	glUniform3f(light2PosLoc, 0.0f, 8.0f, 10.0f);

	// Set specular intensity
	glUniform1f(specInt1Loc, 0.1f);
	glUniform1f(specInt2Loc, 1.0f);
	// Set specular highlight size
	glUniform1f(highlghtSz1Loc, 2.0f);
	glUniform1f(highlghtSz2Loc, 6.0f);

}

// FIXME Lighting variables for material shaders
void installMaterialShader() {
	viewPosLoc = glGetUniformLocation(materialShaders, "viewPosition");
	ambStrLoc = glGetUniformLocation(materialShaders, "ambientStrength");
	ambColLoc = glGetUniformLocation(materialShaders, "ambientColor");
	light1ColLoc = glGetUniformLocation(materialShaders, "light1Color");
	light1PosLoc = glGetUniformLocation(materialShaders, "light1Position");
	light2ColLoc = glGetUniformLocation(materialShaders, "light2Color");
	light2PosLoc = glGetUniformLocation(materialShaders, "light2Position");
	specInt1Loc = glGetUniformLocation(materialShaders, "specularIntensity1");
	highlghtSz1Loc = glGetUniformLocation(materialShaders, "highlightSize1");
	specInt2Loc = glGetUniformLocation(materialShaders, "specularIntensity2");
	highlghtSz2Loc = glGetUniformLocation(materialShaders, "highlightSize2");

	// Material uniforms
	mAmbLoc = glGetUniformLocation(materialShaders, "material.ambient");
	mDiffLoc = glGetUniformLocation(materialShaders, "material.diffuse");
	mSpecLoc = glGetUniformLocation(materialShaders, "material.specular");
	mShiLoc = glGetUniformLocation(materialShaders, "material.shininess");

	// Set the camera view location
	glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

	// Set ambient lighting strength
	glUniform1f(ambStrLoc, 0.3f);
	// Set ambient color
	glUniform3f(ambColLoc, 0.7f, 0.7f, 0.7f); // White natural light
	//Set color of the first light
	glUniform3f(light1ColLoc, 0.2f, 0.2f, 1.0f); // Blue light from a monitor
	// Set position of the first light
	glUniform3f(light1PosLoc, -10.0f, 5.0f, -15.0f);
	// Set color of the second light
	glUniform3f(light2ColLoc, 1.0f, 0.78823529, 0.39215686); // 3900k "yellow" light bulb
	// Set position of the second light
	glUniform3f(light2PosLoc, 0.0f, 8.0f, 10.0f);

	// Set specular intensity
	glUniform1f(specInt1Loc, 0.1f);
	glUniform1f(specInt2Loc, 1.0f);
	// Set specular highlight size
	glUniform1f(highlghtSz1Loc, 2.0f);
	glUniform1f(highlghtSz2Loc, 6.0f);

	// Set material values in the shader
	glUniform4fv(mAmbLoc, 1, matAmb);
	glUniform4fv(mDiffLoc, 1, matDif);
	glUniform4fv(mSpecLoc, 1, matSpe);
	glUniform1f(mShiLoc, matShi);


}

struct LightData {
	glm::vec3 viewPosition;
	float ambientStrength;
	glm::vec3 ambientColor;
	glm::vec3 light1Color;
	glm::vec3 light1Position;
	glm::vec3 light2Color;
	glm::vec3 light2Position;
	float specularIntensity1;
	float highlightSize1;
	float specularIntensity2;
	float highlightSize2;
};


// Hierarchal Matrix Stack for Parent-Child Objects
stack<glm::mat4> mvStack;
stack<glm::mat4> modelStack; // Model only stack for lighting calculations

// Places application-specific initialization tasks
void init(GLFWwindow* window) {

	renderingProgram = createShaderProgram("vertShader.glsl", "fragShader.glsl"); // Reads from and compiles GLSL shader files
	lightedPyramidShaders = createShaderProgram("lightVertShader.glsl", "lightFragShader.glsl"); // Creates the pyramids that represent the lighting position
	materialShaders = createShaderProgram("materialVertShader.glsl", "materialFragShader.glsl");

	glfwGetFramebufferSize(window, &width, &height);

	// Mouse Events
	glfwSetCursorPosCallback(window, glfwMousePositionCallbackWrapper);
	glfwSetScrollCallback(window, glfwMouseScrollCallbackWrapper);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallbackWrapper);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	meshes.CreateMeshes();

	seleniteBaseTexture = loadTexture("Marble019_2K-PNG_Color.png");
	seleniteTipTexture = loadTexture("Marble005_2K-PNG_Color.png");
	fabricTexture = loadTexture("Fabric046_4K-PNG_Color_Advanced.png");
	grungeTexture = loadTexture("texture_overlays_988_1_Modified.png");
	plasticTexture = loadTexture("Plastic014B_2K-PNG_Color.png");
	walmartLogoTexture = loadLogoTexture("walmartlogo.png");
	blackRubberBaseTexture = loadTexture("Rubber004_2K-PNG_Color.png");

	glGenBuffers(1, &ubo);
}

// Draws to GLFW display window
void display(GLFWwindow* window, double currentTime) { // AKA urender function in tutorial

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// adjust OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(renderingProgram); // loads compiled shaders into openGL pipeline
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	// Define the size of the UBO based on size of struct
	GLsizeiptr size = sizeof(LightData);

	// UBO memory allocation
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

	// UBO struct for lights
	LightData newData;
	newData.ambientStrength = 0.3f;
	newData.ambientColor = { 0.7f, 0.7f, 0.7f }; // White natural
	newData.light1Color = { 0.2f, 0.2f, 1.0f }; // Blue light
	newData.light1Position = { -10.0f, 5.0f, -15.0f };
	newData.light2Color = { 1.0f, 0.78823529f, 0.39215686f }; // 3900k yellow light
	newData.light2Position = { 0.0f, 8.0f, 10.0f };
	newData.specularIntensity1 = 0.1f;
	newData.specularIntensity2 = 1.0f;
	newData.highlightSize1 = 2.0f;
	newData.highlightSize2 = 6.0f;

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightData), &newData);


	// get the uniform variables for the model/view, model and projection matrices
	modelLoc = glGetUniformLocation(renderingProgram, "model"); // model only
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix"); // model-view matrix
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix"); // projection
	objectColorLoc = glGetUniformLocation(renderingProgram, "objectColor");


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

	installLights();

	//// --------------DRAWS THE PLANE-----------------
	//// The colour and the shape
	glBindVertexArray(meshes.planeMesh.vao);
	glProgramUniform4f(renderingProgram, objectColorLoc, 0.0f, 0.50196078f, 1.0f, 1.0f);
	
	mvStack.push(mvStack.top()); // Places a copy of the view matrix at top of stack to add model info to
	// 1. Places plane at origin
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Positions the plane
	
	// 2. Rotates Plane
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scales Plane
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));

	// --Model only transforms for lighting calculations--
	modelStack.push(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))); // Positions the plane
	modelStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	modelStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelStack.top()));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fabricTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fabricRoughnessTexture);

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.planeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0); // reactivates texture at loc 0
	glBindTexture(GL_TEXTURE_2D, 0); // unbinds active texture at 0


	mvStack.pop(); // Removes Plane transforms from stack
	modelStack.pop(); // Resets the model matrix for next object

	///**************************************************
	// * START of RENDERING CRYSTAL OBJECT
	// *
	// * Uses Pyramid and Cube Objects.
	// **************************************************
	// */

	//// --------------DRAWS THE PYRAMID (PARENT)-----------------
	//The colour and the shape
	glBindVertexArray(meshes.pyramid4Mesh.vao);
	glProgramUniform4f(renderingProgram, objectColorLoc, 1.0f, 0.0f, 0.50196078f, 1.0f);

	mvStack.push(mvStack.top()); // copies view matrix for manipulation

	// 1. Place Pyramid
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 2.76f, 3.0f)); // Positions the pyramid
	mvStack.push(mvStack.top()); // Copies view * (PYRAMID) position to top of stack

	// 2. Rotates Pyramid 85 degrees along the y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(85.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mvStack.push(mvStack.top()); // Copies view * PYRAMID(position * rotation) to top

	// 3. Scales Pyramid
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.25f, 0.5f));

	// --Model only transforms for lighting calculations--
	modelStack.push(glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 2.76f, 3.0f))); // Positions the pyramid
	modelStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(85.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.25f, 0.5f)); // Pyramid Scale

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelStack.top()));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// Activate texture located in 0 (samp in frag shader)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, seleniteTipTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grungeTexture);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.pyramid4Mesh.numVertices);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

	glActiveTexture(GL_TEXTURE0); // reactivates texture at loc 0
	glBindTexture(GL_TEXTURE_2D, 0); // unbinds active texture at 0
	
	glBindVertexArray(0);

	mvStack.pop(); // Removes PYRAMID scale
	modelStack.pop(); // Removes PYRAMID scale from model only matrix

	// --------------DRAWS THE CUBE (CHILD OF PYRAMID)-----------------
	// The colour and the shape
	glBindVertexArray(meshes.cubeMesh.vao);
	glProgramUniform4f(renderingProgram, objectColorLoc, 0.50196078f, 0.0f, 0.50196078f, 1.0f);

	mvStack.push(mvStack.top()); // Makes copy of PYRAMID(position * rotation)

	// 1. Place Cube relative to Pyramid
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -2.0, 0.0));

	// 2. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.75f, 0.5f));

	// --Model only transforms for lighting calculations--
	modelStack.push(glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -2.0, 0.0)));
	modelStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(85.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.75f, 0.5f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelStack.top()));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, seleniteBaseTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grungeTexture);

	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);
	glBindTexture(GL_TEXTURE_2D, 0); // unbinds active texture at 1

	glActiveTexture(GL_TEXTURE0); // reactivates texture at loc 0
	glBindTexture(GL_TEXTURE_2D, 0); // unbinds active texture at 0

	glBindVertexArray(0);

	mvStack.pop();
	mvStack.pop(); 
	mvStack.pop(); // All that remains in stack is view matrix
	modelStack.pop(); // Model stack is empty and ready for next object.

	//// **** END of RENDERING CRYSTAL OBJECT ****

	///**************************************************
	// * START of RENDERING SOLAR SYSTEM GLOBE
	// **************************************************
	// */
	glUseProgram(materialShaders);
	installMaterialShader();
	// get the uniform variables for the projection, model-view matrices
	modelLoc = glGetUniformLocation(materialShaders, "model"); // model only
	mvLoc = glGetUniformLocation(materialShaders, "mv_matrix"); // model-view matrix
	projLoc = glGetUniformLocation(materialShaders, "proj_matrix"); // projection
	objectColorLoc = glGetUniformLocation(materialShaders, "objectColor");

	// Copy model-view matrix to the uniform variable for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));



	 // --------------DRAWS THE SPHERE (PARENT)-----------------
	glBindVertexArray(meshes.sphereMesh.vao);

	mvStack.push(mvStack.top()); // copies the view matrix to the top for manipulation
	// 1. Position Sphere
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 4.2f));

	// --Model only transforms for lighting calculations--
	modelStack.push(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 4.2f)));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelStack.top()));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	//glProgramUniform4f(materialShaders, objectColorLoc, 1.0f, 0.0f, 0.50196078f, 1.0f);

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.sphereMesh.numIndices, GL_UNSIGNED_INT, (void*)0);

	// Deactivate the VAO
	glBindVertexArray(0);
	modelStack.pop();

	glUseProgram(renderingProgram);
	installLights();
	// get the uniform variables for the projection, model-view matrices
	modelLoc = glGetUniformLocation(renderingProgram, "model"); // model only
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix"); // model-view matrix
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix"); // projection
	objectColorLoc = glGetUniformLocation(renderingProgram, "objectColor");

	// Copy model-view matrix to the uniform variable for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));


	// --------------DRAWS THE TAPERED POLYGON PEDESTAL (CHILD OF SPHERE)-----------------
	// The colour and the shape
	glBindVertexArray(meshes.taperedPolygonMesh.vao);
	glProgramUniform4f(renderingProgram, objectColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);

	mvStack.push(mvStack.top()); // Copies Sphere position

	// 1. Place Cube Relative to Sphere
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.17f, 0.0f));

	// 2. Rotate Cube by 45 degrees on y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// 3. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.3f, 0.75f));

	// --Model only transforms for lighting calculations--
	modelStack.push(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.17f, 0.0f)));
	modelStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.3f, 0.75f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelStack.top()));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.taperedPolygonMesh.numVertices);
	glBindVertexArray(0);

	mvStack.pop();
	mvStack.pop(); // All that remains is the view matrix
	modelStack.pop();

	///**************************************************
	// * RENDERS THE BOOK
	// **************************************************
	// */
	// The colour and the shape
	glBindVertexArray(meshes.cubeMesh.vao);
	glProgramUniform4f(renderingProgram, objectColorLoc, 0.0f, 0.0f, 1.0f, 1.0f);

	mvStack.push(mvStack.top()); // copies view matrix for manipulation

	// 1. Position cube
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, -3.0f));

	// 2. Rotate the cube slightly clockwise
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0, 1.0f, 0.0f));

	// 3. Scale the cube to be booklike, double in width, taller in height, smaller depth
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 3.0f, 0.50f));

	// --Model only transforms for lighting calculations--
	modelStack.push(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, -3.0f)));
	//modelStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(15.0f), glm::vec3(0.0, 1.0f, 0.0f));
	modelStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 3.0f, 0.50f));


	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelStack.top()));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// Draws the cube
	//glDrawElements(GL_TRIANGLES, meshes.cubeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);
	glBindVertexArray(0);

	mvStack.pop(); // All that remains is the view matrix
	modelStack.pop();

	///**************************************************
	// * DRAWS THE MINI TRAFFIC CONE
	// **************************************************
	// */
	// // --------------DRAWS THE CONE (PARENT)-----------------
	// The colour and the shape
	glBindVertexArray(meshes.coneMesh.vao);
	glProgramUniform4f(renderingProgram, objectColorLoc, 1.0f, 0.50196078f, 0.0f, 1.0f);

	mvStack.push(mvStack.top()); // copies view matrix for manipulation

	// 1. Position the cone
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.11f, 8.0f));

	// 2. Rotate the cone
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// 3. Scale the cone
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 2.0f, 1.0f));

	// --Model only transforms for lighting calculations--
	modelStack.push(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.11f, 8.0f)));
	modelStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-65.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 2.0f, 1.0f));


	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelStack.top()));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, plasticTexture);

	// Draws the cone
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36); // bottom
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, walmartLogoTexture);
	glDrawArrays(GL_TRIANGLE_STRIP, 36, 108); // sides

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	modelStack.pop();

	// --------------DRAWS THE SQUARE BASE OF TRAFFIC CONE (CHILD OF CONE)-----------------
	// The colour and the shape
	glBindVertexArray(meshes.cubeMesh.vao);
	glProgramUniform4f(renderingProgram, objectColorLoc, 0.0f, 0.0f, 0.0f, 1.0f);

	mvStack.push(mvStack.top()); // Copies Cone position

	// 1. Place Cube Relative to cone
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// 2. Rotate Cube by 45 degrees on y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scale Cube to Cone
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 0.05f, 1.0f));

	// --Model only transforms for lighting calculations--
	modelStack.push(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	modelStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	modelStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 0.05f, 1.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelStack.top()));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blackRubberBaseTexture);

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	mvStack.pop();
	mvStack.pop(); // All that remains is the view matrix
	modelStack.pop();

	// Use lighting program for pyramids that denote the direction and source of the lights
	glUseProgram(lightedPyramidShaders);

	// get the uniform variables for the projection, model-view matrices
	modelLoc = glGetUniformLocation(lightedPyramidShaders, "model"); // model only
	mvLoc = glGetUniformLocation(lightedPyramidShaders, "mv_matrix"); // model-view matrix
	projLoc = glGetUniformLocation(lightedPyramidShaders, "proj_matrix"); // projection

	// Copy model-view matrix to the uniform variable for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// --------------DRAWS THE FIRST LIGHTING PYRAMID (KEY LIGHT)-----------------
	glBindVertexArray(meshes.pyramid4Mesh.vao);
	//glProgramUniform4f(lightingProgram, objectColorLoc, 1.0f, 0.0f, 0.50196078f, 1.0f);

	// 1. Scale object by .1 (I built my mesh with different vertices than the tutorial)
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	// 2. Rotate shape 11 degrees along x axis
	rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-11.459f), glm::vec3(1.0f, 0.0f, 0.0f));
	// 3. Place object at origin
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 5.0f, -15.0f));

	mMat = translation * rotation * scale;

	// Build the MODEL-VIEW matrix by concatenating matrixes v and m
	mvMat = vMat * mMat;

	// Copy model-view matrix to the uniform variable for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
	glBindVertexArray(0);

	// --------------DRAWS THE SECOND LIGHTING PYRAMID (FILL LIGHT)-----------------
	glBindVertexArray(meshes.pyramid4Mesh.vao);
	//glProgramUniform4f(lightingProgram, objectColorLoc, 1.0f, 0.0f, 0.50196078f, 1.0f);

	// 1. Scale object by .1 (I built my mesh with different vertices than the tutorial)
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	// 2. Rotate shape 11 degrees along x axis
	rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-11.459f), glm::vec3(1.0f, 0.0f, 0.0f));
	// 3. Place object at origin
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 8.0f, 10.0f));

	mMat = translation * rotation * scale;

	// Build the MODEL-VIEW matrix by concatenating matrixes v and m
	mvMat = vMat * mMat;

	// Copy model-view matrix to the uniform variable for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "6-5 Milestone: Lighting Complex Objects", NULL, NULL);
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
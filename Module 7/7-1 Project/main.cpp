/*
 * SNHU CS-330
 * Module 7 Final Project: Rendering a 3D Scene
 *
 * Alex Baires
 * 2-25-24
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
GLuint lightedPyramidShaders, materialShaders;

// Timing
float deltaTime = 0.0f; // time between current time and last frame
float lastFrame = 0.0f;

// Variables to be used in display() function to prevent allocation during rendering
GLuint modelLoc, projLoc, mvLoc, objectColorLoc;

// Material Variables
GLuint mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;

// Advanced lighting variables
GLuint globalAmbLoc, ambLoc, ambLoc2, diffLoc, diffLoc2, specLoc, specLoc2, posLoc, posLoc2, nLoc; //mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc, nLoc;
glm::vec3 currentLightPos, currentLightPos2, lightPosV, lightPos2V; // light position as Vector3f, in both model and view space
float lightPos[3]; // FIRST light position as float array
float lightPos2[3]; // SECOND light position as float array
glm::mat4 invTrMat; // Inverse transpose matrix for materialShaders

// initial light location
glm::vec3 initialLightLoc = glm::vec3(10.0f, 4.0f, 10.0f);
glm::vec3 initialLightLoc2 = glm::vec3(-10.0f, 5.0f, -10.0f);

// Global light
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
// front white light properties
float lightAmbient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
// rear blue light properties
float lightAmbient2[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
float lightDiffuse2[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular2[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Material variables that reflect light
float* matAmb;
float* matDif;
float* matSpe;
float matShi;

// Window variables
int width, height;

// Render matrices
glm::mat4 pMat, vMat;

// Lighting matrices to denote light sources
glm::mat4 mMat, mvMat, scale, rotation, translation;

// Texture variables;
GLuint seleniteBaseTexture, seleniteTipTexture, fabricTexture, fabricRoughnessTexture, grungeTexture, plasticTexture, walmartLogoTexture,
blackRubberBaseTexture, bookFrontTexture, bookRearTexture, bookSpineTexture, bookSideTexture, metalTexture, metalDetailTexture, cockpitTexture, wingTexture, wingTexture2,
blackPlasticTexture;

// Gordon and Clevenger Tutorial Phong implementation
void installAdvancedLights(GLuint shader, glm::mat4 vMatrix) {

	// Convert light's position to view space and save it in a float array
	lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = lightPosV.x;
	lightPos[1] = lightPosV.y;
	lightPos[2] = lightPosV.z;

	// Convert light's position to view space and save it in a float array
	lightPos2V = glm::vec3(vMatrix * glm::vec4(currentLightPos2, 1.0));
	lightPos2[0] = lightPos2V.x;
	lightPos2[1] = lightPos2V.y;
	lightPos2[2] = lightPos2V.z;

	// Get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(shader, "globalAmbient");
	ambLoc = glGetUniformLocation(shader, "light.ambient");
	ambLoc2 = glGetUniformLocation(shader, "light2.ambient");
	diffLoc = glGetUniformLocation(shader, "light.diffuse");
	diffLoc2 = glGetUniformLocation(shader, "light2.diffuse");
	specLoc = glGetUniformLocation(shader, "light.specular");
	specLoc2 = glGetUniformLocation(shader, "light2.specular");
	posLoc = glGetUniformLocation(shader, "light.position");
	posLoc2 = glGetUniformLocation(shader, "light2.position");

	// Set the uniform light and material values in the shader
	glProgramUniform4fv(shader, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(shader, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(shader, ambLoc2, 1, lightAmbient2);
	glProgramUniform4fv(shader, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(shader, diffLoc2, 1, lightDiffuse2);
	glProgramUniform4fv(shader, specLoc, 1, lightSpecular);
	glProgramUniform4fv(shader, specLoc2, 1, lightSpecular2);
	glProgramUniform3fv(shader, posLoc, 1, lightPos);
	glProgramUniform3fv(shader, posLoc2, 1, lightPos2);
}

// Sets up uniforms for reflective surfaces
void changeMaterialSurfaces(GLuint shader) {

	// Material uniforms
	mAmbLoc = glGetUniformLocation(shader, "material.ambient");
	mDiffLoc = glGetUniformLocation(shader, "material.diffuse");
	mSpecLoc = glGetUniformLocation(shader, "material.specular");
	mShiLoc = glGetUniformLocation(shader, "material.shininess");

	// Set material values in the shader
	glUniform4fv(mAmbLoc, 1, matAmb);
	glUniform4fv(mDiffLoc, 1, matDif);
	glUniform4fv(mSpecLoc, 1, matSpe);
	glUniform1f(mShiLoc, matShi);
}

// Hierarchal Matrix Stack for Parent-Child Objects
stack<glm::mat4> mvStack;

// Places application-specific initialization tasks
void init(GLFWwindow* window) {

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
	bookFrontTexture = loadTexture("HTTE.png");
	bookRearTexture = loadTexture("HTTE-REAR.png");
	bookSpineTexture = loadTexture("HTTE-Spine.png");
	bookSideTexture = loadTexture("HTTE-Sides.png");
	blackPlasticTexture = loadTexture("Porcelain.png");

	metalTexture = loadTexture("MetalPlates008_2K-PNG_Metalness.png");
	metalDetailTexture = loadTexture("MetalPlates008_2K-PNG_Color.png");
	cockpitTexture = loadTexture("TIE Fighter Texture FINAL.png");
	wingTexture = loadTexture("MetalPlates009_2K-JPG_Roughness.png");
	wingTexture2 = loadTexture("TEX00001.png");
}

// Draws to GLFW display window
void display(GLFWwindow* window, double currentTime) { // AKA urender function in tutorial
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
	currentLightPos2 = glm::vec3(initialLightLoc2.x, initialLightLoc2.y, initialLightLoc2.z);

	// adjust OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(materialShaders); // loads compiled shaders into openGL pipeline

	// get the uniform variables for the model/view, model and projection matrices
	mvLoc = glGetUniformLocation(materialShaders, "mv_matrix"); // model-view matrix
	projLoc = glGetUniformLocation(materialShaders, "proj_matrix"); // projection
	objectColorLoc = glGetUniformLocation(materialShaders, "objectColor");
	nLoc = glGetUniformLocation(materialShaders, "norm_matrix");


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

	matAmb = pearlAmbient();
	matDif = pearlDiffuse();
	matSpe = pearlSpecular();
	matShi = pearlShininess();

	installAdvancedLights(materialShaders, vMat);

	// Material variables that reflect light
	changeMaterialSurfaces(materialShaders);

	//// --------------DRAWS THE PLANE-----------------
	//// The colour and the shape
	glBindVertexArray(meshes.planeMesh.vao);
	//glProgramUniform4f(materialShaders, objectColorLoc, 0.0f, 0.50196078f, 1.0f, 1.0f);
	
	mvStack.push(mvStack.top()); // Places a copy of the view matrix at top of stack to add model info to
	// 1. Places plane at origin
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Positions the plane
	
	// 2. Rotates Plane
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scales Plane
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fabricTexture);

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.planeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	mvStack.pop(); // Removes Plane transforms from stack

	/////**************************************************
	//// * START of RENDERING CRYSTAL OBJECT
	//// *
	//// * Uses Pyramid and Cube Objects.
	//// **************************************************
	//// */

	//// --------------DRAWS THE PYRAMID (PARENT)-----------------
	//The colour and the shape
	glBindVertexArray(meshes.pyramid4Mesh.vao);
	glProgramUniform4f(materialShaders, objectColorLoc, 1.0f, 0.0f, 0.50196078f, 1.0f);

	mvStack.push(mvStack.top()); // copies view matrix for manipulation

	// 1. Place Pyramid
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 2.76f, 3.0f)); // Positions the pyramid
	mvStack.push(mvStack.top()); // Copies view * (PYRAMID) position to top of stack

	// 2. Rotates Pyramid 85 degrees along the y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mvStack.push(mvStack.top()); // Copies view * PYRAMID(position * rotation) to top

	// 3. Scales Pyramid
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	mvStack.push(mvStack.top()); // Copies scale to top

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
	mvStack.pop(); // Removes PYRAMID rotation

	// --------------DRAWS THE CUBE (CHILD OF PYRAMID)-----------------
	// The colour and the shape
	glBindVertexArray(meshes.cubeMesh.vao);
	glProgramUniform4f(materialShaders, objectColorLoc, 0.50196078f, 0.0f, 0.50196078f, 1.0f);

	mvStack.push(mvStack.top()); // Makes copy of PYRAMID(position * rotation)

	// 1. Place Cube relative to Pyramid
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -2.0, 0.0));

	// 2. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.75f, 0.5f));

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

	// **** END of RENDERING CRYSTAL OBJECT ****

	///**************************************************
	// * START of RENDERING SOLAR SYSTEM GLOBE
	// **************************************************
	// */
	//glUseProgram(materialShaders);

	//installAdvancedLights(materialShaders, vMat);
	// Material variables that reflect light
	matAmb = silverAmbient();
	matDif = silverDiffuse();
	matSpe = silverSpecular();
	matShi = silverShininess();
	changeMaterialSurfaces(materialShaders);

	 // --------------DRAWS THE SPHERE (PARENT)-----------------
	glBindVertexArray(meshes.mySphere.vao);

	mvStack.push(mvStack.top()); // copies the view matrix to the top for manipulation
	// 1. Position Sphere
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 4.2f));

	invTrMat = glm::transpose(glm::inverse(mvStack.top()));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	
	// Draw
	glDrawArrays(GL_TRIANGLES, 0, meshes.mySphere.numIndices);

	// Deactivate the VAO
	glBindVertexArray(0);

	// --------------DRAWS THE TAPERED POLYGON PEDESTAL (CHILD OF SPHERE)-----------------
	// The colour and the shape
	matAmb = bronzeAmbient();
	matDif = bronzeDiffuse();
	matSpe = bronzeSpecular();
	matShi = bronzeShininess();
	changeMaterialSurfaces(materialShaders);

	glBindVertexArray(meshes.taperedPolygonMesh.vao);

	mvStack.push(mvStack.top()); // Copies Sphere position

	// 1. Place Cube Relative to Sphere
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.17f, 0.0f));

	// 2. Rotate Cube by 45 degrees on y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// 3. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.3f, 0.75f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.taperedPolygonMesh.numVertices);
	glBindVertexArray(0);

	mvStack.pop();
	mvStack.pop(); // All that remains is the view matrix

	///**************************************************
	// * RENDERS THE BOOK
	// **************************************************
	// */

	// The colour and the shape
	matAmb = sapphireAmbient();
	matDif = sapphireDiffuse();
	matSpe = sapphireSpecular();
	matShi = sapphireShininess();
	changeMaterialSurfaces(materialShaders);

	// The colour and the shape
	glBindVertexArray(meshes.cubeMesh.vao);

	mvStack.push(mvStack.top()); // copies view matrix for manipulation

	// 1. Position cube
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.75f, -3.0f));

	// 2. Rotate the cube slightly clockwise
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0, 1.0f, 0.0f));
	mvStack.push(mvStack.top()); // Copy box position and rotation
	// 3. Scale the cube to be booklike, double in width, taller in height, smaller depth
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.20f, 2.5f, 0.30f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bookSideTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bookSideTexture);

	// Draws the cube
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	mvStack.pop(); // Removes book scale

	// // --------------RENDERS FRONT COVER (CHILD)-----------------

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copies book position and rotation

	// 1. Position plane
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.05f, 0.0f, 0.35));

	// 2. Rotate the cube slightly clockwise
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.25f, 2.75f, 0.05f));


	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blackPlasticTexture);

	// Draws the plane
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//-----

		// Material variables that reflect light
	matAmb = pearlAmbient();
	matDif = pearlDiffuse();
	matSpe = pearlSpecular();
	matShi = pearlShininess();
	changeMaterialSurfaces(materialShaders);

	glBindVertexArray(meshes.planeMesh.vao);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.04));

	// 2. Rotate the cube slightly clockwise
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// 3. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));


	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bookFrontTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bookFrontTexture);

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.planeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);

	mvStack.pop(); // Removes child transforms

	// // --------------RENDERS BACK COVER (CHILD)-----------------
	matAmb = pewterAmbient();
	matDif = pewterDiffuse();
	matSpe = pewterSpecular();
	matShi = pewterShininess();
	changeMaterialSurfaces(materialShaders);

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copies book position and rotation

	// 1. Position plane
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.05f, 0.0f, -0.35));

	// 2. Rotate the cube slightly clockwise
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.25f, 2.75f, 0.05f));


	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blackPlasticTexture);

	// Draws the plane
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//-----
	glBindVertexArray(meshes.planeMesh.vao);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.04));

	// 2. Rotate the cube slightly clockwise
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// 3. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));


	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bookRearTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bookRearTexture);

	// Draw triangles
	glDrawElements(GL_TRIANGLES, meshes.planeMesh.numIndices, GL_UNSIGNED_SHORT, NULL); // Draws triangle

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	mvStack.pop(); // Removes child transforms


	// // --------------RENDERS BOOK SPINE(CHILD)-----------------
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copies book position and rotation

	// 1. Position plane
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-1.15f, 0.0f, 0.0f));

	// 2. Rotate the cube slightly clockwise
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scale Cube
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.08, 2.75, 0.3));


	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bookSpineTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bookSpineTexture);

	// Draws the plane
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);


	mvStack.pop(); // Removes child transforms
	mvStack.pop(); // All that remains is the view matrix

	///**************************************************
	// * DRAWS THE MINI TRAFFIC CONE
	// **************************************************
	// */
	// // --------------DRAWS THE CONE (PARENT)-----------------
	// The colour and the shape
	matAmb = goldAmbient();
	matDif = goldDiffuse();
	matSpe = goldSpecular();
	matShi = goldShininess();
	changeMaterialSurfaces(materialShaders);
	glBindVertexArray(meshes.coneMesh.vao);
	//glProgramUniform4f(materialShaders, objectColorLoc, 1.0f, 0.50196078f, 0.0f, 1.0f);

	mvStack.push(mvStack.top()); // copies view matrix for manipulation

	// 1. Position the cone
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.11f, 8.0f));

	// 2. Rotate the cone
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// 3. Scale the cone
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 2.0f, 1.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, plasticTexture);

	// Draws the cone
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36); // bottom
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, walmartLogoTexture);
	glDrawArrays(GL_TRIANGLE_STRIP, 36, 108); // sides

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	// --------------DRAWS THE SQUARE BASE OF TRAFFIC CONE (CHILD OF CONE)-----------------
	// The colour and the shape
	matAmb = sapphireAmbient();
	matDif = sapphireDiffuse();
	matSpe = sapphireSpecular();
	matShi = sapphireShininess();
	changeMaterialSurfaces(materialShaders);
	glBindVertexArray(meshes.cubeMesh.vao);
	glProgramUniform4f(materialShaders, objectColorLoc, 0.0f, 0.0f, 0.0f, 1.0f);

	mvStack.push(mvStack.top()); // Copies Cone position

	// 1. Place Cube Relative to cone
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// 2. Rotate Cube by 45 degrees on y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// 3. Scale Cube to Cone
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 0.05f, 1.0f));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blackRubberBaseTexture);

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	mvStack.pop();
	mvStack.pop(); // All that remains is the view matrix

	///**************************************************
	// * DRAWS THE IMPERIAL TIE FIGHTER		|<O>|
	// **************************************************
	// */
	// // --------------DRAWS THE SPHERE COCKPIT (PARENT)-----------------
	// The colour and the shape
	glBindVertexArray(meshes.mySphere.vao);
	//glProgramUniform4f(renderingProgram, objectColorLoc, 1.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(materialShaders);

	//installAdvancedLights(materialShaders, vMat);
	// Material variables that reflect light
	matAmb = pewterAmbient();
	matDif = pewterDiffuse();
	matSpe = pewterSpecular();
	matShi = pewterShininess();
	changeMaterialSurfaces(materialShaders);

	// get the uniform variables for the projection, model-view matrices
	mvLoc = glGetUniformLocation(materialShaders, "mv_matrix"); // model-view matrix
	projLoc = glGetUniformLocation(materialShaders, "proj_matrix"); // projection
	objectColorLoc = glGetUniformLocation(materialShaders, "objectColor");
	nLoc = glGetUniformLocation(materialShaders, "norm_matrix");

	mvStack.push(mvStack.top()); // copies view matrix for manipulation

	// 1. Position the sphere
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, 2.35f, -2.0f));

	// 2. Rotate Cube by 25 degrees on y-axis
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mvStack.push(mvStack.top()); // copies sphere position
	// 2. Scale Sphere
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.75f, 0.75f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, cockpitTexture);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, meshes.mySphere.numIndices);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	//mvStack.pop(); // remove scale from child shapes

	// --------------DRAWS THE FIRST PYLON PYRAMID-----------------
	// 
	mvStack.push(mvStack.top()); // Copies position and rotation of sphere

	// The colour and the shape
	glBindVertexArray(meshes.pyramid4Mesh.vao);
	//glProgramUniform4f(renderingProgram, objectColorLoc, 1.0f, 0.0f, 0.0f, 1.0f);

	// 1. Position pyramid pylon relative to sphere cockpit
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-1.25f, 0.0f, 0.0f));
	mvStack.push(mvStack.top()); // copies position of pylon

	// 2. Rotate pyramid
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	mvStack.push(mvStack.top()); // Copy pylon rotation
	//// 3. Scale Pyramid to Sphere
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.75f, 1.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, metalDetailTexture);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.pyramid4Mesh.numVertices);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	mvStack.pop(); // removes scale of pylon.

	// --------------DRAWS THE FIRST CONNECTING TAPERED CYLINDER-----------------
	// 
		// The colour and the shape
	glBindVertexArray(meshes.taperedCylinderMesh.vao);
	//glProgramUniform4f(renderingProgram, objectColorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.10f, 0.0f));
	//mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 1.0f));


	mvStack.push(mvStack.top()); // copy cylinder position/rotation to stack

	//// 3. Scale Pyramid to Sphere
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.5f, 0.25f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, metalDetailTexture);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 72);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	mvStack.pop(); // Leaves connector position
	mvStack.pop();
	mvStack.pop(); // sphere + view
	//mvStack.pop(); // view
	//mvStack.pop();// empty

	// --------------DRAWS THE LEFT WING-----------------
	// 
	mvStack.push(mvStack.top()); // Copies position of the sphere

	// The colour and the shape
	glBindVertexArray(meshes.wingMesh.vao);
	//glProgramUniform4f(renderingProgram, objectColorLoc, 1.0f, 0.0f, 0.0f, 1.0f);

	// 1. Position wing relative to sphere
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-2.4f, 0.0f, 0.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	//3. Scale Wing to pylon
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 3.0f, 2.5f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wingTexture);

	// Draws the cube
	glDrawArrays(GL_TRIANGLES, 0, meshes.wingMesh.numVertices);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	//mvStack.pop(); //removes wing positioning FIX ME

	//---------------DRAWS FIRST LEFT WING DETAIL

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copy wing
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 1.1f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	mvStack.pop(); //removes parallel wing info
	//---------DRAWS PARALLEL WING DETAIL

	mvStack.push(mvStack.top()); // copy first left wing detail info
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f,0.76f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.55f, 0.027f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	//mvStack.pop(); //removes parallel wing info
	mvStack.pop(); // removes left wing detail info

	//---------DRAWS PARALLEL WING DETAIL

	mvStack.push(mvStack.top()); // copy first left wing detail info
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -0.76f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.55f, 0.027f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	//mvStack.pop(); //removes parallel wing info
	mvStack.pop(); // removes left wing detail info

	//---------------DRAWS SECOND LEFT WING DETAIL

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copy wing
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 1.1f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	mvStack.pop(); //removes 1st left detail positioning

	//---------DRAWS PARALLEL WING DETAIL

	mvStack.push(mvStack.top()); // copy first left wing detail info
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -0.76f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.55f, 0.027f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	//mvStack.pop(); //removes parallel wing info
	mvStack.pop(); // removes left wing detail info

	//---------DRAWS PARALLEL WING DETAIL

	mvStack.push(mvStack.top()); // copy first left wing detail info
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.76f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.55f, 0.027f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	//mvStack.pop(); //removes parallel wing info
	mvStack.pop(); // removes left wing detail info

	//---------------DRAWS THIRD LEFT WING DETAIL

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copy wing
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 1.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	//mvStack.pop(); //removes 1st left detail positioning

	//-------------DRAWS TOP OUTLINE

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top());
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 19.7f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 0.5f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	mvStack.pop(); //removes 1st left detail positioning

	//-------------DRAWS BOTTOM OUTLINE

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top());
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -19.7f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 0.5f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	mvStack.pop(); //removes 1st left detail positioning
	mvStack.pop();
	mvStack.pop(); //removes wing positioning

	//---------------DRAWS CONNECTING CYLINDER

	matAmb = silverAmbient();
	matDif = silverDiffuse();
	matSpe = silverDiffuse();
	matShi = silverShininess();
	changeMaterialSurfaces(materialShaders);

	glBindVertexArray(meshes.cylinderMesh.vao);
	mvStack.push(mvStack.top());
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-2.225f, 0.0f, 0.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.35f, 0.5f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wingTexture2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wingTexture2);

	// Draw triangles
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);

	mvStack.pop(); //removes 1st left detail positioning

	// --------------DRAWS THE SECOND PYLON PYRAMID-----------------
	// 

	matAmb = pewterAmbient();
	matDif = pewterDiffuse();
	matSpe = pewterSpecular();
	matShi = pewterShininess();
	changeMaterialSurfaces(materialShaders);

	mvStack.push(mvStack.top()); // Copies position and rotation of sphere
	// The colour and the shape
	glBindVertexArray(meshes.pyramid4Mesh.vao);
	//glProgramUniform4f(renderingProgram, objectColorLoc, 1.0f, 0.0f, 0.0f, 1.0f);

	// 1. Position pyramid pylon relative to sphere cockpit
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(1.25f, 0.0f, 0.0f));
	mvStack.push(mvStack.top()); // copies position of pylon

	// 2. Rotate pyramid
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	mvStack.push(mvStack.top()); // Copy pylon rotation
	//// 3. Scale Pyramid to Sphere
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.75f, 1.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, metalDetailTexture);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, meshes.pyramid4Mesh.numVertices);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	mvStack.pop(); // removes scale of pylon.


	// --------------DRAWS THE FIRST CONNECTING TAPERED CYLINDER-----------------
	// 
		// The colour and the shape
	glBindVertexArray(meshes.taperedCylinderMesh.vao);
	//glProgramUniform4f(renderingProgram, objectColorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.10f, 0.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 1.0f));

	mvStack.push(mvStack.top()); // copy cylinder position/rotation to stack
	//// 3. Scale Pyramid to Sphere
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.5f, 0.25f));
	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, metalDetailTexture);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 72);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	mvStack.pop(); // Leaves connector position
	mvStack.pop();
	mvStack.pop(); // sphere + view

	// --------------DRAWS THE RIGHT WING-----------------
	// 
	mvStack.push(mvStack.top()); // Copies position of the sphere

	// The colour and the shape
	glBindVertexArray(meshes.wingMesh.vao);
	//glProgramUniform4f(renderingProgram, objectColorLoc, 1.0f, 0.0f, 0.0f, 1.0f);

	// 1. Position pyramid pylon relative to sphere cockpit
	//mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.4f, 0.0f, 0.0f));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.4f, 0.0f, 0.0f));

	//2. Scale Wing to pylon
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 3.0f, 2.5f));


	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wingTexture);

	// Draws the cube
	glDrawArrays(GL_TRIANGLES, 0, meshes.wingMesh.numVertices);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	//mvStack.pop(); //removes wing positioning
	//mvStack.pop(); // Removes sphere stuff

		//---------------DRAWS FIRST LEFT WING DETAIL

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copy wing
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 1.1f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	mvStack.pop(); //removes parallel wing info
	//---------DRAWS PARALLEL WING DETAIL

	mvStack.push(mvStack.top()); // copy first left wing detail info
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.76f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.55f, 0.027f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	//mvStack.pop(); //removes parallel wing info
	mvStack.pop(); // removes left wing detail info

	//---------DRAWS PARALLEL WING DETAIL

	mvStack.push(mvStack.top()); // copy first left wing detail info
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -0.76f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.55f, 0.027f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	//mvStack.pop(); //removes parallel wing info
	mvStack.pop(); // removes left wing detail info

	//---------------DRAWS SECOND LEFT WING DETAIL

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copy wing
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(-63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 1.1f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	mvStack.pop(); //removes 1st left detail positioning

	//---------DRAWS PARALLEL WING DETAIL

	mvStack.push(mvStack.top()); // copy first left wing detail info
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -0.76f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.55f, 0.027f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	//mvStack.pop(); //removes parallel wing info
	mvStack.pop(); // removes left wing detail info

	//---------DRAWS PARALLEL WING DETAIL

	mvStack.push(mvStack.top()); // copy first left wing detail info
	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.76f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.55f, 0.027f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	//mvStack.pop(); //removes parallel wing info
	mvStack.pop(); // removes left wing detail info

	//---------------DRAWS THIRD LEFT WING DETAIL

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top()); // Copy wing
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 1.0f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	//mvStack.pop(); //removes 1st left detail positioning

	//-------------DRAWS TOP OUTLINE

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top());
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 19.7f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 0.5f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);



	mvStack.pop(); //removes 1st left detail positioning

	//-------------DRAWS BOTTOM OUTLINE

	glBindVertexArray(meshes.cubeMesh.vao);
	mvStack.push(mvStack.top());
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -19.7f, 0.0f));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 0.5f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));


	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, meshes.cubeMesh.numVertices);

	glBindVertexArray(0);

	mvStack.pop(); //removes 1st left detail positioning
	mvStack.pop();
	mvStack.pop(); //removes wing positioning

	//---------------DRAWS CONNECTING CYLINDER
	matAmb = silverAmbient();
	matDif = silverDiffuse();
	matSpe = silverSpecular();
	matShi = silverShininess();
	changeMaterialSurfaces(materialShaders);


	glBindVertexArray(meshes.cylinderMesh.vao);
	mvStack.push(mvStack.top());
	// 1. Position plane relative to wing
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.58f, 0.0f, 0.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.35f, 0.5f));

	// Copy model matrix to the uniform variables for the shaders
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wingTexture2);

	// Draw triangles
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	mvStack.pop(); //removes 1st left detail positioning

	/**************************************************************************************/
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

	// 1. Scale object by .1 (I built my mesh with different vertices than the tutorial)
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	// 2. Rotate shape 11 degrees along x axis
	rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-11.459f), glm::vec3(1.0f, 0.0f, 0.0f));
	// 3. Place object at origin
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 4.0f, 10.0f));

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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "7-2 Final Project: Rendering a 3D Scene", NULL, NULL);
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
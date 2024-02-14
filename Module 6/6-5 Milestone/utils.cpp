/*
 * utils.cpp
 *
 * Alex Baires
 * 1-21-24
 *
 */

#include "utils.h"

extern Camera camera; // decalres global camera variable from main
// Error checking functions

void printShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char* log;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;

	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);

	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

bool checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();

	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

// Reading GLSL Source from Files

string readShaderSource(const char* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";

	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint createShaderProgram(const char* vertexShader, const char* fragmentShader) { // creates vertex and fragment shaders

	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	string vertShaderStr = readShaderSource(vertexShader);
	string fragShaderStr = readShaderSource(fragmentShader);

	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER); // creates and stores integer id that points to vertex shader object
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER); // creates and stores integer id that points to fragment shader object

	glShaderSource(vShader, 1, &vertShaderSrc, NULL); // loads GLSL code into vertex shader
	glShaderSource(fShader, 1, &fragShaderSrc, NULL); // loads GLSL code into fragment shader

	glCompileShader(vShader);
	// catch errors while compiling vertex shaders
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);

	if (vertCompiled != 1) {
		cout << "Vertex Compilation Failed." << endl;
		printShaderLog(vShader);
	}
	glCompileShader(fShader);
	// catch errors while compiling fragment shaders
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);

	if (fragCompiled != 1) {
		cout << "Fragment Compilation Failed." << endl;
		printShaderLog(fShader);
	}

	GLuint vfProgram = glCreateProgram(); // creates program object

	glAttachShader(vfProgram, vShader); // attaches vertex shader to program
	glAttachShader(vfProgram, fShader); // attaches fragment shader to program

	glLinkProgram(vfProgram); // requests GLSL compiler to ensure shaders are compatible
	// catch errors while linking shaders
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);

	if (linked != 1) {
		cout << "Linking Failed." << endl;
		printProgramLog(vfProgram);
	}

	return vfProgram;
}

// SOIL2 texture functions
/* loadTexture loads seamless textures with mip-mapping and Anisotropic filtering */
GLuint loadTexture(const char* texImagePath) {
	GLuint textureID;
	textureID = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// if also anisotropic filtering
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
		glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
	}
	if (textureID == 0) cout << "could not find texture file" << texImagePath << endl;
	return textureID;
}

/* loadLogoTexture loads textures without mipmapping or anisotrophic filtering.
	This is to eliminate any artifacting/rendering errors on text.
*/
GLuint loadLogoTexture(const char* texImagePath) {
	GLuint textureID;
	textureID = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	if (textureID == 0) cout << "could not find texture file" << texImagePath << endl;
	return textureID;
}

// Process input by querying GLFW for each key press/release

void processInput(GLFWwindow* window, Camera& camera, float& deltaTime) {

	//bool keypress = false; // For events

	static const float cameraSpeed = 2.5f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float cameraOffset = cameraSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void activateOrtho(GLFWwindow* window, glm::mat4 &pMat) {
	// Orthographic matrix (If user presses P key)
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		pMat = camera.GetOrthoMatrix();
	}
}

void mousePositionCallback(GLFWwindow* window, double xPos, double yPos, Camera& camera) {
	if (gFirstMouse) {
		gLastX = xPos;
		gLastY = yPos;
		gFirstMouse = false;
	}

	float xOffset = xPos - gLastX;
	float yOffset = gLastY - yPos; // Reversed since y-coordinates go from bottom to top

	gLastX = xPos;
	gLastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);

}

void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset, Camera& camera) {
	camera.ProcessMouseScroll(yOffset);
}

// Handles mouse button events.
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods, Camera& camera) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		if (action == GLFW_PRESS)
			cout << "Left mouse button pressed" << endl;
		else
			cout << "Left mouse button released" << endl;
	}
	break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
	{
		if (action == GLFW_PRESS)
			cout << "Middle mouse button pressed" << endl;
		else
			cout << "Middle mouse button released" << endl;
	}
	break;

	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		if (action == GLFW_PRESS)
			cout << "Right mouse button pressed" << endl;
		else
			cout << "Right mouse button released" << endl;
	}
	break;

	default:
		cout << "Unhandled mouse button event" << endl;
		break;
	}
}

// Wrapper functions
void glfwMousePositionCallbackWrapper(GLFWwindow* window, double xPos, double yPos) {
	mousePositionCallback(window, xPos, yPos, camera);
}

void glfwMouseScrollCallbackWrapper(GLFWwindow* window, double xOffset, double yOffset) {
	mouseScrollCallback(window, xOffset, yOffset, camera);
}

void glfwMouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods) {
	mouseButtonCallback(window, button, action, mods, camera);
}
// glfw - function executes when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
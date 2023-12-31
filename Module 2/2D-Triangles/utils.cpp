/*
 * utils.cpp
 *
 * Alex Baires
 * 1-21-24
 *
 */

#include "utils.h"

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

GLuint createShaderProgram() { // creates vertex and fragment shaders

	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	string vertShaderStr = readShaderSource("vertShader.glsl");
	string fragShaderStr = readShaderSource("fragShader.glsl");

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

// Process input by querying GLFW for each key press/release

void UProcessInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

// glfw - function executes when window is resized
void UResizeWindow(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Creates the mesh
void UCreateMesh(GLmesh& mesh) {

	// Specifies Normalized Device Coordinates for triangle vertices
	GLfloat verts[] =
	{	
		// The two triangles will be drawn using indices.
		// Left triangle indices: 0, 1, 2
		// Right triangle indices: 3, 2, 4

		// index 0
		-1.0f, 1.0f, 0.0f,	// top-first third of screen
		1.0f, 0.0f, 0.0f, 1.0f, // red

		// index 1
		-1.0f, 0.0f, 0.0f, // bottom-left of screen
		0.0f, 0.0f, 1.0f, 1.0f, // blue

		// index 2
		-0.5f, 0.0f, 0.0f,	// bottom-center of screen
		0.0f, 1.0f, 0.0f, 1.0f, // green

		// index 3
		0.0f, 0.0f, 0.0f,	// top-second third of the screen
		1.0f, 0.0f, 0.0f, 1.0f, // red

		// index 4
		0.0f, -1.0f, 0.0f,	// bottom-right of screen
		0.0f, 1.0f, 0.0f, 1.0f // green
	};

	glGenVertexArrays(1, &mesh.vao); // can also generate multiple VAOs or buffers at once
	glBindVertexArray(mesh.vao);

	glGenBuffers(2, mesh.vbos); // Create 1 single buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data to GPU

	// Creates a buffer object for the indices
	GLushort indices[] = { 0, 1, 2, 3, 2, 4 }; // Using idex 2 twice
	mesh.numIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Creates the Vertex Attribute Pointer for the screen coordinates
	const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
	const GLuint floatsPerColor = 4; // (r, g, b, a)

	// Strides between vertex coordinates is 6 (xyz rgba)
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor);

	// Instructs the GPU on how to handle the vbo data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, ie x and y | data type | deactivate normalization | 0
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// Parameters: attribPointerPosition 1 | floats per color is 4, ie rgba | data type | deactivate normalization 
	// | 6 strides till next color | 2 floats til beginning of each color
	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);
}

void UDestroyMesh(GLmesh& mesh) {

	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(2, mesh.vbos);
}
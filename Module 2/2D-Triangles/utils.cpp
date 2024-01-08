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
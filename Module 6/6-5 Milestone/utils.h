#pragma once
/*
 * utils.h
 * 
 * Reusable OpenGL functions for error checking and reading GLSL shader programs
 * 
 * 
 * 
 * Adapted from:
 * Computer Graphics Programming in OpenGL with C++
 * By Gordon and Clevenger
 * 
 */

#ifndef UTILS_H
#define UTILS_H

#include <SOIL2/SOIL2.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "meshes.h"
#include "Camera.h"
#include <string>
#include <iostream>
#include <fstream>


#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	// Camera Control Axes
	// Location
	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0f;
	bool gFirstMouse = true;
}


using namespace std;

void printShaderLog(GLuint shader); // display contents of OpenGL log when GLSL compilation failed

void printProgramLog(int prog); // display contents of OpenGL log when GLSL linking failed

bool checkOpenGLError(); // checks for occurrence of an openGL error.

string readShaderSource(const char* filePath);

GLuint createShaderProgram(const char* vertexShader, const char* fragmentShader); // creates vertex and fragment shaders

GLuint createShaderProgram(const char* vp, const char* fp);

//GLuint createShaderProgram(const char* vp, const char* gp, const char* fp);

// Texture functions
GLuint loadTexture(const char* texImagePath);

GLuint loadLogoTexture(const char* texImagePath);

void processInput(GLFWwindow* window, Camera& camera, float& deltaTime);

void activateOrtho(GLFWwindow* window, glm::mat4 &pMat);

void mousePositionCallback(GLFWwindow* window, double xPos, double yPos, Camera& camera);

void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset, Camera& camera);

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods, Camera& camera);

// Wrapper functions for mouse
void glfwMousePositionCallbackWrapper(GLFWwindow* window, double xPos, double yPos);
void glfwMouseScrollCallbackWrapper(GLFWwindow* window, double xOffset, double yOffset);
void glfwMouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods);

// Materials ADS
float* goldAmbient();
float* goldDiffuse();
float* goldSpecular();
float goldShininess();

float* silverAmbient();
float* silverDiffuse();
float* silverSpecular();
float silverShininess();

float* bronzeAmbient();
float* bronzeDiffuse();
float* bronzeSpecular();
float bronzeShininess();

float* pearlAmbient();
float* pearlDiffuse();
float* pearlSpecular();
float pearlShininess();

float* jadeAmbient();
float* jadeDiffuse();
float* jadeSpecular();
float jadeShininess();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif /* UTILS_H */
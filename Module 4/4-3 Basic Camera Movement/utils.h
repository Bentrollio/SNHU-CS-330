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

#include "GLmesh.h"
#include <string>
#include <iostream>
#include <fstream>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

void printShaderLog(GLuint shader); // display contents of OpenGL log when GLSL compilation failed

void printProgramLog(int prog); // display contents of OpenGL log when GLSL linking failed

bool checkOpenGLError(); // checks for occurrence of an openGL error.

string readShaderSource(const char* filePath);

GLuint createShaderProgram(); // creates vertex and fragment shaders

GLuint createShaderProgram(const char* vp, const char* fp);

GLuint createShaderProgram(const char* vp, const char* gp, const char* fp);

void processInput(GLFWwindow* window, float &deltaTime, glm::vec3 &cameraPos, glm::vec3 &cameraFront, glm::vec3 &cameraUp);

void mousePositionCallback(GLFWwindow* window, double xPos, double yPos);

void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif /* UTILS_H */
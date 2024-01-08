#pragma once
/*
 * utils.h
 * 
 * Reusable OpenGL functions for error checking and reading GLSL shader programs
 * 
 * Adapted from:
 * Computer Graphics Programming in OpenGL with C++
 * By Gordon and Clevenger
 * 
 */

#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void printShaderLog(GLuint shader); // display contents of OpenGL log when GLSL compilation failed

void printProgramLog(int prog); // display contents of OpenGL log when GLSL linking failed

bool checkOpenGLError(); // checks for occurrence of an openGL error.

string readShaderSource(const char* filePath);

GLuint createShaderProgram(); // creates vertex and fragment shaders

GLuint createShaderProgram(const char* vp, const char* fp);

GLuint createShaderProgram(const char* vp, const char* gp, const char* fp);

#endif /* UTILS_H */
/*
 * SNHU CS-330 
 * Module 8 Assignment - Coding Collisions
 * 
 * Alex Baires
 * 3-3-2024
 * 
 */


#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>

using namespace std;

const float DEG2RAD = 3.14159 / 180;

void processInput(GLFWwindow* window);

enum BRICKTYPE { REFLECTIVE, DESTRUCTABLE, PADDLE };
enum ONOFF { ON, OFF };

class Brick
{
public:
	float red, green, blue;
	float x, y, width;
	int hp;
	BRICKTYPE brick_type;
	ONOFF onoff;

	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
	{
		brick_type = bt; x = xx; y = yy, width = ww; red = rr, green = gg, blue = bb;
		onoff = ON;
		hp = 2;
	};

	void drawBrick()
	{
		if (onoff == ON)
		{
			double halfside = width / 2;

			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(x + width, y + halfside);
			glVertex2d(x + width, y - halfside);
			glVertex2d(x - width, y - halfside);
			glVertex2d(x - width, y + halfside);

			glEnd();
		}
	}
};

class Circle
{
public:
	float red, green, blue;
	float radius;
	float x;
	float y;
	float speed = 0.01;
	int direction; // 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left
	int hp;
	ONOFF onoff;

	Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
	{
		x = xx;
		y = yy;
		radius = rr;
		red = r;
		green = g;
		blue = b;
		radius = rad;
		direction = dir;
		onoff = ON;
		hp = 5;
	}

	void CheckCollision(Brick* brk)
	{
		if (brk->brick_type == REFLECTIVE && onoff == ON)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				direction = GetRandomDirection();
				x = x + 0.05;
				y = y + 0.025;
				--hp;
				if (hp <= 0) {
					onoff = OFF;
				}
			}
		}
		else if (brk->brick_type == DESTRUCTABLE && onoff == ON && brk->hp > 0 && onoff == ON)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				brk->hp--;
				if (brk->hp <= 0) {

					brk->onoff = OFF;
				}
				brk->green = 1.0;

				direction = GetRandomDirection();
				x = x + 0.05;
				y = y + 0.025;
				onoff = OFF;
			}
		}
		else if (brk->brick_type == PADDLE && onoff == ON)
		{
			if ((x > brk->x - brk->width && x <= brk->x + brk->width) && (y > brk->y - brk->width && y <= brk->y + brk->width))
			{
				direction = GetRandomDirection();
				x = x + 0.05;
				y = y + 0.025;
			}
		}

	}

	int GetRandomDirection()
	{
		return (rand() % 8) + 1;
	}

	void MoveOneStep()
	{

		if (direction == 1 || direction == 5 || direction == 6)  // down
		{
			if (y > -1 + radius)
			{
				y -= speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}

		if (direction == 2 || direction == 5 || direction == 7)  // right
		{
			if (x < 1 - radius)
			{
				x += speed;
			}
			else
			{
				direction = GetRandomDirection();

			}
		}

		if (direction == 3 || direction == 7 || direction == 8)  // up
		{
			if (y < 1 - radius) {
				y += speed;
			}
			else
			{
				direction = GetRandomDirection();

			}
		}

		if (direction == 4 || direction == 6 || direction == 8)  // left
		{
			if (x > -1 + radius) {
				x -= speed;
			}
			else
			{
				direction = GetRandomDirection();
			}
		}
	}

	void DrawCircle()
	{
		if (onoff == ON)
		{
			glColor3f(red, green, blue);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 360; i++) {
				float degInRad = i * DEG2RAD;
				glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
			}
			glEnd();
		}
	}
};


vector<Circle> world;
Brick paddle(PADDLE, 0, -0.99, 0.1, 1, 0, 0);

int main(void) {
	srand(time(NULL));

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(480, 480, "Random World of Circles", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Main row of bricks
	Brick brick(DESTRUCTABLE, 0.8, 0.9, 0.2, 1, 0, 0);
	Brick brick2(DESTRUCTABLE, 0.4, 0.9, 0.2, 1, 0, 0);
	Brick brick3(DESTRUCTABLE, 0.0, 0.9, 0.2, 1, 0, 0);
	Brick brick4(DESTRUCTABLE, 0, 0.9, 0.2, 1, 0, 0);
	Brick brick5(DESTRUCTABLE, -0.4, 0.9, 0.2, 1, 0, 0);
	Brick brick6(DESTRUCTABLE, -0.8, 0.9, 0.2, 1, 0, 0);

	// Obstacle bricks
	Brick obstacle(REFLECTIVE, 0.4, 0.0, 0.2, 0.0, 0.0, 1.0);
	Brick obstacle2(REFLECTIVE, -0.4, 0.0, 0.2, 0.0, 0.0, 1.0);

	while (!glfwWindowShouldClose(window)) {
		//Setup View
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window);

		//Movement
		for (int i = 0; i < world.size(); i++)
		{
			world[i].CheckCollision(&brick);
			world[i].CheckCollision(&brick2);
			world[i].CheckCollision(&brick3);
			world[i].CheckCollision(&brick4);
			world[i].CheckCollision(&brick5);
			world[i].CheckCollision(&brick6);
			world[i].CheckCollision(&obstacle);
			world[i].CheckCollision(&obstacle2);


			world[i].CheckCollision(&paddle);

			world[i].MoveOneStep();
			world[i].DrawCircle();

			for (int j = 0; j < world.size(); j++)
			{

				Circle* ptrA = &world[i];
				Circle* ptrB = &world[j];

				if (ptrA != ptrB)
				{
					if (((world[i].x > world[j].x - world[j].radius && world[i].x <= world[j].x + world[j].radius)
						&& (world[i].y > world[j].y - world[j].radius && world[i].y <= world[j].y + world[j].radius)))
					{
						world[i].direction = world[i].onoff = OFF;
						world[j].direction = world[j].onoff = ON;
					}
				}
			}

		}
		brick.drawBrick();
		brick2.drawBrick();
		brick3.drawBrick();
		brick4.drawBrick();
		brick5.drawBrick();
		brick6.drawBrick();
		obstacle.drawBrick();
		obstacle2.drawBrick();
		paddle.drawBrick();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate;
	exit(EXIT_SUCCESS);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static bool circleExists = false;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !circleExists)
	{
		double r, g, b;
		r = rand() / 10000;
		g = rand() / 10000;
		b = rand() / 10000;
		Circle B(paddle.x, paddle.y, 02, 2, 0.05, r, g, b); // circle spawns from paddle
		world.push_back(B);
		circleExists = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		circleExists = false;
	}

	// THE PADDLER

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		paddle.x = (paddle.x - .01);
		if (paddle.x < -1) {
			paddle.x += .01;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		paddle.x = (paddle.x + .01);
		if (paddle.x > 1) {
			paddle.x -= .01;
		}
	}

}
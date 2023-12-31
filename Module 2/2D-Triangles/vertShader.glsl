#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 colorFromVBO;
out vec4 colorFromVS;
void main(void)
{ 
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	colorFromVS = colorFromVBO;
}
#version 430 core
in vec4 colorFromVS;
out vec4 FragColor;
void main(void)
{ 
	FragColor = colorFromVS;
}
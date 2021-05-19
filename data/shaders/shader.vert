#version 330 core

in vec3 position;

uniform mat4 m,v,p;

void main() {
	//gl_Position = p * v * m * vec4(position, 1.0);
	gl_Position = vec4(position, 1.0);
}
#version 330 core

in vec3 position;

in vec3 tmp1;
in vec3 tmp2;
in vec3 tmp3;
in vec3 tmp4;
in vec3 tmp5;

uniform mat4 m,v,p;

void main() {
	gl_Position = p * v * m * vec4(position, 1.0);
}
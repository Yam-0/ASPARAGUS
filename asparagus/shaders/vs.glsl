#version 330 core

uniform vec2 dposition;

in vec2 position;

void main()
{
	gl_Position = vec4(position.x + dposition.x, position.y - dposition.y, 0.0, 1.0);
}
#version 330 core

uniform vec3 uniColor;

out vec4 outColor;

void main()
{
    outColor = vec4(uniColor, 1.0);
}
#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 cameraPosition;
layout (location = 2) in vec3 cameraRotation;
layout (location = 3) in vec3 objectPosition;
layout (location = 4) in vec3 objectScale;
layout (location = 5) in vec3 objectRotation;

uniform float depth;
uniform float fov;
uniform vec2 windowSize;

vec3 rotateVector(float a, vec3 vector, int axis);
vec2 projectPoint(vec3 p, float fov, float depth);
float map(float value,
		   float x1, float y1,
		   float x2, float y2);

void main()
{
	//New position
	vec3 npos = vertexPosition;

	/* ORIGIN RELATIVE SCALING */
	npos *= objectScale;

	/* ORIGIN RELATIVE ROTATION */
	npos = rotateVector(-objectRotation.x, npos, 0);
	npos = rotateVector(-objectRotation.z, npos, 2);
	//npos = rotateVector(-objectRotation.y, npos, 1);

	/* MOVING TO CAMERA RELATIVE POSITION */
	npos = npos - cameraPosition + objectPosition;

	/* AXIS ROTATION AROUND CAMERA */
	npos = rotateVector(cameraRotation.z, npos, 2);
	npos = rotateVector(-cameraRotation.x, npos, 0);
	//npos = rotateVector(cameraRotation.y, npos, 1);

	vec2 ppos = projectPoint(npos, fov, depth);
	gl_Position = vec4(ppos.x, ppos.y, 0.0, 1.0);
	//gl_Position = vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0);
}

vec3 rotateVector(float a, vec3 vector, int axis)
{
	vec3 tempvector = vector;

	switch (axis)
	{
	case 0: //x
		tempvector.x = vector.x;
		tempvector.z = cos(a) * vector.z - sin(a) * vector.y;
		tempvector.y = sin(a) * vector.z + cos(a) * vector.y;
		break;
	case 1: //y
		//tempvector.x = cosf(a) * vector.z - sinf(a) * vector.x;
		//tempvector.y = vector.y;
		//tempvector.z = sinf(a) * vector.z + cosf(a) * vector.x;
		break;
	case 2: //z
		tempvector.x = cos(a) * vector.x - sin(a) * vector.y;
		tempvector.y = sin(a) * vector.x + cos(a) * vector.y;
		tempvector.z = vector.z;
		break;

	default:
		break;
	}

	return tempvector;
}

vec2 projectPoint(vec3 p, float _fov, float _depth)
{
	float dist = sqrt(p.y * p.y + p.z * p.z + p.x * p.x);
	dist = dist == 0 ? 0.0001 : dist;
	float r = (_depth / dist);
	p *= r;
	float ax = atan(p.z/p.y);
	float az = atan(p.x/p.y);
	float ssx = map(az, _fov / 2, -_fov / 2, -1.0, 1.0);
	float ssy = map(ax, _fov / 2, -_fov / 2, -1.0, 1.0);
	return vec2(ssx, ssy);
}

float map(float value,
		   float x1, float y1,
		   float x2, float y2)
{
	float number = x2 + (y2 - x2) * ((value - x1) / (y1 - x1));

	return number;
}
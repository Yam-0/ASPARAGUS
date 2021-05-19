#ifndef ASP_CAMERA_H
#define ASP_CAMERA_H

#include "util.h"

struct ASP_Camera
{
	mat4 view, proj;
	ASP_Entity *parentObject;

	bool attached;
	float fov, aspect;
	float pitch, yaw;
	float z_near, z_far;
};

struct ASP_Camera ASP_CreateCamera(float fov, ASP_Entity *parentObject);
void ASP_AttachCamera(struct ASP_Camera *camera, ASP_Entity *entity);
void ASP_UpdateCamera(struct ASP_Camera *camera);

#endif
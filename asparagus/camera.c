#include "include/util.h"
#include "include/camera.h"
#include "include/window.h"

struct ASP_Camera ASP_Camera_Create(float fov, ASP_Entity *parentObject)
{
	struct ASP_Camera camera;

	memset(&camera, 0, sizeof(struct ASP_Camera));
	camera.fov = fov;
	camera.aspect = (float)window.screenSize.x / (float)window.screenSize.y;
	camera.parentObject = parentObject;
	camera.z_near = 0.01f;
	camera.z_far = 1000.0f;

	ASP_Camera_Update(&camera);
	return camera;
}

void ASP_Camera_Attach(struct ASP_Camera *camera, ASP_Entity *entity)
{
	camera->parentObject = entity;
	camera->attached = ASP_TRUE;
	return;
}

void ASP_Camera_Update(struct ASP_Camera *camera)
{
	float pdir[3], pp[3];
	if (camera->attached)
	{
		pdir[0] = cosf(camera->parentObject->rotation.x) * sinf(camera->parentObject->rotation.z);
		pdir[1] = sinf(camera->parentObject->rotation.x);
		pdir[2] = cosf(camera->parentObject->rotation.x) * cosf(camera->parentObject->rotation.z);

		pp[0] = camera->parentObject->position.x;
		pp[1] = camera->parentObject->position.z;
		pp[2] = camera->parentObject->position.y;
	}

	glm_normalize(pdir);

	float cpright[3];
	float cpup[3];
	glm_vec3_cross((vec3){0.0f, 1.0f, 0.0f}, pdir, cpright);
	glm_vec3_cross(pdir, cpright, cpup);
	glm_mat4_identity(camera->view);
	glm_mat4_identity(camera->proj);

	float wpp[3];
	glm_vec3_add(pp, pdir, wpp);
	glm_lookat(pp, wpp, cpup, camera->view);
	glm_perspective(camera->fov, camera->aspect, camera->z_near, camera->z_far, camera->proj);
	return;
}

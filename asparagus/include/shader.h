#ifndef ASP_SHADER_H
#define ASP_SHADER_H

#include "util.h"
#include "camera.h"

struct ASP_VertexAttribute
{
	GLuint index;
	const GLchar *name;
};

struct ASP_Shader
{
	GLuint shader_handle, vs_handle, fs_handle;
};

struct ASP_Shader ASP_CreateShader(char *filepath_vs, char *filepath_fs, size_t n, struct ASP_VertexAttribute attr[]);
void ASP_DestroyShader(struct ASP_Shader shader);
void ASP_BindShader(struct ASP_Shader shader);
void ASP_Mat4f_uniform(struct ASP_Shader shader, char *name, mat4 m);
void ASP_Mat4f_camera(struct ASP_Shader shader, struct ASP_Camera *camera);

#endif
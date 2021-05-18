#ifndef ASP_VAO_H
#define ASP_VAO_H

#include "util.h"
#include "vbo.h"

struct ASP_VAO
{
	GLuint object_handle;
};

struct ASP_VAO ASP_VAO_Create();

void ASP_VAO_Destroy(struct ASP_VAO object);
void ASP_VAO_Bind(struct ASP_VAO object);
void ASP_VAO_Attribute(
	struct ASP_VAO vao_object, struct ASP_VBO vbo_object,
	GLuint size, GLenum type, GLsizei stride, size_t offset);

#endif
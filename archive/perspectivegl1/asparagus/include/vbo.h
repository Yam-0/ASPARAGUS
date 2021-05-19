#ifndef ASP_VBO_H
#define ASP_VBO_H

#include "util.h"

struct ASP_VBO
{
	GLuint object_handle;
	GLint object_type;
	bool isDynamic;
};

struct ASP_VBO ASP_VBO_Create(GLint object_type, bool isDynamic);

void ASP_VBO_Destroy(struct ASP_VBO object);
void ASP_VBO_Bind(struct ASP_VBO object);
void ASP_VBO_Buffer(struct ASP_VBO object, void *data, size_t offset, size_t n);

#endif
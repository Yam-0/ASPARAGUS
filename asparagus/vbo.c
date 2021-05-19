#include "include/vbo.h"

struct ASP_VBO ASP_VBO_Create(GLint object_type, bool isDynamic)
{
	struct ASP_VBO object;
	object.object_type = object_type;
	object.isDynamic = isDynamic;
}

void ASP_VBO_Destroy(struct ASP_VBO object)
{
	glDeleteBuffers(1, object.object_handle);
}

void ASP_VBO_Bind(struct ASP_VBO object)
{
	glBindBuffer(object.object_type, object.object_handle);
}

void ASP_VBO_Buffer(struct ASP_VBO object, void *data, size_t offset, size_t n)
{
	ASP_VBO_Bind(object);
	glBufferData(object.object_type, n - offset, data, object.isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}
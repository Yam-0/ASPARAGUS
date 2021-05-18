#include "include/vbo.h"

struct ASP_VBO ASP_VBO_Create(GLint object_type, bool isDynamic)
{
	struct ASP_VBO object;
	object.object_type = object_type;
	object.isDynamic = isDynamic;
}

void ASP_VBO_Destroy(struct ASP_VBO object)
{
}

void ASP_VBO_Bind(struct ASP_VBO object)
{
}

void ASP_VBO_Buffer(struct ASP_VBO object, void *data, size_t offset, size_t n)
{
}
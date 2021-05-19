#include "include/vao.h"

struct ASP_VAO ASP_VAO_Create()
{
	struct ASP_VAO object;
	glGenVertexArrays(1, &object.object_handle);
	return object;
}

void ASP_VAO_Destroy(struct ASP_VAO object)
{
	glDeleteVertexArrays(1, &object.object_handle);
}

void ASP_VAO_Bind(struct ASP_VAO object)
{
	glBindVertexArray(object.object_handle);
}

void ASP_VAO_Attribute(
	struct ASP_VAO vao_object, struct ASP_VBO vbo_object,
	GLuint size, GLenum type, GLsizei stride, size_t offset)
{
	ASP_VAO_Bind(vao_object);
	ASP_VBO_Bind(vbo_object);
	glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void *)offset);
}
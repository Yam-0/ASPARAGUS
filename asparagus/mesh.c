#include "include/mesh.h"

void ASP_Mesh_Init(struct ASP_Mesh *object, size_t db_size, size_t ib_size, size_t if_size)
{
	object->attached = false;

	memset(object, 0, sizeof(struct ASP_Mesh));
	object->vao = ASP_VAO_Create();
	object->vbo = ASP_VBO_Create(GL_ARRAY_BUFFER, GL_FALSE);
	object->ibo = ASP_VBO_Create(GL_ELEMENT_ARRAY_BUFFER, GL_FALSE);

	struct ASP_MeshBuffer *buffers[3] = {
		&object->data, &object->indices, &object->faces};

	buffers[0]->capacity = db_size;
	buffers[1]->capacity = ib_size;
	buffers[2]->capacity = if_size;
}

void ASP_Mesh_Attach(struct ASP_Mesh *object, ASP_Entity *entity)
{
	object->parentObject = entity;
	object->attached = true;
}

void ASP_Mesh_Destroy(struct ASP_Mesh *object)
{
	free(&object->data);
	ASP_VAO_Destroy(object->vao);
	ASP_VBO_Destroy(object->vbo);
	ASP_VBO_Destroy(object->ibo);
}

void ASP_Mesh_Render(struct ASP_Mesh *object, struct ASP_Camera *camera)
{
	ASP_BindShader(state.shader);
	ASP_Mat4f_camera(state.shader, camera);

	mat4 mm;
	glm_mat4_identity(mm);

	if (object->attached)
	{
		glm_translate(mm, ((ivec3){
							  object->parentObject->position.x,
							  object->parentObject->position.y,
							  object->parentObject->position.z}));
	}

	ASP_Mat4f_uniform(state.shader, "m", mm);

	const size_t vertex_size = object->vertex_count * sizeof(float);
	ASP_VAO_Attribute(object->vao, object->vbo, 0, 3, vertex_size, 0 * sizeof(float));
	ASP_VAO_Attribute(object->vao, object->vbo, 1, 2, vertex_size, 3 * sizeof(float));
	ASP_VAO_Attribute(object->vao, object->vbo, 2, 3, vertex_size, 5 * sizeof(float));

	ASP_VAO_Bind(object->vao);
	ASP_VBO_Bind(object->ibo);
	glDrawElements(GL_TRIANGLES, object->indices.count, GL_UNSIGNED_SHORT, NULL);
}

void ASP_Mesh_Update(struct ASP_Mesh *object)
{
}

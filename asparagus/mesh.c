#include "include/mesh.h"

void ASP_Mesh_Init(struct ASP_Mesh *object)
{
	if(!object)
	{
		printf("Nullpointer in mesh init.\n");
		return;
	}

	object->parentObject = NULL;
	object->attached = false;

	object->vao = ASP_VAO_Create();
	object->vbo = ASP_VBO_Create(GL_ARRAY_BUFFER, GL_FALSE);
	object->ibo = ASP_VBO_Create(GL_ELEMENT_ARRAY_BUFFER, GL_FALSE);

	return;
}

void ASP_Mesh_Attach(struct ASP_Mesh *mesh, ASP_Entity *entity)
{
	if (!mesh || !entity)
	{
		if (!mesh)
			printf("Tried to attach nullpointer to entity\n");
		if (!entity)
			printf("Tried to attach mesh to nullpointer\n");

		return;
	}

	entity->mesh = mesh;
	mesh->parentObject = entity;
	mesh->attached = true;
	printf("Attached to entity: %s\n", entity->name);

	return;
}

void ASP_Mesh_Destroy(struct ASP_Mesh *object)
{
	if(!object)
	{
		printf("Nullpointer in mesh destroy.\n");
		return;
	}

	ASP_VAO_Destroy(object->vao);
	ASP_VBO_Destroy(object->vbo);
	ASP_VBO_Destroy(object->ibo);

	return;
}

void ASP_Mesh_Render(struct ASP_Mesh *object, struct ASP_Camera *camera)
{
	ASP_Shader_Bind(state.shader);
	ASP_Mat4f_camera(state.shader, camera);

	mat4 mm;
	glm_mat4_identity(mm);

	if (object->attached)
	{
		glm_translate(mm, ((vec3){
							  object->parentObject->position.x,
							  object->parentObject->position.z,
							  object->parentObject->position.y}));

		glm_rotate(mm, object->parentObject->rotation.x, ((vec3){1.0f, 0.0f, 0.0f}));
		glm_rotate(mm, object->parentObject->rotation.z, ((vec3){0.0f, 1.0f, 0.0f}));
		glm_rotate(mm, object->parentObject->rotation.y, ((vec3){0.0f, 0.0f, 1.0f}));

		glm_scale(mm, ((vec3){
						  object->parentObject->scale.x,
						  object->parentObject->scale.z,
						  object->parentObject->scale.y}));
	}

	if (ASPMP_M1)
		ASP_Mat4f_Print(mm[0]);

	ASP_Mat4f_uniform(state.shader, "m", mm[0]);

	size_t stride = 3 * sizeof(float);
	ASP_VAO_Attribute(object->vao, object->vbo, 0, 3, GL_FLOAT, stride, 0 * sizeof(float));
	ASP_VAO_Attribute(object->vao, object->vbo, 1, 3, GL_FLOAT, stride, 3 * sizeof(float));
	ASP_VAO_Attribute(object->vao, object->vbo, 2, 2, GL_FLOAT, stride, 6 * sizeof(float));

	ASP_Mesh_Update(object);

	ASP_VAO_Bind(object->vao);
	ASP_VBO_Bind(object->ibo);
	glDrawElements(GL_TRIANGLES, object->index_count * 3, GL_UNSIGNED_INT, 0);

	return;
}

void ASP_Mesh_Update(struct ASP_Mesh *object)
{
	glBindVertexArray(object->vao.object_handle);
	ASP_VBO_Buffer(object->vbo, object->vertices, 0, sizeof(float) * object->vertex_count * 3);
	ASP_VBO_Buffer(object->ibo, object->indices, 0, sizeof(float) * object->index_count * 3);

	return;
}

void ASP_Mesh_GenerateSquare(struct ASP_Mesh *object)
{
	object->vertex_count = 8;
	object->index_count = 12;

	float *vertices = (float *)malloc(object->vertex_count * sizeof(float) * 3);
	int *indices = (int *)malloc(object->index_count * sizeof(int) * 3);

	float _vertices[] = {
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f};

	int _indices[] = {
		0, 1, 2,
		2, 3, 0,
		1, 5, 6,
		6, 2, 1,
		7, 6, 5,
		5, 4, 7,
		4, 0, 3,
		3, 7, 4,
		4, 5, 1,
		1, 0, 4,
		3, 2, 6,
		6, 7, 3};

	for (int i = 0; i < object->vertex_count; i++)
	{
		vertices[i * 3] = _vertices[i * 3];
		vertices[i * 3 + 1] = _vertices[i * 3 + 1];
		vertices[i * 3 + 2] = _vertices[i * 3 + 2];
	}
	for (int i = 0; i < object->index_count; i++)
	{
		indices[i * 3] = _indices[i * 3];
		indices[i * 3 + 1] = _indices[i * 3 + 1];
		indices[i * 3 + 2] = _indices[i * 3 + 2];
	}

	object->vertices = vertices;
	object->indices = indices;

	return;
}

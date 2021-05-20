#include "include/mesh.h"

void ASP_Mesh_Init(struct ASP_Mesh *object)
{
	object->parentObject = NULL;
	object->attached = false;

	object->vao = ASP_VAO_Create();
	object->vbo = ASP_VBO_Create(GL_ARRAY_BUFFER, GL_FALSE);
	object->ibo = ASP_VBO_Create(GL_ELEMENT_ARRAY_BUFFER, GL_FALSE);

	object->vertices.capacity = 0;
	object->faces.capacity = 0;
	object->indices.capacity = 0;
}

void ASP_Mesh_Attach(struct ASP_Mesh *mesh, ASP_Entity *entity)
{
	entity->mesh = mesh;
	mesh->parentObject = entity;
	mesh->attached = true;
	printf("Attached to entity: %s", entity->name);
}

void ASP_Mesh_Destroy(struct ASP_Mesh *object)
{
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

	//TEMP SOLUTION
	//-------------------------------------------------------------
	float vertices[] = {
		0.5f, 0.5f, 0.0f,	// top right
		0.5f, -0.5f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f	// top left
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};

	glBindVertexArray(object->vao.object_handle);

	ASP_VBO_Bind(object->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ASP_VBO_Bind(object->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//-------------------------------------------------------------

	ASP_VAO_Bind(object->vao);
	ASP_VBO_Bind(object->ibo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ASP_Mesh_Update(struct ASP_Mesh *object)
{
}

void ASP_Mesh_FillSquare(struct ASP_Mesh *object)
{
	struct ASP_MeshBuffer vertices, faces, indices;

	vertices.capacity = 3 * 8 * sizeof(int);
	faces.capacity = 3 * 12 * sizeof(int);
	indices.capacity = 1 * sizeof(int);

	vertices.count = 8;
	faces.count = 12;
	indices.count = 1;

	vertices.index = sizeof(float);
	faces.index = sizeof(int);
	indices.index = sizeof(int);

	float _vertices[3 * 8] = {
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f};

	int _faces[3 * 12] = {0, 1, 3,
						  1, 2, 3,
						  0, 1, 4,
						  1, 4, 5,
						  1, 2, 5,
						  2, 6, 5,
						  3, 2, 6,
						  3, 7, 6,
						  0, 3, 7,
						  0, 4, 7,
						  4, 5, 6,
						  4, 6, 7};

	int _indices[1] = {1};

	vertices.data = _vertices;
	faces.data = _faces;
	indices.data = _indices;

	object->vertices = vertices;
	object->faces = faces;
	object->indices = indices;
}

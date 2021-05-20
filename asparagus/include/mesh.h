#ifndef ASP_MESH_H
#define ASP_MESH_H

#include "util.h"
#include "../vbo.c"
#include "../vao.c"

/*
struct ASP_Face
{
	size_t indices_base;
	ASP_FVector3 position;
	float distance;
};
*/

struct ASP_Mesh
{
	ASP_Entity *parentObject;
	bool attached;

	//struct ASP_MeshBuffer vertices, faces, indices;
	float *vertices;
	int *indices;

	int index_count;
	int vertex_count;

	struct ASP_VAO vao;
	struct ASP_VBO vbo, ibo;
};

void ASP_Mesh_Init(struct ASP_Mesh *object);
void ASP_Mesh_Attach(struct ASP_Mesh *object, ASP_Entity *entity);
void ASP_Mesh_Destroy(struct ASP_Mesh *object);
void ASP_Mesh_Render(struct ASP_Mesh *object, struct ASP_Camera *camera);
void ASP_Mesh_Update(struct ASP_Mesh *object);

#endif
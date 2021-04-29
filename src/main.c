#include <stdio.h>
#include <windows.h>
#include "../asparagus/asparagus.c"

int update(float delta);
int start();

ASP_Entity player;
ASP_Entity cube;
float p_mspeed = 35;
float p_rspeed = 0.4f;
int iHorizontal = 0;
int iVertical = 0;

DrawEntity(ASP_Entity entity);
ASP_Entity GenerateCubeEntity();

int main(int argc, char *argv[])
{
	ASP_init(&update, &start);
	getchar();
	return 0;
}

int start()
{
	player = ASP_EntityC();
	player.position.x = 0;
	player.position.y = 0;
	player.rotation.z = PI;

	/* REGISTER GAME OBJECT */
	cube = GenerateCubeEntity();
	return 0;
}

int update(float deltatime)
{
	//printf("Updated! | Deltatime: %f | fps: %i\n", deltatime, ASP_FPS);

	iHorizontal = 0;
	iVertical = 0;

	if (ASPK_D == 1)
	{
		iHorizontal += 1;
	}
	if (ASPK_A == 1)
	{
		iHorizontal -= 1;
	}
	if (ASPK_W == 1)
	{
		iVertical += 1;
	}
	if (ASPK_S == 1)
	{
		iVertical -= 1;
	}

	float p_x1 = sinf(player.rotation.z) * p_mspeed;
	float p_y1 = cosf(player.rotation.z) * p_mspeed;

	float p_x2 = sinf(player.rotation.z + (PI / 2)) * -p_mspeed;
	float p_y2 = cosf(player.rotation.z + (PI / 2)) * -p_mspeed;

	float p_vx = (p_x1 * iVertical) + (p_x2 * iHorizontal);
	float p_vy = (p_y1 * iVertical) + (p_y2 * iHorizontal);

	player.position.x += (p_vx * deltatime);
	player.position.y += (p_vy * deltatime);

	if (ASPK_Right == 1)
	{
		player.rotation.z -= p_rspeed * deltatime;
	}
	if (ASPK_Left == 1)
	{
		player.rotation.z += p_rspeed * deltatime;
	}

	DrawEntity(cube);

	/* MINIMAP */
	ASP_Color color = ASP_ColorC(255, 0, 0, 255);
	ASP_DrawRect(renderer, color, ASP_IVector2C(10, 10), ASP_IVector2C(100, 100));
	ASP_IVector2 ssp_bc = ASP_IVector2C(60, 60);
	ASP_DrawPixel(renderer, color, ssp_bc);
	ASP_IVector2 ssp_pp = ASP_IVector2C(ssp_bc.x + player.position.x / 10, ssp_bc.y + player.position.y / 10);
	ASP_DrawRect(renderer, color, ASP_IVector2C(ssp_pp.x - 5, ssp_pp.y - 5), ASP_IVector2C(10, 10));
	int ssp_ax = sinf(player.rotation.z) * 15;
	int ssp_ay = cosf(player.rotation.z) * 15;
	ASP_IVector2 ssp_ap = ASP_IVector2C(ssp_ax, ssp_ay);
	ASP_DrawLine(renderer, color, ASP_IVector2C(ssp_pp.x, ssp_pp.y), ASP_IVector2C(ssp_pp.x + ssp_ap.x, ssp_pp.y + ssp_ap.y));

	return 0;
}

ASP_Entity GenerateCubeEntity()
{
	int tp[8];

	ASP_Vertex v1 = ASP_VertexC(ASP_FVector3C(-0.5f, -0.5f, -0.5f), tp, 0, 1);
	ASP_Vertex v2 = ASP_VertexC(ASP_FVector3C(-0.5f, 0.5f, -0.5f), tp, 1, 1);
	ASP_Vertex v3 = ASP_VertexC(ASP_FVector3C(0.5f, 0.5f, -0.5f), tp, 2, 1);
	ASP_Vertex v4 = ASP_VertexC(ASP_FVector3C(0.5f, -0.5f, -0.5f), tp, 3, 1);
	ASP_Vertex v5 = ASP_VertexC(ASP_FVector3C(-0.5f, -0.5f, 0.5f), tp, 4, 1);
	ASP_Vertex v6 = ASP_VertexC(ASP_FVector3C(-0.5f, 0.5f, 0.5f), tp, 5, 1);
	ASP_Vertex v7 = ASP_VertexC(ASP_FVector3C(0.5f, 0.5f, 0.5f), tp, 6, 1);
	ASP_Vertex v8 = ASP_VertexC(ASP_FVector3C(0.5f, -0.5f, 0.5f), tp, 7, 1);

	v8.pointer = &v7;
	v7.pointer = &v6;
	v6.pointer = &v5;
	v5.pointer = &v4;
	v4.pointer = &v3;
	v3.pointer = &v2;
	v2.pointer = &v1;
	v1.pointer = &v8;

	ASP_Entity entity;
	strcpy(entity.name, "Cube");
	entity.id = 420;
	entity.position = ASP_FVector3C(1, 1, 1);
	entity.rotation = ASP_FVector3C(0, 0, 0);
	entity.scale = ASP_FVector3C(1, 1, 1);
	entity.vertices[0] = v1;
	entity.vertices[1] = v2;
	entity.vertices[2] = v3;
	entity.vertices[3] = v4;
	entity.vertices[4] = v5;
	entity.vertices[5] = v6;
	entity.vertices[6] = v7;
	entity.vertices[7] = v8;
	entity.type = 1;

	return entity;
}

int DrawEntity(ASP_Entity entity)
{
	ASP_IVector2 dcenter = ASP_IVector2C(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	float p_fov = PI / 2;
	ASP_Color color = ASP_ColorC(255, 0, 0, 255);

	ASP_FVector2 wallpoint1 = ASP_FVector2C(0, 0);
	wallpoint1 = ASP_FVector2C(wallpoint1.x - player.position.x, wallpoint1.y - player.position.y);

	ASP_FVector2 tempwallpoint1;
	tempwallpoint1.x = cosf(player.rotation.z) * wallpoint1.x - sinf(player.rotation.z) * wallpoint1.y;
	tempwallpoint1.y = sinf(player.rotation.z) * wallpoint1.x + cosf(player.rotation.z) * wallpoint1.y;
	wallpoint1 = tempwallpoint1;

	float dist = sqrtf(wallpoint1.x * wallpoint1.x + wallpoint1.y * wallpoint1.y) + 1.0f;
	float dot = DotProduct(ASP_UNIT_j_F3, ASP_FVector3C(wallpoint1.x, wallpoint1.y, 0));
	int height = 1000 / sqrtf(dot);
	height = (height < 0) ? 0 : height;

	float wallpointangle = atanf((float)(wallpoint1.x) / (float)(wallpoint1.y));
	int wallpointssx = mapf(wallpointangle, p_fov / 2, -p_fov / 2, -SCREEN_WIDTH / 2, SCREEN_WIDTH / 2);
	ASP_DrawLine(renderer, color, ASP_IVector2C(dcenter.x + wallpointssx, dcenter.y - height / 2), ASP_IVector2C(dcenter.x + wallpointssx, dcenter.y + height / 2));
	ASP_DrawPixel(renderer, color, dcenter);

	return 0;
}
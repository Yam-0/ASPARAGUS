#include <stdio.h>
#include <windows.h>
#include "../asparagus/asparagus.c"

int update(float delta);
int start();

ASP_Entity player;
float p_mspeed = 10;
float p_rspeed = 1.0f;
int iHorizontal = 0;
int iVertical = 0;

DrawEntity(ASP_Entity entity);
int ProjectAndDrawLine(ASP_FVector3 v, ASP_FVector3 w, ASP_Entity frustum);

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
	return 0;
}

int update(float deltatime)
{
	printf("Updated! | Deltatime: %f | fps: %i\n", deltatime, ASP_FPS);

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

	float ra = (float)(p_rspeed * deltatime);

	if (ASPK_Right == 1)
	{
		player.rotation.z -= ra;
	}
	if (ASPK_Left == 1)
	{
		player.rotation.z += ra;
	}

	DrawEntity(ASP_EntityC());

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

int DrawEntity(ASP_Entity entity)
{
	ASP_FVector3 vertices[8];

	vertices[0] = ASP_FVector3C(0.5f, 0.5f, -0.5f);
	vertices[1] = ASP_FVector3C(-0.5f, 0.5f, -0.5f);
	vertices[2] = ASP_FVector3C(-0.5f, -0.5f, -0.5f);
	vertices[3] = ASP_FVector3C(0.5f, -0.5f, -0.5f);
	vertices[4] = ASP_FVector3C(0.5f, 0.5f, 0.5f);
	vertices[5] = ASP_FVector3C(-0.5f, 0.5f, 0.5f);
	vertices[6] = ASP_FVector3C(-0.5f, -0.5f, 0.5f);
	vertices[7] = ASP_FVector3C(0.5f, -0.5f, 0.5f);

	int faces[3][12] =
		{{0, 1, 3},
		 {1, 2, 3},
		 {0, 1, 5},
		 {1, 4, 5},
		 {1, 2, 5},
		 {2, 6, 5},
		 {3, 2, 6},
		 {3, 7, 6},
		 {0, 3, 7},
		 {0, 4, 7},
		 {4, 5, 6},
		 {4, 6, 7}};

	for (int i = 0; i < 12; i++)
	{
		int face[3];

		face[0] = faces[0][i];
		face[1] = faces[1][i];
		face[2] = faces[2][i];

		ProjectAndDrawLine(vertices[face[0]], vertices[face[1]], player);
		ProjectAndDrawLine(vertices[face[1]], vertices[face[2]], player);
		ProjectAndDrawLine(vertices[face[2]], vertices[face[0]], player);
	}

	return 0;
}

int ProjectAndDrawLine(ASP_FVector3 v, ASP_FVector3 w, ASP_Entity frustum)
{
	ASP_IVector2 dcenter = ASP_IVector2C(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	float p_fov = PI / 2;
	ASP_Color color = ASP_ColorC(255, 0, 0, 255);

	ASP_FVector2 linepos = ASP_FVector2C(v.x, v.y);
	linepos = ASP_FVector2C(linepos.x - player.position.x, linepos.y - player.position.y);

	ASP_FVector2 templinepos;
	templinepos.x = cosf(player.rotation.z) * linepos.x - sinf(player.rotation.z) * linepos.y;
	templinepos.y = sinf(player.rotation.z) * linepos.x + cosf(player.rotation.z) * linepos.y;
	linepos = templinepos;

	//float dist = sqrtf(linepos.x * linepos.x + linepos.y * linepos.y) + 1.0f;
	float dot = DotProduct(ASP_UNIT_j_F3, ASP_FVector3C(linepos.x, linepos.y, 0));
	int height = 1000 / dot;
	height = (height < 0) ? 0 : height;

	float lineangle = atanf((float)(linepos.x) / (float)(linepos.y));
	int linessx = mapf(lineangle, p_fov / 2, -p_fov / 2, -SCREEN_WIDTH / 2, SCREEN_WIDTH / 2);
	ASP_DrawLine(renderer, color, ASP_IVector2C(dcenter.x + linessx, dcenter.y - height / 2), ASP_IVector2C(dcenter.x + linessx, dcenter.y + height / 2));
	ASP_DrawPixel(renderer, color, dcenter);
}
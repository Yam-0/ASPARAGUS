#include <stdio.h>
#include <windows.h>
#include "../asparagus/asparagus.c"

int update(float delta);
int start();

ASP_Entity player;
float p_mspeed = 4;
float p_rspeed = 2.0f;
int iHorizontal = 0;
int iVertical = 0;

ASP_Entity box1;
ASP_Entity box2;
ASP_Entity pyramid1;

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
	box1 = ASP_GenerateBoxEntity();
	box2 = ASP_GenerateBoxEntity();
	box2.position = ASP_FVector3C(1, 1, 0);
	pyramid1 = ASP_GeneratePyramidEntity();
	pyramid1.position = ASP_FVector3C(2, 1, 0);
	ASP_Mouseaim = 1;
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
	if (ASPK_SHIFT == 1)
	{
		iVertical *= 2;
		iHorizontal *= 2;
	}

	float p_x1 = sinf(player.rotation.z) * p_mspeed;
	float p_y1 = cosf(player.rotation.z) * p_mspeed;

	float p_x2 = sinf(player.rotation.z + (PI / 2)) * -p_mspeed;
	float p_y2 = cosf(player.rotation.z + (PI / 2)) * -p_mspeed;

	float p_vx = (p_x1 * iVertical) + (p_x2 * iHorizontal);
	float p_vy = (p_y1 * iVertical) + (p_y2 * iHorizontal);

	player.position.x += (p_vx * deltatime);
	player.position.y += (p_vy * deltatime);

	if (ASPK_SPACE == 1)
	{
		player.position.z += (4.0f * deltatime);
	}
	if (ASPK_CTRL == 1)
	{
		player.position.z -= (4.0f * deltatime);
	}

	if (ASPKP_ESC == 1)
	{
		ASP_Mouseaim = !(ASP_Mouseaim == 1);
	}

	if (ASP_Mouseaim == 1)
	{
		player.rotation.z += ASPML_DX * deltatime;
		player.rotation.x += ASPML_DY * deltatime;
	}

	if (ASPK_Up == 1)
	{
		box1.position.y -= 5 * deltatime;
	}
	if (ASPK_Down == 1)
	{
		box1.position.y += 5 * deltatime;
	}
	if (ASPK_Right == 1)
	{
		box1.position.x += 5 * deltatime;
	}
	if (ASPK_Left == 1)
	{
		box1.position.x -= 5 * deltatime;
	}

	ASP_DrawEntity(box1, player);
	ASP_DrawEntity(box2, player);
	ASP_DrawEntity(pyramid1, player);

	ASP_Color RED = ASP_ColorC(255, 0, 0, 255);
	ASP_DrawPixel(renderer, RED, ASP_IVector2C(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));

	/* MINIMAP */
	ASP_Color color = ASP_ColorC(255, 0, 0, 255);
	ASP_DrawRect(renderer, color, ASP_IVector2C(10, 10), ASP_IVector2C(100, 100));
	ASP_IVector2 ssp_bc = ASP_IVector2C(60, 60);
	ASP_DrawPixel(renderer, color, ssp_bc);
	ASP_IVector2 ssp_pp = ASP_IVector2C(ssp_bc.x + player.position.x * 5, ssp_bc.y + player.position.y * 5);
	ASP_DrawRect(renderer, color, ASP_IVector2C(ssp_pp.x - 5, ssp_pp.y - 5), ASP_IVector2C(10, 10));
	int ssp_ax = sinf(player.rotation.z) * 15;
	int ssp_ay = cosf(player.rotation.z) * 15;
	ASP_IVector2 ssp_ap = ASP_IVector2C(ssp_ax, ssp_ay);
	ASP_DrawLine(renderer, color, ASP_IVector2C(ssp_pp.x, ssp_pp.y), ASP_IVector2C(ssp_pp.x + ssp_ap.x, ssp_pp.y + ssp_ap.y));

	return 0;
}
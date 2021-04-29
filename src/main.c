#include <stdio.h>
#include <windows.h>
#include "../asparagus/asparagus.c"

int update(float delta);
int start();

ASP_Entity player;
float p_mspeed = 35;
float p_rspeed = 0.4f;
int iHorizontal = 0;
int iVertical = 0;

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

	printf("Updated! | Dot: %f | wallpointssx: %i\n", dot, wallpointssx);

	/* MINIMAP */
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
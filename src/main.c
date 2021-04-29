#include <stdio.h>
#include <windows.h>
#include "../asparagus/asparagus.c"

int update(float delta);
int start();

ASP_Entity player;

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

	float p_mspeed = 75;
	float p_rspeed = 2;
	int iHorizontal = 0;
	int iVertical = 0;
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
	ASP_FVector2 w1p = ASP_FVector2C(5, 5);
	float dist = sqrtf(powf(player.position.x - w1p.x, 2) + powf(player.position.y - w1p.y, 2));
	int height = 100 / dist;

	ASP_Color color = ASP_ColorC(255, 0, 0, 255);
	ASP_DrawLine(renderer, color, ASP_IVector2C(dcenter.x, dcenter.y - height / 2), ASP_IVector2C(dcenter.x, dcenter.y + height / 2));

	return 0;
}

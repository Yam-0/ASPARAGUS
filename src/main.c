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
ASP_Entity bullet;
ASP_Entity pyramid1;

ASP_FVector3 bulletspeed;

ASP_Sprite goblin;
ASP_Sprite dog;

#define IMG_PATH1 "../data/goblin.png"
#define IMG_PATH2 "../data/dog.png"

int main(int argc, char *argv[])
{
	ASP_init(&update, &start);
	getchar();
	return 0;
}

int start()
{
	//Player
	player = ASP_EntityC();
	player.position.x = 0;
	player.position.y = 0;
	player.rotation.z = PI;

	//World objects
	box1 = ASP_GenerateBoxEntity();
	box2 = ASP_GenerateBoxEntity();
	box2.position = ASP_FVector3C(1, 1, 0);
	bullet = ASP_GenerateBoxEntity();
	bullet.position = ASP_FVector3C(0, 0, 0);
	bullet.scale = ASP_FVector3C(0.2f, 0.2f, 0.2f);
	pyramid1 = ASP_GeneratePyramidEntity();
	pyramid1.position = ASP_FVector3C(2, 1, 0);

	//Settings
	ASP_Mouseaim = 1;

	//Sprites
	goblin = ASP_LoadSprite(IMG_PATH1);
	dog = ASP_LoadSprite(IMG_PATH2);

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
		if (ASP_Mouseaim == 1)
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else
		{
			SDL_SetWindowFullscreen(window, 0);
		}
	}

	if (ASP_Mouseaim == 1)
	{
		player.rotation.z += ASPML_DX * deltatime;
		player.rotation.x += ASPML_DY * deltatime;
	}

	if (ASPK_Up == 1 && ASPK_SHIFT != 1)
	{
		box1.position.y -= 5 * deltatime;
	}
	if (ASPK_Down == 1 && ASPK_SHIFT != 1)
	{
		box1.position.y += 5 * deltatime;
	}
	if (ASPK_Right == 1 && ASPK_SHIFT != 1)
	{
		box1.position.x += 5 * deltatime;
	}
	if (ASPK_Left == 1 && ASPK_SHIFT != 1)
	{
		box1.position.x -= 5 * deltatime;
	}

	if (ASPK_Up == 1 && ASPK_SHIFT == 1)
	{
		box1.rotation.x += 5 * deltatime;
	}
	if (ASPK_Down == 1 && ASPK_SHIFT == 1)
	{
		box1.rotation.x -= 5 * deltatime;
	}
	if (ASPK_Right == 1 && ASPK_SHIFT == 1)
	{
		box1.rotation.z += 5 * deltatime;
	}
	if (ASPK_Left == 1 && ASPK_SHIFT == 1)
	{
		box1.rotation.z -= 5 * deltatime;
	}

	if (ASPK_E == 1)
	{
		box1.scale.y += 5 * deltatime;
	}
	if (ASPK_Q == 1)
	{
		box1.scale.y -= 5 * deltatime;
	}

	if (ASPMP_M1 == 1)
	{
		//ASPMP_M1 = 0;
		float speed = 5;
		bullet.position = player.position;
		bullet.rotation.x = player.rotation.x;
		bullet.rotation.y = player.rotation.y;
		bullet.rotation.z = player.rotation.z;
		float dd = cosf(player.rotation.x);
		bulletspeed.x = sinf(player.rotation.z) * dd * speed * deltatime;
		bulletspeed.y = cosf(player.rotation.z) * dd * speed * deltatime;
		bulletspeed.z = sinf(player.rotation.x) * speed * deltatime;
	}

	bullet.position.x += bulletspeed.x;
	bullet.position.y += bulletspeed.y;
	bullet.position.z += bulletspeed.z;

	ASP_DrawEntity(box1, player);
	ASP_DrawEntity(box2, player);
	ASP_DrawEntity(bullet, player);
	ASP_DrawEntity(pyramid1, player);

	/* HUD */
	ASP_Color color = ASP_ColorC(255, 255, 255, 255);
	ASP_DrawPixel(renderer, color, ASP_IVector2C(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	ASP_DrawRect(renderer, color, ASP_IVector2C(10, 10), ASP_IVector2C(100, 100));
	ASP_IVector2 ssp_bc = ASP_IVector2C(60, 60);
	ASP_DrawPixel(renderer, color, ssp_bc);
	ASP_IVector2 ssp_pp = ASP_IVector2C(ssp_bc.x + player.position.x * 5, ssp_bc.y + player.position.y * 5);
	ASP_DrawRect(renderer, color, ASP_IVector2C(ssp_pp.x - 5, ssp_pp.y - 5), ASP_IVector2C(10, 10));
	int ssp_ax = sinf(player.rotation.z) * 15;
	int ssp_ay = cosf(player.rotation.z) * 15;
	ASP_IVector2 ssp_ap = ASP_IVector2C(ssp_ax, ssp_ay);
	ASP_DrawLine(renderer, color, ASP_IVector2C(ssp_pp.x, ssp_pp.y), ASP_IVector2C(ssp_pp.x + ssp_ap.x, ssp_pp.y + ssp_ap.y));

	/* Sprites */
	ASP_DrawSprite(renderer, goblin, ASP_IVector2C(0, 0), ASP_IVector2C(box1.scale.y, box1.scale.y));

	return 0;
}
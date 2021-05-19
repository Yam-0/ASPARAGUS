#include <stdio.h>
#include <windows.h>
#include "../asparagus/asparagus.c"

ASP_Entity player;
float p_mspeed = 4;
float p_rspeed = 2.0f;
int iHorizontal = 0;
int iVertical = 0;

ASP_Entity box1;
struct ASP_Mesh box1mesh;

ASP_FVector3 bulletspeed;

ASP_Sprite goblin;

#define IMG_PATH1 "../data/goblin.png"

void start()
{
	printf("STARTED.\n");

	//Player
	player = ASP_EntityC();
	player.position.x = 0;
	player.position.y = 0;
	player.position.z = 0;
	player.rotation.x = 0;
	player.rotation.y = 0;
	player.rotation.z = 0;
	ASP_AttachCamera(&camera, &player);

	//World objects
	box1 = ASP_GenerateBoxEntity();
	ASP_Mesh_Init(&box1mesh);
	ASP_Mesh_Attach(&box1, &box1mesh);
	box1.mesh = &box1mesh;

	//Settings
	state.grabMouse = 1;

	//Sprites
	goblin = ASP_LoadSprite(IMG_PATH1);
	return;
}

void update()
{
	float deltatime = window.deltatime;
	//printf("Updated! | Deltatime: %f | fps: %i\n", deltatime, window.fps);

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
		state.grabMouse = !(state.grabMouse == 1);
		if (state.grabMouse == 1)
		{
			SDL_SetWindowFullscreen(window.handle, SDL_WINDOW_FULLSCREEN);
		}
		else
		{
			SDL_SetWindowFullscreen(window.handle, 0);
		}
	}

	if (state.grabMouse == 1)
	{
		player.rotation.z += window.mouse.delta.x * deltatime;
		player.rotation.x += window.mouse.delta.y * deltatime;
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

	/*
	//Shoot
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
	*/

	ASP_Mesh_Render(&box1mesh, &camera);

	return;
}

void tick()
{
}

void destroy()
{
}

int main(int argc, char *argv[])
{
	ASP_init(&start, &update, &tick, &destroy);
	getchar();
	return 0;
}
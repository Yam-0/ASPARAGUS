#include <stdio.h>
#include <windows.h>
#include "../asparagus/asparagus.c"

ASP_Entity player;
float p_mspeed = 4;
float p_rspeed = 2.0f;
float mouse_sensitivity = 35;
int iHorizontal = 0;
int iVertical = 0;

ASP_Entity box1;
struct ASP_Mesh box1mesh;

ASP_FVector3 bulletspeed;

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
	ASP_Camera_Attach(&camera, &player);

	//World objects
	box1 = ASP_Entity_Create("BOX");
	ASP_Mesh_Init(&box1mesh);
	ASP_Mesh_GenerateSquare(&box1mesh);
	ASP_Mesh_Attach(&box1mesh, &box1);

	//Settings
	state.grabMouse = ASP_TRUE;
	state.wireframe = ASP_FALSE;

	return;
}

void update()
{
	float deltatime = window.deltatime;
	//printf("Updated! | Deltatime: %f | fps: %i\n", deltatime, window.fps);

	iHorizontal = 0;
	iVertical = 0;

	if (ASPK_D)
	{
		iHorizontal += 1;
	}
	if (ASPK_A)
	{
		iHorizontal -= 1;
	}
	if (ASPK_W)
	{
		iVertical += 1;
	}
	if (ASPK_S)
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

	if (ASPK_SPACE)
	{
		player.position.z += (4.0f * deltatime);
	}
	if (ASPK_CTRL)
	{
		player.position.z -= (4.0f * deltatime);
	}

	if (ASPKP_ESC)
	{
		state.grabMouse = !state.grabMouse;
		if (state.grabMouse == 1)
		{
			SDL_SetWindowFullscreen(window.handle, SDL_WINDOW_FULLSCREEN);
		}
		else
		{
			SDL_SetWindowFullscreen(window.handle, 0);
		}
	}
	if (ASPKP_0)
		state.wireframe = !state.wireframe;

	if (state.grabMouse == 1)
	{
		player.rotation.z += window.mouse.delta.x * deltatime * mouse_sensitivity / 100;
		player.rotation.x += window.mouse.delta.y * deltatime * mouse_sensitivity / 100;

		player.rotation.x = clampf(player.rotation.x, -PI / 2, PI / 2);
	}

	if (ASPK_Up && !ASPK_SHIFT)
	{
		box1.position.y -= 5 * deltatime;
	}
	if (ASPK_Down && !ASPK_SHIFT)
	{
		box1.position.y += 5 * deltatime;
	}
	if (ASPK_Right && !ASPK_SHIFT)
	{
		box1.position.x += 5 * deltatime;
	}
	if (ASPK_Left && !ASPK_SHIFT)
	{
		box1.position.x -= 5 * deltatime;
	}

	if (ASPK_Up && ASPK_SHIFT)
	{
		box1.rotation.x += 5 * deltatime;
	}
	if (ASPK_Down && ASPK_SHIFT)
	{
		box1.rotation.x -= 5 * deltatime;
	}
	if (ASPK_Right && ASPK_SHIFT)
	{
		box1.rotation.z += 5 * deltatime;
	}
	if (ASPK_Left && ASPK_SHIFT)
	{
		box1.rotation.z -= 5 * deltatime;
	}

	if (ASPK_E)
	{
		box1.scale.z += 5 * deltatime;
	}
	if (ASPK_Q)
	{
		box1.scale.z -= 5 * deltatime;
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

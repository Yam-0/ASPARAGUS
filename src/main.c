#include <stdio.h>
#include <windows.h>
#include "../asparagus/asparagus.c"

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

void start()
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
	state.grabMouse = 1;

	//Sprites
	goblin = ASP_LoadSprite(IMG_PATH1);
	dog = ASP_LoadSprite(IMG_PATH2);

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

	/* LOCAL VERTEX POSITIONS */
	float vertices[] = {
		-0.5f, 0.0, -0.5f,
		0.5f, 0.0f, -0.5f,
		0.0f, 0.0f, 0.5f};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glUniform3f(uniColor, (sinf(window.totalSeconds * 4.0f) + 1.0f) / 2.0f, (cosf(window.totalSeconds * 4.0f) + 1.0f) / 2.0f, 1.0f);

	float *mm = ASP_Mat4f_GetModelMatrix(box1);
	float *vm = ASP_Mat4f_GetViewMatrix(player);
	float *pm = ASP_Mat4f_GetProjectionMatrix(PI / 2);

	mat4 view, projection;

	float aspect = 16 / 9;
	float znear = 0.01f;
	float zfar = 1000.0f;
	float fov = PI / 2;

	float pdir[3] = {cosf(player.rotation.x) * sinf(player.rotation.z),
					 sinf(player.rotation.x),
					 cosf(player.rotation.x) * cosf(player.rotation.z)};

	glm_normalize(pdir);

	float cpright[3];
	float cpup[3];
	glm_vec3_cross((vec3){0.0f, 1.0f, 0.0f}, pdir, cpright);
	glm_vec3_cross(pdir, cpright, cpup);
	glm_mat4_identity(view);
	glm_mat4_identity(projection);

	float pp[3] = {player.position.x, player.position.z, player.position.y};
	float wpp[3];
	glm_vec3_add(pp, pdir, wpp);
	glm_lookat(pp, wpp, cpup, view);
	glm_perspective(fov, aspect, znear, zfar, projection);

	if (ASPMP_M1)
	{
		ASP_Mat4f_Print(mm);
	}

	vm = view[0];
	pm = projection[0];

	glMatrixMode(GL_MODELVIEW);
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, mm);
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, vm);
	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, pm);

	//free(mm);
	//free(vm);
	//free(pm);

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
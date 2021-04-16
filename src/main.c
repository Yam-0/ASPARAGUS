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
	printf("Starting update loop!\n");
	player = ASP_EntityC();
	player.position.x = 5;
	player.position.y = 5;
	return 0;
}

int update(float deltatime)
{
	if (deltatime < 0.002f)
	{
		deltatime = 0.001f;
	}

	char sfps[10];
	char str[10];
	int fps = 1.0f / deltatime;
	sprintf(str, "%i", &sfps);
	printf("Updated! | Deltatime: %f | fps: %i\n", deltatime, fps);

	float speed = 75;

	if (ASPK_Right == 1)
	{
		player.position.x += speed * deltatime;
	}
	if (ASPK_Left == 1)
	{
		player.position.x -= speed * deltatime;
	}
	if (ASPK_Up == 1)
	{
		player.position.y -= speed * deltatime;
	}
	if (ASPK_Down == 1)
	{
		player.position.y += speed * deltatime;
	}

	int boxSize = (SCREEN_WIDTH - 60) / 3;
	int xOffset = (SCREEN_WIDTH - boxSize * 3) / 2;
	ASP_IVector2 boxScale = ASP_IVector2C(boxSize, boxSize);
	int box1posx = (boxSize * 0) + xOffset;
	int box2posx = (boxSize * 1) + xOffset;
	int box3posx = (boxSize * 2) + xOffset;

	ASP_Color RED = ASP_ColorC(255, 0, 0, 255);
	ASP_DrawRect(renderer, RED, ASP_IVector2C(box1posx, SCREEN_HEIGHT / 2 - boxSize / 2), boxScale);
	ASP_DrawRect(renderer, RED, ASP_IVector2C(box2posx, SCREEN_HEIGHT / 2 - boxSize / 2), boxScale);
	ASP_DrawRect(renderer, RED, ASP_IVector2C(box3posx, SCREEN_HEIGHT / 2 - boxSize / 2), boxScale);

	/*FIRST BOX*/
	ASP_DrawRect(renderer, RED, );

	return 0;
}

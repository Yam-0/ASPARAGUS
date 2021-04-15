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
	//printf("Updated! | Deltatime: %f | fps: %i\n", deltatime, fps);

	float speed = 25;

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

	ASP_Color color1 = ASP_ColorC(255, 0, 0, 255);
	ASP_Color color2 = ASP_ColorC(0, 255, 0, 255);

	ASP_IVector2 p1 = ASP_IVector2C(player.position.x - 5, player.position.y - 5);
	ASP_IVector2 p2 = ASP_IVector2C(player.position.x + 5, player.position.y - 5);
	ASP_IVector2 p3 = ASP_IVector2C(player.position.x + 5, player.position.y + 5);
	ASP_IVector2 p4 = ASP_IVector2C(player.position.x - 5, player.position.y + 5);

	ASP_DrawLine(renderer, color1, p1, p2);
	ASP_DrawLine(renderer, color1, p2, p3);
	ASP_DrawLine(renderer, color1, p3, p4);
	ASP_DrawLine(renderer, color1, p4, p1);

	ASP_Color color3 = ASP_ColorC(255, 0, 0, 255);
	ASP_IVector2 pp1 = ASP_IVector2C(player.position.x, player.position.y);
	ASP_IVector2 pp2 = ASP_IVector2C(300, 200);
	ASP_IVector2 pp3 = ASP_IVector2C(200, 300);
	ASP_DrawLine(renderer, color3, pp1, pp2);
	ASP_DrawLine(renderer, color2, pp1, pp3);

	return 0;
}

#include <stdio.h>
#include "../asparagus/asparagus.c"

int update(float delta);
int start();

int main(int argc, char *argv[])
{
	ASP_init(&update, &start);
	getchar();
	return 0;
}

int start()
{
	printf("Starting update loop!\n");
	return 0;
}

int update(float deltatime)
{
	printf("Updated! | Deltatime: %f\n", deltatime);

	struct ASP_Color color;
	color.r = 255;
	color.g = 0;
	color.b = 0;
	color.a = 255;

	ASP_DrawLine(renderer, color, 50, 50, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50);

	return 0;
}

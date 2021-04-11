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

	for (int i = 0; i < 1000; i++)
	{
		//ASP_sleep(2);
	}

	return 0;
}

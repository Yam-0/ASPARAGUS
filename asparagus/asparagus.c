#include <stdio.h>
#include <time.h>
#include "../sdl/include/SDL2/SDL.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int ASP_Running = 0;
int ASP_Sleeping = 0;

int colorA = 255;
int colorB = 144;
int colorC = 255;

int ASP_init(int (*update)(float), int (*start)());
int ASP_sleep(int m_secs);
int ASP_Render(SDL_Renderer *renderer, SDL_Window *window);
int ASP_EventHandler();

int ASP_init(int (*update)(float), int (*start)())
{
	printf("------- USING THE ASPARAGUS ENGINE -------\n");

	ASP_Running = 0;

	float deltatime = 0;
	int msec = 0;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("ASPARAGUS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	//Start callback
	(*start)();
	ASP_Running = 1;

	SDL_Surface *screenSurface = SDL_GetWindowSurface(window);

	//Update loop
	while (ASP_Running)
	{
		//Time before callback
		clock_t before = clock();

		ASP_EventHandler();

		//Update callback
		(*update)(deltatime);

		SDL_SetRenderDrawColor(renderer, colorA, colorB, colorC, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawPoint(renderer, 100, 100);
		ASP_Render(renderer, window);

		//Frame time - deltatime
		clock_t difference = clock() - before;
		msec = difference * 1000 / CLOCKS_PER_SEC;
		deltatime = (float)msec / 1000;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

int ASP_EventHandler()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		ASP_Running = 0;
		break;
	case SDL_MOUSEBUTTONDOWN:
		colorA = rand() % 255;
		colorB = rand() % 255;
		colorC = rand() % 255;
		break;
	case SDL_MOUSEMOTION:
		colorA = 255;
		colorB = 255;
		colorC = 255;
		break;

	default:
		break;
	}

	return 0;
}

int ASP_Render(SDL_Renderer *renderer, SDL_Window *window)
{
	SDL_RenderPresent(renderer);
	return 0;
}

int ASP_sleep(int m_secs)
{
	ASP_Sleeping = 1;
	int sleeptime = m_secs;

	clock_t before = clock();

	//Sleep loop
	while (ASP_Sleeping)
	{
		clock_t difference = clock() - before;
		sleeptime = difference * 1000 / CLOCKS_PER_SEC;
		if (sleeptime >= m_secs)
		{
			ASP_Sleeping = 0;
		}
	}

	return 0;
}

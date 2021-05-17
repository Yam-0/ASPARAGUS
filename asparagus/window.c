#include "include/window.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

struct ASP_Window window;

int ASP_CreateWindow(_ASP_CALLBACK start, _ASP_CALLBACK update, _ASP_CALLBACK tick, _ASP_CALLBACK destroy)
{
	window.start = start;
	window.update = update;
	window.tick = update;
	window.destroy = update;

	window.screenSize.x = SCREEN_WIDTH;
	window.screenSize.y = SCREEN_HEIGHT;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	window.handle = SDL_CreateWindow("ASPARAGUS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window.screenSize.x, window.screenSize.y, SDL_WINDOW_OPENGL);
	if (window.handle == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
}

void ASP_HandleCursor()
{
	int tmplx, tmply = 0;
	SDL_GetMouseState(&tmplx, &tmply);
	SDL_ShowCursor(!state.grabMouse);

	window.mouse.delta.x = 0;
	window.mouse.delta.y = 0;

	if (state.mouseFocus == 1 && state.keyboardFocus == 1)
	{
		window.mouse.delta.x = window.mouse.position.x - tmplx;
		window.mouse.delta.y = window.mouse.position.y - tmply;

		if (state.grabMouse)
		{
			SDL_WarpMouseInWindow(window.handle, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			tmplx = SCREEN_WIDTH / 2;
			tmply = SCREEN_HEIGHT / 2;
		}
	}

	window.mouse.position.x = tmplx;
	window.mouse.position.y = tmply;
}
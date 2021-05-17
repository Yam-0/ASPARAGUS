#ifndef WINDOW_H
#define WINDOW_H
#define u64 uint64_t

#include <SDL2/SDL.h>
#include "util.h"

struct ASP_Key
{
	bool down, last, pressed;
};

struct ASP_Mouse
{
	struct ASP_Key keys[5];
	ASP_IVector2 position, delta;
};

struct ASP_Keyboard
{
	struct ASP_Key keys[5];
};

typedef void (*_ASP_CALLBACK)();

struct ASP_Window
{
	SDL_Window *handle;
	ASP_IVector2 screenSize;
	_ASP_CALLBACK start, update, destroy, tick;

	struct ASP_Mouse mouse;
	struct ASP_Keyboard keyboard;

	//Timing
	float deltatime;
	int fps;
	u64 totalTicks;
	u64 totalFrames, lastFrame;
	u64 totalSeconds, lastSecond;
};

extern struct ASP_Window window;

int ASP_CreateWindow(_ASP_CALLBACK start, _ASP_CALLBACK update, _ASP_CALLBACK tick, _ASP_CALLBACK destroy);

#endif
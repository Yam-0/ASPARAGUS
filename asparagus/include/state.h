#ifndef STATE_H
#define STATE_H

#include "util.h"

struct ASP_State
{
	struct ASP_Window *window;

	bool running, wireframe;
	bool grabMouse;
	bool keyboardFocus, mouseFocus;
};

extern struct ASP_State state;
struct ASP_State state;

#endif

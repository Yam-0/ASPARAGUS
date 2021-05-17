#ifndef ASP_STATE_H
#define ASP_STATE_H

#include "util.h"
#include "shader.h"

struct ASP_State
{
	struct ASP_Window *window;
	struct ASP_Shader shader;

	bool running, wireframe;
	bool grabMouse;
	bool keyboardFocus, mouseFocus;
};

extern struct ASP_State state;
struct ASP_State state;

#endif

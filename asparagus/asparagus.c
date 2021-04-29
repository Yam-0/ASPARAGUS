#include "./asparagus.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int ASP_Running = 0;
int ASP_Sleeping = 0;

int ASPK_Right, ASPK_Left, ASPK_Down, ASPK_Up;
int ASPK_W, ASPK_A, ASPK_S, ASPK_D;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *btexture;

float PI = 3.141592f;
float ASP_FPS;

int ASP_init(int (*update)(float), int (*start)())
{
	printf("------- USING THE ASPARAGUS ENGINE -------\n");

	ASP_Running = 0;

	float deltatime = 0;
	int msec = 0;

	ASPK_Right, ASPK_Left, ASPK_Down, ASPK_Up = 0;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow("ASPARAGUS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		//Update callback
		(*update)(deltatime);

		ASP_Render(renderer, window);

		//Frame time & deltatime
		clock_t difference = clock() - before;
		msec = difference * 1000 / CLOCKS_PER_SEC;
		deltatime = (float)msec / 1000;
		deltatime = (deltatime <= 0.001f) ? deltatime + 0.001f : deltatime;
		ASP_FPS = 1.0f / deltatime;
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
		break;

	case SDL_MOUSEMOTION:
		break;

	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_RIGHT:
			ASPK_Right = 1;
			break;

		case SDLK_LEFT:
			ASPK_Left = 1;
			break;

		case SDLK_DOWN:
			ASPK_Down = 1;
			break;

		case SDLK_UP:
			ASPK_Up = 1;
			break;

		case SDLK_w:
			ASPK_W = 1;
			break;

		case SDLK_a:
			ASPK_A = 1;
			break;

		case SDLK_s:
			ASPK_S = 1;
			break;

		case SDLK_d:
			ASPK_D = 1;
			break;
		}
		break;

	case SDL_KEYUP:
		switch (event.key.keysym.sym)
		{
		case SDLK_RIGHT:
			ASPK_Right = 0;
			break;

		case SDLK_LEFT:
			ASPK_Left = 0;
			break;

		case SDLK_DOWN:
			ASPK_Down = 0;
			break;

		case SDLK_UP:
			ASPK_Up = 0;
			break;

		case SDLK_w:
			ASPK_W = 0;
			break;

		case SDLK_a:
			ASPK_A = 0;
			break;

		case SDLK_s:
			ASPK_S = 0;
			break;

		case SDLK_d:
			ASPK_D = 0;
			break;
		}
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

int ASP_DrawPixel(SDL_Renderer *renderer, ASP_Color color, ASP_IVector2 p)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(renderer, p.x, p.y);

	return 0;
}
int ASP_DrawLine(SDL_Renderer *renderer, ASP_Color color, ASP_IVector2 p1, ASP_IVector2 p2)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;

	float l_tr = (float)dy / (float)dx;
	float l_angle = atanf(l_tr);
	float l_length = sqrtf(dx * dx + dy * dy);

	float xOffset;
	float yOffset;

	int l_dirM = 2 * (p2.x >= p1.x) - 1;
	xOffset = cosf(l_angle) * l_dirM;
	yOffset = sinf(l_angle) * l_dirM;

	ASP_IVector2 drawpoint;
	for (int i = 0; i < l_length; i++)
	{
		drawpoint.x = round(p1.x + xOffset * i);
		drawpoint.y = round(p1.y + yOffset * i);
		ASP_DrawPixel(renderer, color, drawpoint);
	}

	return 0;
}

int ASP_DrawRect(SDL_Renderer *renderer, ASP_Color color, ASP_IVector2 position, ASP_IVector2 scale)
{
	ASP_IVector2 p1 = ASP_IVector2C(position.x, position.y);
	ASP_IVector2 p2 = ASP_IVector2C(position.x + scale.x, position.y);
	ASP_IVector2 p3 = ASP_IVector2C(position.x + scale.x, position.y + scale.y);
	ASP_IVector2 p4 = ASP_IVector2C(position.x, position.y + scale.y);

	ASP_DrawLine(renderer, color, p1, p2);
	ASP_DrawLine(renderer, color, p2, p3);
	ASP_DrawLine(renderer, color, p3, p4);
	ASP_DrawLine(renderer, color, p4, p1);

	return 0;
}

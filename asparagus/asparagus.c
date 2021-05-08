#include "./asparagus.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
int DISPLAY_WIDTH;
int DISPLAY_HEIGHT;

int ASP_Running = 0;
int ASP_Sleeping = 0;
int ASP_Mouseaim = 0;
int ASP_KEYBOARDFOCUSED;
int ASP_MOUSEFOCUSED;
int ASP_FOCUSED;

int ASPML_X, ASPML_Y, ASPML_DX, ASPML_DY = 0;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *btexture;

float PI = 3.141592f;
int ASP_FPS;

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
		deltatime = (deltatime == 0.0f) ? deltatime + 0.0001f : deltatime;
		ASP_FPS = 1.0f / deltatime;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

int ASP_EventHandler()
{
	ASP_ResetKeyStates();
	SDL_Event event;
	SDL_PollEvent(&event);

	ASP_KEYBOARDFOCUSED = (window == SDL_GetKeyboardFocus()) ? 1 : 0;
	ASP_MOUSEFOCUSED = (window == SDL_GetMouseFocus()) ? 1 : 0;

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	DISPLAY_WIDTH = DM.w;
	DISPLAY_HEIGHT = DM.h;

	int tmplx, tmply = 0;
	SDL_GetMouseState(&tmplx, &tmply);
	SDL_ShowCursor(!ASP_Mouseaim);
	ASPML_DX, ASPML_DY = 0;
	if (ASP_MOUSEFOCUSED == 1 && ASP_KEYBOARDFOCUSED == 1)
	{
		ASPML_DX = ASPML_X - tmplx;
		ASPML_DY = ASPML_Y - tmply;

		if (ASP_Mouseaim == 1)
		{
			SDL_WarpMouseInWindow(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			tmplx = SCREEN_WIDTH / 2;
			tmply = SCREEN_HEIGHT / 2;
		}
	}

	ASPML_X = tmplx;
	ASPML_Y = tmply;

	switch (event.type)
	{
	case SDL_QUIT:
		ASP_Running = 0;
		break;

	case SDL_MOUSEBUTTONDOWN:
		break;

	case SDL_MOUSEMOTION:;

		break;

	case SDL_KEYDOWN:
		if (event.key.repeat == 0)
			ASP_SetKeyState(event.key.keysym.sym, 1);
		break;

	case SDL_KEYUP:
		ASP_SetKeyState(event.key.keysym.sym, 0);
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

int InsideTriangle(ASP_IVector2 s, ASP_IVector2 a, ASP_IVector2 b, ASP_IVector2 c)
{
	int as_x = s.x - a.x;
	int as_y = s.y - a.y;

	int s_ab = (b.x - a.x) * as_y - (b.y - a.y) * as_x > 0;

	if ((c.x - a.x) * as_y - (c.y - a.y) * as_x > 0 == s_ab)
		return 0;

	if ((c.x - b.x) * (s.y - b.y) - (c.y - b.y) * (s.x - b.x) > 0 != s_ab)
		return 0;

	return 1;
}

int ASP_DrawEntity(ASP_Entity entity, ASP_Entity camera)
{
	ASP_FVector3 v;
	ASP_FVector3 w;

	ASP_IVector2 dcenter = ASP_IVector2C(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	ASP_Color RED = ASP_ColorC(255, 0, 0, 255);
	ASP_Color BLACK = ASP_ColorC(0, 0, 0, 255);
	float p_fov = PI / 2;
	float depth = PI;

	//Loop over faces
	for (int i = 0; i < entity.facecount; i++)
	{
		int face[3];
		face[0] = entity.faces[i][0];
		face[1] = entity.faces[i][1];
		face[2] = entity.faces[i][2];

		ASP_IVector2 trig[3];

		//Face vertex loop
		for (int j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				v = entity.vertices[face[0]];
				w = entity.vertices[face[1]];
			}
			if (j == 1)
			{
				v = entity.vertices[face[1]];
				w = entity.vertices[face[2]];
			}
			if (j == 2)
			{
				v = entity.vertices[face[2]];
				w = entity.vertices[face[0]];
			}

			ASP_FVector3 vp = ASP_FVector3C(
				v.x - camera.position.x + entity.position.x,
				v.y - camera.position.y + entity.position.y,
				v.z - camera.position.z + entity.position.z);
			ASP_FVector3 wp = ASP_FVector3C(
				w.x - camera.position.x + entity.position.x,
				w.y - camera.position.y + entity.position.y,
				w.z - camera.position.z + entity.position.z);

			/* Z AXIS */
			ASP_FVector3 tempvp = vp;
			ASP_FVector3 tempwp = wp;
			tempvp.x = cosf(camera.rotation.z) * vp.x - sinf(camera.rotation.z) * vp.y;
			tempvp.y = sinf(camera.rotation.z) * vp.x + cosf(camera.rotation.z) * vp.y;
			tempwp.x = cosf(camera.rotation.z) * wp.x - sinf(camera.rotation.z) * wp.y;
			tempwp.y = sinf(camera.rotation.z) * wp.x + cosf(camera.rotation.z) * wp.y;
			vp = tempvp;
			wp = tempwp;

			/* X AXIS */
			tempvp = vp;
			tempwp = wp;
			tempvp.z = cosf(camera.rotation.x) * vp.z - sinf(camera.rotation.x) * vp.y;
			tempvp.y = sinf(camera.rotation.x) * vp.z + cosf(camera.rotation.x) * vp.y;
			tempwp.z = cosf(camera.rotation.x) * wp.z - sinf(camera.rotation.x) * wp.y;
			tempwp.y = sinf(camera.rotation.x) * wp.z + cosf(camera.rotation.x) * wp.y;
			vp = tempvp;
			wp = tempwp;

			if (vp.y >= 0 && wp.y >= 0)
			{
				float vdist = sqrtf(vp.y * vp.y + vp.z * vp.z + vp.x * vp.x) + 0.1f;
				float wdist = sqrtf(wp.y * wp.y + wp.z * wp.z + wp.x * wp.x) + 0.1f;
				float vdot = DotProduct(ASP_UNIT_j_F3, v);
				float wdot = DotProduct(ASP_UNIT_j_F3, w);
				vp.x *= (depth / vdist);
				wp.x *= (depth / wdist);
				vp.y *= (depth / vdist);
				wp.y *= (depth / wdist);
				vp.z *= (depth / vdist);
				wp.z *= (depth / wdist);

				float vaz = atanf((float)(vp.x) / (float)(vp.y));
				float waz = atanf((float)(wp.x) / (float)(wp.y));
				float vay = atanf((float)(vp.z) / (float)(vp.y));
				float way = atanf((float)(wp.z) / (float)(wp.y));
				int vssx = mapf(vaz, p_fov / 2, -p_fov / 2, 0, SCREEN_WIDTH);
				int wssx = mapf(waz, p_fov / 2, -p_fov / 2, 0, SCREEN_WIDTH);
				int vssy = mapf(vay, p_fov / 2, -p_fov / 2, 0, SCREEN_HEIGHT);
				int wssy = mapf(way, p_fov / 2, -p_fov / 2, 0, SCREEN_HEIGHT);
				vssx = (vssx < 0) ? 0 : vssx;
				wssx = (wssx < 0) ? 0 : wssx;
				vssy = (vssy < 0) ? 0 : vssy;
				wssy = (wssy < 0) ? 0 : wssy;

				ASP_DrawLine(renderer, RED, ASP_IVector2C(vssx, vssy), ASP_IVector2C(wssx, wssy));
				trig[j] = ASP_IVector2C(vssx, vssy);
			}
			else
			{
				trig[j] = ASP_IVector2C(0, 0);
			}
		}

		/*
		int minx = trig[0].x;
		int miny = trig[0].y;
		int maxx = minx;
		int maxy = miny;
		for (int c = 0; c < 3; c++)
		{
			int vertexx = trig[c].x;
			int vertexy = trig[c].y;
			if (vertexx < minx)
			{
				minx = vertexx;
			}
			if (vertexy < miny)
			{
				miny = vertexy;
			}
			if (vertexx > maxx)
			{
				maxx = vertexx;
			}
			if (vertexy > maxy)
			{
				maxy = vertexy;
			}
		}

		for (int x = 0; x < maxx - minx; x++)
		{
			for (int y = 0; y < maxy - miny; y++)
			{
				if (InsideTriangle(ASP_IVector2C(minx + x, miny + y), trig[0], trig[1], trig[2]) == 1)
				{
					ASP_DrawPixel(renderer, BLACK, ASP_IVector2C(minx + x, miny + y));
				}
			}
		}
		*/

		//ASP_DrawRect(renderer, BLACK, ASP_IVector2C(minx, miny), ASP_IVector2C(maxx - minx, maxy - miny));
	}

	return 0;
}

ASP_Entity ASP_GenerateBoxEntity()
{
	ASP_Entity box;
	strcpy(box.name, "BOX");
	box.id = 0;
	box.position = ASP_FVector3C(0, 0, 0);
	box.rotation = ASP_FVector3C(0, 0, 0);
	box.scale = ASP_FVector3C(1, 1, 1);

	ASP_FVector3 vertices[8] = {
		ASP_FVector3C(0.5f, 0.5f, -0.5f),
		ASP_FVector3C(-0.5f, 0.5f, -0.5f),
		ASP_FVector3C(-0.5f, -0.5f, -0.5f),
		ASP_FVector3C(0.5f, -0.5f, -0.5f),
		ASP_FVector3C(0.5f, 0.5f, 0.5f),
		ASP_FVector3C(-0.5f, 0.5f, 0.5f),
		ASP_FVector3C(-0.5f, -0.5f, 0.5f),
		ASP_FVector3C(0.5f, -0.5f, 0.5f)};

	for (int i = 0; i < 8; i++)
	{
		box.vertices[i] = vertices[i];
	}

	box.vertexcount = 8;

	int faces[12][3] =
		{0, 1, 3,
		 1, 2, 3,
		 0, 1, 4,
		 1, 4, 5,
		 1, 2, 5,
		 2, 6, 5,
		 3, 2, 6,
		 3, 7, 6,
		 0, 3, 7,
		 0, 4, 7,
		 4, 5, 6,
		 4, 6, 7};

	for (int i = 0; i < 12; i++)
	{
		box.faces[i][0] = faces[i][0];
		box.faces[i][1] = faces[i][1];
		box.faces[i][2] = faces[i][2];
	}

	box.facecount = 12;
	box.type = 1;

	return box;
}

ASP_Entity ASP_GeneratePyramidEntity()
{
	ASP_Entity pyramid;
	strcpy(pyramid.name, "BOX");
	pyramid.id = 0;
	pyramid.position = ASP_FVector3C(0, 0, 0);
	pyramid.rotation = ASP_FVector3C(0, 0, 0);
	pyramid.scale = ASP_FVector3C(1, 1, 1);

	ASP_FVector3 vertices[5] = {
		ASP_FVector3C(0.5f, 0.5f, -0.5f),
		ASP_FVector3C(-0.5f, 0.5f, -0.5f),
		ASP_FVector3C(-0.5f, -0.5f, -0.5f),
		ASP_FVector3C(0.5f, -0.5f, -0.5f),
		ASP_FVector3C(0.0f, 0.0f, 0.7f)};

	for (int i = 0; i < 5; i++)
	{
		pyramid.vertices[i] = vertices[i];
	}

	pyramid.vertexcount = 5;

	int faces[6][3] = {
		{0, 1, 3},
		{1, 2, 3},
		{0, 1, 4},
		{1, 2, 4},
		{2, 3, 4},
		{3, 0, 4}};

	for (int i = 0; i < 6; i++)
	{
		pyramid.faces[i][0] = faces[i][0];
		pyramid.faces[i][1] = faces[i][1];
		pyramid.faces[i][2] = faces[i][2];
	}

	pyramid.facecount = 6;
	pyramid.type = 1;

	return pyramid;
}

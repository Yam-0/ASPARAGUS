#include "./asparagus.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;
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
uint32_t *pixelBuffer;

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

	SDL_RenderSetLogicalSize(renderer, 1920, 1080);

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
	btexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Update loop
	while (ASP_Running)
	{
		//Time before callback
		clock_t before = clock();

		ASP_EventHandler();

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_SetRenderTarget(renderer, btexture);
		SDL_RenderClear(renderer);

		int32_t pitch = 0;
		pixelBuffer = NULL;
		SDL_LockTexture(btexture, NULL, (void **)&pixelBuffer, &pitch);
		ASP_DrawFill(renderer, ASP_ColorC(60, 160, 255, 255));

		//Update callback
		(*update)(deltatime);

		ASP_Render(renderer, window, btexture);

		//Frame time & deltatime
		clock_t difference = clock() - before;
		msec = difference * 1000 / CLOCKS_PER_SEC;
		deltatime = (float)msec / 1000;
		deltatime = (deltatime == 0.0f) ? deltatime + 0.0001f : deltatime;
		ASP_FPS = 1.0f / deltatime;
	}

	SDL_DestroyTexture(btexture);
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
		ASPMP_M1 = (ASPMP_M1 == 0) ? 1 : 0;
		ASPM_M1 = 1;
		break;
	case SDL_MOUSEBUTTONUP:
		ASPM_M1 = 0;
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

int ASP_Render(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *texture)
{
	SDL_UnlockTexture(texture);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
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
	if (p.y < 0 || p.y >= SCREEN_HEIGHT || p.x < 0 || p.x >= SCREEN_WIDTH)
		return;
	int i = index(p.x, p.y, SCREEN_WIDTH);
	pixelBuffer[i] = (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;
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

int ASP_DrawFill(SDL_Renderer *renderer, ASP_Color color)
{
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			ASP_DrawPixel(renderer, color, ASP_IVector2C(i, j));
		}
	}
}

ASP_FVector3 ASP_RotateVector(float a, ASP_FVector3 vector, int axis)
{
	ASP_FVector3 tempvector = vector;

	switch (axis)
	{
	case 0: //x
		tempvector.x = vector.x;
		tempvector.z = cosf(a) * vector.z - sinf(a) * vector.y;
		tempvector.y = sinf(a) * vector.z + cosf(a) * vector.y;
		break;
	case 1: //y
		tempvector.x = cosf(a) * vector.z - sinf(a) * vector.x;
		tempvector.y = vector.y;
		tempvector.z = sinf(a) * vector.z + cosf(a) * vector.x;
		break;
	case 2: //z
		tempvector.x = cosf(a) * vector.x - sinf(a) * vector.y;
		tempvector.y = sinf(a) * vector.x + cosf(a) * vector.y;
		tempvector.z = vector.z;
		break;

	default:
		break;
	}

	vector = tempvector;

	return vector;
}

int ASP_InTriangle(ASP_IVector2 p, ASP_IVector2 a, ASP_IVector2 b, ASP_IVector2 c)
{
	int as_x = p.x - a.x;
	int as_y = p.y - a.y;

	int s_ab = (b.x - a.x) * as_y - (b.y - a.y) * as_x > 0;

	if ((c.x - a.x) * as_y - (c.y - a.y) * as_x > 0 == s_ab)
		return 0;

	if ((c.x - b.x) * (p.y - b.y) - (c.y - b.y) * (p.x - b.x) > 0 != s_ab)
		return 0;

	return 1;
}

int ASP_DrawEntity(ASP_Entity entity, ASP_Entity camera)
{
	ASP_FVector3 v;
	ASP_FVector3 w;

	ASP_Color COLOR = ASP_ColorC(255, 255, 255, 255);
	ASP_Color fCOLOR = ASP_ColorC(0, 255, 255, 255);
	float p_fov = PI / 2;
	float depth = PI;

	//Loop over faces
	for (int i = 0; i < entity.facecount; i++)
	{
		int face[3];
		face[0] = entity.faces[i][0];
		face[1] = entity.faces[i][1];
		face[2] = entity.faces[i][2];

		ASP_IVector2 tris[3];

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

			ASP_FVector3 vp = v;
			ASP_FVector3 wp = w;

			/* ORIGIN RELATIVE SCALING */
			vp.x *= entity.scale.x;
			vp.y *= entity.scale.y;
			vp.z *= entity.scale.z;
			wp.x *= entity.scale.x;
			wp.y *= entity.scale.y;
			wp.z *= entity.scale.z;

			/* ORIGIN RELATIVE ROTATION */
			vp = ASP_RotateVector(-entity.rotation.x, vp, 0);
			wp = ASP_RotateVector(-entity.rotation.x, wp, 0);
			vp = ASP_RotateVector(-entity.rotation.z, vp, 2);
			wp = ASP_RotateVector(-entity.rotation.z, wp, 2);
			//vp = ASP_RotateVector(-entity.rotation.y, vp, 1);
			//wp = ASP_RotateVector(-entity.rotation.y, wp, 1);

			/* MOVING TO CAMERA RELATIVE POSITION */
			vp.x = vp.x - camera.position.x + entity.position.x;
			vp.y = vp.y - camera.position.y + entity.position.y;
			vp.z = vp.z - camera.position.z + entity.position.z;
			wp.x = wp.x - camera.position.x + entity.position.x;
			wp.y = wp.y - camera.position.y + entity.position.y;
			wp.z = wp.z - camera.position.z + entity.position.z;

			/* AXIS ROTATION AROUND CAMERA */
			vp = ASP_RotateVector(camera.rotation.z, vp, 2);
			wp = ASP_RotateVector(camera.rotation.z, wp, 2);
			vp = ASP_RotateVector(camera.rotation.x, vp, 0);
			wp = ASP_RotateVector(camera.rotation.x, wp, 0);

			if (vp.y >= 0 && wp.y >= 0)
			{
				float vdist = sqrtf(vp.y * vp.y + vp.z * vp.z + vp.x * vp.x) + 0.0001f;
				float wdist = sqrtf(wp.y * wp.y + wp.z * wp.z + wp.x * wp.x) + 0.0001f;
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
				ASP_DrawLine(renderer, COLOR, ASP_IVector2C(vssx, vssy), ASP_IVector2C(wssx, wssy));
				tris[j] = ASP_IVector2C(vssx, vssy);
			}
			else
			{
				tris[j] = ASP_IVector2C(0, 0);
			}
		}
		int minx = tris[0].x;
		int miny = tris[0].y;
		int maxx = minx;
		int maxy = miny;
		for (int c = 0; c < 3; c++)
		{
			int vertexx = tris[c].x;
			int vertexy = tris[c].y;
			vertexx = (vertexx > SCREEN_WIDTH) ? SCREEN_WIDTH : vertexx;
			vertexy = (vertexy > SCREEN_HEIGHT) ? SCREEN_HEIGHT : vertexy;
			vertexx = (vertexx < 0) ? 0 : vertexx;
			vertexy = (vertexy < 0) ? 0 : vertexy;
			minx = (vertexx < minx) ? vertexx : minx;
			miny = (vertexy < miny) ? vertexy : miny;
			maxx = (vertexx > maxx) ? vertexx : maxx;
			maxy = (vertexy > maxy) ? vertexy : maxy;
		}

		//fCOLOR.r = sqrtf(maxx - minx) * 256;
		//fCOLOR.g = sqrtf(maxy - miny) * 256;
		//fCOLOR.b = sqrtf(maxx - miny) * 256;

		for (int x = 0; x < maxx - minx; x++)
		{
			for (int y = 0; y < maxy - miny; y++)
			{
				if (ASP_InTriangle(ASP_IVector2C(minx + x, miny + y), tris[0], tris[1], tris[2]) == 1)
				{
					ASP_DrawPixel(renderer, entity.faceColor[i], ASP_IVector2C(minx + x, miny + y));
				}
			}
		}
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

		box.faceColor[i] = ASP_ColorC(rand() % 255, 255, 255, 255);
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

		pyramid.faceColor[i] = ASP_ColorC(rand() % 255, 255, 255, 255);
	}

	pyramid.facecount = 6;
	pyramid.type = 1;

	return pyramid;
}

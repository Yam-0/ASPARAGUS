#include "include/asparagus.h"

int DISPLAY_WIDTH;
int DISPLAY_HEIGHT;

int ASP_KEYBOARDFOCUSED;
int ASP_MOUSEFOCUSED;
int ASP_FOCUSED;

float PI = 3.141592f;
struct ASP_Camera camera;

int ASP_init(_ASP_CALLBACK start, _ASP_CALLBACK update, _ASP_CALLBACK tick, _ASP_CALLBACK destroy)
{
	printf("------- USING THE ASPARAGUS ENGINE -------\n");

	ASP_CreateWindow(start, update, tick, destroy);
	state.window = &window;
	state.running = ASP_TRUE;

	float deltatime = 0;
	int msec = 0;

	/* SHADERS */
	state.shader = ASP_CreateShader("../data/shaders/shader.vert", "../data/shaders/shader.frag", 0, NULL);
	ASP_BindShader(state.shader);

	//Camera
	camera = ASP_CreateCamera(PI / 2, NULL);

	//Start callback
	window.start();

	//Update loop
	while (state.running)
	{
		//Time before callback
		clock_t before = clock();

		ASP_EventHandler();

		//Update callback
		window.update();

		ASP_UpdateCamera(&camera);
		ASP_Render();

		//Frame time & deltatime
		clock_t difference = clock() - before;
		msec = difference * 1000 / CLOCKS_PER_SEC;
		deltatime = (float)msec / 1000;
		window.deltatime = (deltatime == 0.0f) ? deltatime + 0.0001f : deltatime;
		window.fps = 1.0f / deltatime;
		window.totalSeconds += deltatime;

		/*
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			printf("ERROR!\n");
			//printf("error:%i\n", err);
			printf("error:%i\n", err);

			switch (err)
			{
			case GL_INVALID_ENUM:
				printf("error: 1\n");
				break;
			case GL_INVALID_VALUE:
				printf("error: 2\n");
				break;
			case GL_INVALID_OPERATION:
				printf("error: 3\n");
				break;
			case GL_STACK_OVERFLOW:
				printf("error: 4\n");
				break;
			case GL_STACK_UNDERFLOW:
				printf("error: 5\n");
				break;
			case GL_OUT_OF_MEMORY:
				printf("error: 6\n");
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				printf("error: 7\n");
				break;

			default:
				printf("error not found\n");
				break;
			}

			printf("\n");
		}
		*/
	}

	ASP_DestroyShader(state.shader);

	//glDeleteBuffers(1, &vbo);
	//glDeleteVertexArrays(1, &vao);

	SDL_DestroyWindow(window.handle);

	SDL_Quit();

	return 0;
}

int ASP_EventHandler()
{
	ASP_ResetKeyStates();
	SDL_Event event;
	SDL_PollEvent(&event);

	state.keyboardFocus = (window.handle == SDL_GetKeyboardFocus()) ? ASP_TRUE : ASP_FALSE;
	state.mouseFocus = (window.handle == SDL_GetMouseFocus()) ? ASP_TRUE : ASP_FALSE;

	ASP_HandleCursor();

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	DISPLAY_WIDTH = DM.w;
	DISPLAY_HEIGHT = DM.h;

	switch (event.type)
	{
	case SDL_QUIT:
		//state.running = ASP_FALSE;
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

int ASP_Render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	SDL_GL_SwapWindow(window.handle);
	return 0;
}

ASP_Sprite ASP_LoadSprite(char *name)
{
	ASP_Sprite sprite = ASP_SpriteC(0, 0, 0);
	int x, y, n;
	unsigned char *image = stbi_load(name, &x, &y, &n, 0);
	if (image == NULL)
	{
		printf("Failed to load image %s\n", name);
		return sprite;
	}

	printf("Image loaded! Image size: (%dpx, %dpx) channels: %d\n", x, y, n);

	int pixelcount = x * y;
	ASP_Color *pixels = malloc(pixelcount * sizeof(ASP_Color));
	int r, g, b, a;

	for (int imgx = 0; imgx < x; imgx++)
	{
		for (int imgy = 0; imgy < y; imgy++)
		{
			r = image[0];
			g = image[1];
			b = image[2];
			a = image[3];

			unsigned char *pixelOffset = image + (imgx + y * imgy) * n;
			unsigned char r = pixelOffset[0];
			unsigned char g = pixelOffset[1];
			unsigned char b = pixelOffset[2];
			unsigned char a = n >= 4 ? pixelOffset[3] : 0xff;

			a = 255;
			pixels[imgy * y + imgx] = ASP_ColorC(r, g, b, a);
		}
	}

	sprite.w = x;
	sprite.h = y;
	sprite.pixels = (ASP_Color *)pixels;
	stbi_image_free(image);
	return sprite;
}

ASP_Color ASP_SampleSprite(ASP_Sprite sprite, float x, float y)
{
	float nxo = overflowf(x, 0.0f, 1.0f);
	float nyo = overflowf(y, 0.0f, 1.0f);
	int nx = mapf(nxo, 0.0f, 1.0f, 0.0f, sprite.w);
	int ny = mapf(nyo, 0.0f, 1.0f, 1.0f, sprite.h) - 1;

	return sprite.pixels[ny * sprite.w + nx];
}

int ASP_DrawSprite(ASP_Sprite sprite, ASP_IVector2 position, ASP_IVector2 scale)
{
	ASP_Color color;
	float nx, ny;

	for (int x = 0; x < scale.x; x++)
	{
		for (int y = 0; y < scale.y; y++)
		{
			nx = mapf(x, 0, scale.x, 0, 4);
			ny = mapf(y, 0, scale.y, 0, 4);
			color = ASP_SampleSprite(sprite, nx, ny);
			ASP_DrawPixel(color, ASP_IVector2C(x, y));
		}
	}
}

int ASP_DrawPixel(ASP_Color color, ASP_IVector2 p)
{
	if (p.y < 0 || p.y >= SCREEN_HEIGHT || p.x < 0 || p.x >= SCREEN_WIDTH)
		return 1;
	int i = index(p.x, p.y, SCREEN_WIDTH);

	return 0;
}
int ASP_DrawLine(ASP_Color color, ASP_IVector2 p1, ASP_IVector2 p2)
{
	//SDL_SetRenderDrawColor(color.r, color.g, color.b, color.a);

	ASP_IVector2 dp = ASP_V_Sub_I2(p2, p1);

	float l_tr = (float)dp.y / (float)dp.x;
	float l_angle = atanf(l_tr);
	float l_length = sqrtf(dp.x * dp.x + dp.y * dp.y);
	int l_dirM = 2 * (p2.x >= p1.x) - 1;
	float xOffset = cosf(l_angle) * l_dirM;
	float yOffset = sinf(l_angle) * l_dirM;

	ASP_IVector2 drawpoint;
	for (int i = 0; i < l_length; i++)
	{
		drawpoint.x = round(p1.x + xOffset * i);
		drawpoint.y = round(p1.y + yOffset * i);
		ASP_DrawPixel(color, drawpoint);
	}

	return 0;
}

int ASP_DrawRect(ASP_Color color, ASP_IVector2 position, ASP_IVector2 scale)
{
	ASP_IVector2 p1 = ASP_IVector2C(position.x, position.y);
	ASP_IVector2 p2 = ASP_IVector2C(position.x + scale.x, position.y);
	ASP_IVector2 p3 = ASP_IVector2C(position.x + scale.x, position.y + scale.y);
	ASP_IVector2 p4 = ASP_IVector2C(position.x, position.y + scale.y);

	ASP_DrawLine(color, p1, p2);
	ASP_DrawLine(color, p2, p3);
	ASP_DrawLine(color, p3, p4);
	ASP_DrawLine(color, p4, p1);

	return 0;
}

int ASP_DrawFill(ASP_Color color)
{
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			ASP_DrawPixel(color, ASP_IVector2C(i, j));
		}
	}
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

ASP_IVector2 ASP_Project(ASP_FVector3 p, ASP_Entity camera, float fov, float depth)
{
	float dist = sqrtf(p.y * p.y + p.z * p.z + p.x * p.x);
	dist = dist == 0 ? 0.0001f : dist;
	float r = (depth / dist);
	p.x *= r;
	p.y *= r;
	p.z *= r;
	float ax = atan2f(p.z, p.y);
	float az = atan2f(p.x, p.y);
	int ssx = mapf(az, fov / 2, -fov / 2, 0, SCREEN_WIDTH);
	int ssy = mapf(ax, fov / 2, -fov / 2, 0, SCREEN_HEIGHT);
	return ASP_IVector2C(ssx, ssy);
}

int ASP_DrawEntity(ASP_Entity entity, ASP_Entity camera)
{
	ASP_FVector3 p;
	ASP_Color COLOR = ASP_ColorC(255, 255, 255, 255);
	float p_fov = PI / 2;
	float depth = PI * 168;

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
			p = entity.vertices[face[j]];

			/* ORIGIN RELATIVE SCALING */
			p.x *= entity.scale.x;
			p.y *= entity.scale.y;
			p.z *= entity.scale.z;

			/* ORIGIN RELATIVE ROTATION */
			p = ASP_RotateVector(-entity.rotation.x, p, 0);
			p = ASP_RotateVector(-entity.rotation.z, p, 2);
			p = ASP_RotateVector(-entity.rotation.y, p, 1);

			/* MOVING TO CAMERA RELATIVE POSITION */
			p.x = p.x - camera.position.x + entity.position.x;
			p.y = p.y - camera.position.y + entity.position.y;
			p.z = p.z - camera.position.z + entity.position.z;

			/* AXIS ROTATION AROUND CAMERA */
			p = ASP_RotateVector(camera.rotation.z, p, 2);
			p = ASP_RotateVector(camera.rotation.x, p, 0);
			p = ASP_RotateVector(camera.rotation.y, p, 1);

			if (p.y > 0)
			{
				tris[j] = ASP_Project(p, camera, p_fov, depth);
			}
			else
			{
				tris[j] = ASP_IVector2C(0, 0);
			}
		}

		ASP_DrawLine(COLOR, tris[0], tris[1]);
		ASP_DrawLine(COLOR, tris[1], tris[2]);
		ASP_DrawLine(COLOR, tris[2], tris[0]);

		ASP_IVector2 min, max = ASP_IVector2C(tris[0].x, tris[0].y);
		int vx, vy;
		for (int c = 0; c < 3; c++)
		{
			int vx = tris[c].x;
			int vy = tris[c].y;
			vx = (vx > SCREEN_WIDTH) ? SCREEN_WIDTH : vx;
			vy = (vy > SCREEN_HEIGHT) ? SCREEN_HEIGHT : vy;
			vx = (vx < 0) ? 0 : vx;
			vy = (vy < 0) ? 0 : vy;
			min.x = (vx < min.x) ? vx : min.x;
			min.y = (vy < min.y) ? vy : min.y;
			max.x = (vx > max.x) ? vx : max.x;
			max.y = (vy > max.y) ? vy : max.y;
		}

		for (int x = 0; x < max.x - min.x; x++)
		{
			for (int y = 0; y < max.y - min.y; y++)
			{
				//if (ASP_InTriangle(ASP_IVector2C(min.x + x, min.y + y), tris[0], tris[1], tris[2]) == 1)
				//{
				//	ASP_DrawPixel(renderer, entity.faceColor[i], ASP_IVector2C(min.x + x, min.y + y));
				//}
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

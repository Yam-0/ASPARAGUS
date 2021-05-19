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

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

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

int ASP_DrawEntity(ASP_Entity entity)
{
	if (entity.mesh != NULL)
		ASP_Mesh_Render(entity.mesh, &camera);

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

	struct ASP_MeshBuffer vertices, faces, indices;

	vertices.capacity = 3 * 8 * sizeof(int);
	faces.capacity = 3 * 12 * sizeof(int);
	indices.capacity = 1 * sizeof(int);

	ASP_Mesh_Init(&box.mesh);

	vertices.count = 8;
	faces.count = 12;
	indices.count = 1;

	vertices.index = sizeof(float);
	faces.index = sizeof(int);
	indices.index = sizeof(int);

	float _vertices[3 * 8] = {
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f};

	int _faces[3 * 12] = {0, 1, 3,
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

	int _indices[1] = {1};

	vertices.data = _vertices;
	faces.data = _faces;
	indices.data = _indices;

	box.mesh->vertices = vertices;
	box.mesh->faces = faces;
	box.mesh->indices = indices;

	box.type = 1;

	return box;
}

ASP_Entity ASP_GeneratePyramidEntity()
{
	ASP_Entity pyramid;

	return pyramid;
}

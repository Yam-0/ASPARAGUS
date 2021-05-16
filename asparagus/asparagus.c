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

float ASP_Runtime;

int ASPML_X, ASPML_Y, ASPML_DX, ASPML_DY = 0;

SDL_Window *window;

float PI = 3.141592f;
int ASP_FPS;

GLint uniColor, uniDepth, uniFov, uniWindowSize;

int ASP_init(int (*update)(float), int (*start)())
{
	printf("------- USING THE ASPARAGUS ENGINE -------\n");

	ASP_Running = 0;
	ASP_Runtime = 0;

	float deltatime = 0;
	int msec = 0;

	ASPK_Right, ASPK_Left, ASPK_Down, ASPK_Up = 0;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow("ASPARAGUS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GLContext mainContext = SDL_GL_CreateContext(window);
	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* SHADERS */
	char *vs1, *vs2, *fs1;

	//vs1 = ASP_LoadShader("../asparagus/shaders/vs.glsl");
	//GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//ASP_CompileShader(vertexShader, vs1);
	//free(vs1);

	vs2 = ASP_LoadShader("../asparagus/shaders/project.vert");
	GLuint projectVS = glCreateShader(GL_VERTEX_SHADER);
	ASP_CompileShader(projectVS, vs2);
	//free(vs2);

	fs1 = ASP_LoadShader("../asparagus/shaders/fs.glsl");
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	ASP_CompileShader(fragmentShader, fs1);
	//free(fs1);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, projectVS);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint vertexPosition = glGetAttribLocation(shaderProgram, "vertexPosition");
	GLint cameraPosition = glGetAttribLocation(shaderProgram, "cameraPosition");
	GLint cameraRotation = glGetAttribLocation(shaderProgram, "cameraRotation");
	GLint objectPosition = glGetAttribLocation(shaderProgram, "objectPosition");
	GLint objectScale = glGetAttribLocation(shaderProgram, "objectScale");
	GLint ObjectRotation = glGetAttribLocation(shaderProgram, "objectRotation");

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void *)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void *)(6 * sizeof(float)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void *)(9 * sizeof(float)));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void *)(12 * sizeof(float)));
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void *)(15 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	uniColor = glGetUniformLocation(shaderProgram, "uniColor");

	uniDepth = glGetUniformLocation(shaderProgram, "depth");
	uniFov = glGetUniformLocation(shaderProgram, "fov");
	uniWindowSize = glGetUniformLocation(shaderProgram, "windowSize");

	glUniform1f(uniDepth, PI / 2);
	glUniform1f(uniFov, PI);
	glUniform2f(uniWindowSize, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Start callback
	(*start)();
	ASP_Running = 1;

	//Update loop
	while (ASP_Running)
	{
		//Time before callback
		clock_t before = clock();

		ASP_EventHandler();

		//Update callback
		(*update)(deltatime);

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		SDL_GL_SwapWindow(window);

		//Frame time & deltatime
		clock_t difference = clock() - before;
		msec = difference * 1000 / CLOCKS_PER_SEC;
		deltatime = (float)msec / 1000;
		deltatime = (deltatime == 0.0f) ? deltatime + 0.0001f : deltatime;
		ASP_FPS = 1.0f / deltatime;
		ASP_Runtime += deltatime;
	}

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(projectVS);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}

const char *ASP_LoadShader(char *filename)
{
	FILE *sfh = fopen(filename, "r");
	if (sfh == NULL)
	{
		printf("Could not read file: %s\n", filename);
		ferror("error");
		fclose(sfh);
		return NULL;
	}

	printf("Read file: %s\n", filename);

	int nlc = 0;
	char c;
	for (c = getc(sfh); c != EOF; c = getc(sfh))
		if (c == '\n')
			nlc++;

	rewind(sfh);
	long int size = 0;
	fseek(sfh, -nlc, SEEK_END);
	size = ftell(sfh);
	rewind(sfh);

	char *shader = (char *)malloc(size);
	if (!shader)
	{
		printf("Memory error in ASP_LoadShader.\n");
		fclose(sfh);
		return NULL;
	}

	size_t readSize = fread(shader, 1, size, sfh);
	readSize = (readSize >= 0) ? readSize : 0;
	shader[readSize] = '\0';
	if (readSize != size)
	{
		printf("Read error in ASP_LoadShader.\n");
		printf("Read size %zu, malloc size %i.\n", readSize, size);
		fclose(sfh);
		return NULL;
	}

	printf("\nLoaded Shader \"%s\":\n%s\nEND\n", filename, shader);
	fclose(sfh);

	return shader;
}

GLuint ASP_CompileShader(GLuint shader, char *code)
{
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_TRUE)
	{
		printf("Compiled shader!\n");
		return shader;
	}
	else
	{
		printf("Failed to compile shader!\n");
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		return shader;
	}
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

int ASP_Render(SDL_Window *window)
{
	glDrawArrays(GL_LINES, 0, 2);
	glfwSwapBuffers(window);
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

#include "include/asparagus.h"

int DISPLAY_WIDTH;
int DISPLAY_HEIGHT;

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

	/* SHADERS - LOAD, COMPILE, LINK and BIND */
	state.shader = ASP_Shader_Create(
		"../data/shaders/shader.vert",
		"../data/shaders/shader.frag", 1,
		(struct ASP_VertexAttribute[]){
			{.index = 0, .name = "position"}, //Vertex position
											  //{.index = 1, .name = "color"},
											  //{.index = 2, .name = "uv"},
		});
	ASP_Shader_Bind(state.shader);

	//Public camera, unattached.
	camera = ASP_Camera_Create(PI / 2, NULL);

	//Opengl configurations
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Start callback
	window.start();

	//Update loop
	while (state.running)
	{
		//Time before callback
		clock_t before = clock();

		ASP_EventHandler();
		glClearColor(0.2f, 0.4f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, state.wireframe ? GL_LINE : GL_FILL);

		//Update callback
		window.update();

		ASP_Camera_Update(&camera);
		ASP_Render();

		//Frame time & deltatime
		clock_t difference = clock() - before;
		msec = difference * 1000 / CLOCKS_PER_SEC;
		deltatime = (float)msec / 1000;
		window.deltatime = (deltatime == 0.0f) ? deltatime + 0.0001f : deltatime;
		window.fps = 1.0f / deltatime;
		window.totalSeconds += deltatime;
	}

	window.destroy();

	ASP_Shader_Destroy(state.shader);

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
	SDL_GL_SwapWindow(state.window->handle);
	return 0;
}

ASP_Entity ASP_Entity_Create(char *name)
{
	ASP_Entity entity;
	strcpy(entity.name, name);
	entity.id = 0;
	entity.position = ASP_FVector3C(0, 0, 0);
	entity.rotation = ASP_FVector3C(0, 0, 0);
	entity.scale = ASP_FVector3C(1, 1, 1);
	entity.type = 1;
	return entity;
}

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

	/* SHADERS */
	state.shader = ASP_CreateShader(
		"../data/shaders/shader.vert",
		"../data/shaders/shader.frag", 1,
		(struct ASP_VertexAttribute[]){
			{.index = 0, .name = "position"} //Vertex position
											 //{.index = 1, .name = "tmp"},
											 //{.index = 2, .name = "color"},
		});
	ASP_BindShader(state.shader);

	//--------------------------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left
		0.5f, -0.5f, 0.0f,	// right
		0.0f, 0.5f, 0.0f	// top
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//--------------------------------------------------------------------------------------

	//Camera
	camera = ASP_CreateCamera(PI / 2, NULL);

	//Start callback
	//window.start();

	//Update loop
	while (state.running)
	{
		//Time before callback
		clock_t before = clock();

		ASP_EventHandler();

		glClearColor(0.2f, 0.4f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(state.shader.shader_handle);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		SDL_GL_SwapWindow(state.window->handle);
		//Update callback
		//window.update();

		//ASP_UpdateCamera(&camera);
		//ASP_Render();

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
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	SDL_GL_SwapWindow(window.handle);
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
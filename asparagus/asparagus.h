/* THE ASPARAGUS ENGINE BY TAGE ÅKERSTRÖM */
/* Project started : 15/4/2021 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "structs.c"
#include "aspgeneric.c"
#include "aspkeys.c"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h";

/* Initiate asparagus with callback pointers */
int ASP_init(int (*update)(float), int (*start)());

/* Load asparagus shader from ./shader folder */
const char *ASP_LoadShader(char *filename);

/* Compile glsl shader */
GLuint ASP_CompileShader(GLuint shader, char *code);

/* Hard sleep for x milli secs */
int ASP_sleep(int m_secs);

/* Render to window */
int ASP_Render(SDL_Window *window);

/* Asparagus internal event handler */
int ASP_EventHandler();

/* Load an image file to ASP_Sprite sprite */
ASP_Sprite ASP_LoadSprite(char name[128]);

/* Sample ASP_Sprite at normalized (x, y) position */
ASP_Color ASP_SampleSprite(ASP_Sprite sprite, float x, float y);

/* Draw ASP_Sprite to the render surface */
int ASP_DrawSprite(ASP_Sprite sprite, ASP_IVector2 position, ASP_IVector2 scale);

/* Draws a pixel to the render surface */
int ASP_DrawPixel(ASP_Color color, ASP_IVector2 p);

/* Draws a line to the render surface */
int ASP_DrawLine(ASP_Color color, ASP_IVector2 p1, ASP_IVector2 p2);

/* Draws a rectangle to the render surface */
int ASP_DrawRect(ASP_Color color, ASP_IVector2 position, ASP_IVector2 scale);

/* Fills the render surface with color */
int ASP_DrawFill(ASP_Color color);

/* Draws an ASP entity to the render surface */
int ASP_DrawEntity(ASP_Entity entity, ASP_Entity camera);

/* Generates a box :P */
ASP_Entity ASP_GenerateBoxEntity();

/* Generates a pyramid :O */
ASP_Entity ASP_GeneratePyramidEntity();
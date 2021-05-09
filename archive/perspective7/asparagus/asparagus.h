/* THE ASPARAGUS ENGINE BY TAGE ÅKERSTRÖM */
/* Made : 15/4/2021 */

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "../sdl/include/SDL2/SDL.h"
#include "./structs.c"
#include "./aspgeneric.c"
#include "./aspkeys.c"

/* Initiate asparagus with callback pointers */
int ASP_init(int (*update)(float), int (*start)());

/* Hard sleep for x milli secs */
int ASP_sleep(int m_secs);

/* Render to window */
int ASP_Render(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *texture);

/* Asparagus internal event handler */
int ASP_EventHandler();

/* Draws a pixel to the render surface */
int ASP_DrawPixel(SDL_Renderer *renderer, ASP_Color color, ASP_IVector2 p);

/* Draws a line to the render surface */
int ASP_DrawLine(SDL_Renderer *renderer, ASP_Color color, ASP_IVector2 p1, ASP_IVector2 p2);

/* Draws a rectangle to the render surface */
int ASP_DrawRect(SDL_Renderer *renderer, ASP_Color color, ASP_IVector2 position, ASP_IVector2 scale);

/* Fills the render surface with a color */
int ASP_DrawFill(SDL_Renderer *renderer, ASP_Color color);

/* Draws an ASP entity to the render surface */
int ASP_DrawEntity(ASP_Entity entity, ASP_Entity camera);

/* Generates a box :P */
ASP_Entity ASP_GenerateBoxEntity();

/* Generates a pyramid :O */
ASP_Entity ASP_GeneratePyramidEntity();
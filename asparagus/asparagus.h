/* THE ASPARAGUS ENGINE BY TAGE ÅKERSTRÖM */
/* Made : 15/4/2021 */

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "../sdl/include/SDL2/SDL.h"
#include "./structs.c"
#include "./aspgeneric.c"

/* Initiate asparagus with callback pointers */
int ASP_init(int (*update)(float), int (*start)());

/* Hard sleep for x milli secs */
int ASP_sleep(int m_secs);

/* Render to window */
int ASP_Render(SDL_Renderer *renderer, SDL_Window *window);

/* Asparagus internal event handler */
int ASP_EventHandler();

/* Draws a pixel to the renderer surface */
int ASP_DrawPixel(SDL_Renderer *renderer, ASP_Color color, ASP_IVector2 p);

/* Draws a line to the renderer surface */
int ASP_DrawLine(SDL_Renderer *renderer, ASP_Color color, ASP_IVector2 p1, ASP_IVector2 p2);

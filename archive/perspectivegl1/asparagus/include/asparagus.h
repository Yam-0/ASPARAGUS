/* THE ASPARAGUS ENGINE BY TAGE ÅKERSTRÖM */
/* Project started : 15/4/2021 */

#ifndef ASPARAGUS_H
#define ASPARAGUS_H

#define ASP_AXIS_X 0
#define ASP_AXIS_Y 1
#define ASP_AXIS_Z 2
#define ASP_TRUE 1
#define ASP_FALSE 0

#include "../mesh.c"
#include "../camera.c"
#include "util.h"
#include "../window.c"
#include "../shader.c"

/* Initiate asparagus with callback pointers */
int ASP_init(_ASP_CALLBACK start, _ASP_CALLBACK update, _ASP_CALLBACK tick, _ASP_CALLBACK destroy);

/* Render to window */
int ASP_Render();

/* Asparagus internal event handler */
int ASP_EventHandler();

/* Generates an ASP entity */
ASP_Entity ASP_Entity_Create(char *name);

#endif
#ifndef UTIL_H
#define UTIL_H

#include "asparagus.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/call.h>

#include "../aspgeneric.c"
#include "../aspkeys.c"

#include "mesh.h"
#include "vao.h"
#include "vbo.h"
#include "camera.h"
#include "shader.h"
#include "state.h"
#include "window.h"
#include "vec.h"
#include "structs.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h";

#endif
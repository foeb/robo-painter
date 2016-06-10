#pragma once

#include <assert.h>
#include <math.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "kosher.h"

double perlin_generate(double x, double y);
int l_perlin_generate(lua_State *L);

static const struct luaL_Reg libperlin[] = {
    { "generate", l_perlin_generate },
    { NULL, NULL }
};

int luaopen_libperlin(lua_State *L)
{
    luaL_newlib(L, libperlin);
    return 1;
}

/* PERLIN_WEIGHT: the weight function for perlin_generate. This polynomial has
 * a continuous second derivative everywhere. */
/* #define PERLIN_WEIGHT(t) (6.0 * pow(t, 5) - 15.0 * pow(t, 4) + 10.0 * pow(t, 3)) */
#define PERLIN_WEIGHT(t) (3.0 * t*t - 2.0 * t*t*t)

/* PERLIN_SIZE: the length of perlin_permtable and perlin_gradient_grid */
#define PERLIN_SIZE 256


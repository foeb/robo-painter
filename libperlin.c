#include "libperlin.h"

/* lerp: linear interpolation between a1 and a2 with a weight w 
 * between 0 and 1 */
double perlin_lerp(double a1, double a2, double w) 
{
    assert(0.0 <= w && w <= 1.0);
    return (1.0 - w) * a1 + w * a2;
}

/* perlin_get: returns the gradient at x, y as a pointer to two values */
double *perlin_get(int x, int y)
{
    return perlin_gradient_grid[(x + perlin_permtable[y % PERLIN_SIZE]) 
                                                            % PERLIN_SIZE];
}

/* perlin_dot_grid: perform the dot product of x, y and the gradient at
 * gx, gy.  */
double perlin_dot_grid(int gx, int gy, double x, double y)
{
    double *p = perlin_get(gx, gy);
    return (x - gx) * p[0] + (y - gy) * p[1];
}

/* perlin_generate: return the value of this perlin noise at x, y */
static double perlin_generate(double x, double y)
{
    assert(!isnan(x) && !isinf(x));
    assert(!isnan(y) && !isinf(y));
    int x0 = floor(x);
    int x1 = x0 + 1;
    int y0 = floor(y);
    int y1 = y0 + 1;
    double sx = PERLIN_WEIGHT((x - x0));
    double sy = PERLIN_WEIGHT((y - y0));
    double n0 = perlin_dot_grid(x0, y0, x, y);
    double n1 = perlin_dot_grid(x1, y0, x, y);
    double ix0 = perlin_lerp(n0, n1, sx);
    n0 = perlin_dot_grid(x0, y1, x, y);
    n1 = perlin_dot_grid(x1, y1, x, y);
    double ix1 = perlin_lerp(n0, n1, sx);
    return perlin_lerp(ix0, ix1, sy);
}

/* l_perlin_generate: Lua interface to perlin_generate */
static int l_perlin_generate(lua_State *L)
{
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 2);
    lua_pushnumber(L, perlin_generate(x, y));
    return 1;
}

/*
int main()
{
    lua_State *L = luaL_newstate();
    lua_pushcfunction(L, l_perlin_generate);
    lua_setglobal(L, "perlin_generate");
    lua_close(L);
}
*/


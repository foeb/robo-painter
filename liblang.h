#pragma once

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "kosher.h"
//#include "libperlin.h"

extern double perlin_generate(double, double);

typedef uint_fast8_t lang_word_t;

typedef struct {
  int length;
  int seed;
  lang_word_t words[1];
} l_lang_exp;

double lang_apply_fun(lang_word_t word, double x, double y, double a, double b);
lang_word_t lang_random_terminal();
lang_word_t lang_random_word();
int lang_get_degree(lang_word_t word);
double lang_map_exp(l_lang_exp *t,
             int (*fun)(int nvalues, lang_word_t *values, int values_top,
                      double *results, int results_top, double x, double y), 
             int do_average, double x, double y);
int lang_interpret_fn(int nvalues, lang_word_t *values, int values_top, 
                        double *results, int results_top, double x, double y);
double lang_interpret(l_lang_exp *exp, double x, double y);
int lang_generate_exp(int seed, int maxdepth, lang_word_t *result, int alloc_new_result);
void lang_print_exp(lang_word_t *exp, int exp_length);

int l_lang_generate_exp(lua_State *L);
int l_lang_to_exp(lua_State *L);
int l_lang_interpret(lua_State *L);
int l_lang_print_exp(lua_State *L);

const struct luaL_Reg liblang[] = {
  { "generate", l_lang_generate_exp },
  { "print", l_lang_print_exp },
  { "to_exp", l_lang_to_exp },
  { "interpret", l_lang_interpret },
  { NULL, NULL }
};

int luaopen_liblang(lua_State *L)
{
  luaL_newmetatable(L, "lang.exp");
  luaL_newlib(L, liblang);
  return 1;
}


#define PI 3.1415926
#define HALF_PI 1.57079632

/* Not including LANG_WORD_A and LANG_WORD_B for the time-being */
#define LANG_TOTAL_WORDS    0x17

#define LANG_WORD_X         0x00
#define LANG_WORD_Y         0x01
#define LANG_WORD_PERLIN    0x02
#define LANG_WORD_RAND      0x03
#define LANG_WORD_ADD       0x04
#define LANG_WORD_SUB       0x05
#define LANG_WORD_MUL       0x06
#define LANG_WORD_DIV       0x07
#define LANG_WORD_POW       0x08
#define LANG_WORD_SIN       0x09
#define LANG_WORD_COS       0x0a
#define LANG_WORD_TAN       0x0b
#define LANG_WORD_SINH      0x0c
#define LANG_WORD_COSH      0x0e
#define LANG_WORD_TANH      0x0d
#define LANG_WORD_ABS       0x0f
#define LANG_WORD_EXP       0x10
#define LANG_WORD_LOG       0x11
#define LANG_WORD_MIN       0x12
#define LANG_WORD_MAX       0x13
#define LANG_WORD_SQRT      0x14
#define LANG_WORD_LGAMMA    0x15
#define LANG_WORD_HYPBOLIC  0x16
#define LANG_WORD_A         0x17
#define LANG_WORD_B         0x18

#define LANG_FUN_ADD(x,y,a,b) ((a) + (b))
#define LANG_FUN_SUB(x,y,a,b) ((a) - (b))
#define LANG_FUN_MUL(x,y,a,b) ((a) * (b))
#define LANG_FUN_DIV(x,y,a,b) ((b) == 0 ? (a) : (a)/(b))
#define LANG_FUN_POW(x,y,a,b) (pow((a), ceil(ceil(b))))
#define LANG_FUN_SIN(x,y,a,b) (sin(a))
#define LANG_FUN_COS(x,y,a,b) (cos(a))
#define LANG_FUN_TAN(x,y,a,b) (tan(a))
#define LANG_FUN_SINH(x,y,a,b) (sinh(a))
#define LANG_FUN_COSH(x,y,a,b) (cosh(a))
#define LANG_FUN_TANH(x,y,a,b) (tanh(a))
#define LANG_FUN_ABS(x,y,a,b) (fabs(a))
#define LANG_FUN_EXP(x,y,a,b) (exp(a))
#define LANG_FUN_LOG(x,y,a,b) (log(a))
#define LANG_FUN_MIN(x,y,a,b) (fmin((a), (b)))
#define LANG_FUN_MAX(x,y,a,b) (fmax((a), (b)))
#define LANG_FUN_SQRT(x,y,a,b) ((a) <  0 ? (a) : sqrt(a))
#define LANG_FUN_LGAMMA(x,y,a,b) ((a) <= 0 ? (a) : lgamma(a))
#define LANG_FUN_RAND(x,y,a,b) (rand()/RAND_MAX)
#define LANG_FUN_X(x,y,a,b) (x)
#define LANG_FUN_Y(x,y,a,b) (y)
#define LANG_FUN_PERLIN(x,y,a,b) \
      ((a) == 0 ? perlin_generate((x) / HALF_PI, (y) / HALF_PI) \
                : perlin_generate((x) / (a), (y) / (a)))
#define LANG_FUN_HYPBOLIC(x,y,a,b) ((a) == 0 && (b) == 0 ? \
      (x)*(x) - (y)*(y) : ((x)*(x))/((a)*(a)) - ((y)*(y))/((b)*(b)))
#define LANG_FUN_A(x,y,a,b) (a)
#define LANG_FUN_B(x,y,a,b) (b)



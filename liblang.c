#include "liblang.h"

double lang_apply_fun(lang_word_t fun, double x, double y, double a, double b)
{
    switch(fun) {
        case LANG_WORD_X:
            return LANG_FUN_X(x, y, a, b);
        case LANG_WORD_Y:
            return LANG_FUN_Y(x, y, a, b);
        case LANG_WORD_PERLIN:
            return LANG_FUN_PERLIN(x, y, a, b);
        case LANG_WORD_RAND:
            return LANG_FUN_RAND(x, y, a, b);
        case LANG_WORD_ADD:
            return LANG_FUN_ADD(x, y, a, b);
        case LANG_WORD_SUB:
            return LANG_FUN_SUB(x, y, a, b);
        case LANG_WORD_MUL:
            return LANG_FUN_MUL(x, y, a, b);
        case LANG_WORD_DIV:
            return LANG_FUN_DIV(x, y, a, b);
        case LANG_WORD_POW:
            return LANG_FUN_POW(x, y, a, b);
        case LANG_WORD_SIN:
            return LANG_FUN_SIN(x, y, a, b);
        case LANG_WORD_COS:
            return LANG_FUN_COS(x, y, a, b);
        case LANG_WORD_TAN:
            return LANG_FUN_TAN(x, y, a, b);
        case LANG_WORD_SINH:
            return LANG_FUN_SINH(x, y, a, b);
        case LANG_WORD_COSH:
            return LANG_FUN_COSH(x, y, a, b);
        case LANG_WORD_TANH:
            return LANG_FUN_TANH(x, y, a, b);
        case LANG_WORD_ABS:
            return LANG_FUN_ABS(x, y, a, b);
        case LANG_WORD_EXP:
            return LANG_FUN_EXP(x, y, a, b);
        case LANG_WORD_LOG:
            return LANG_FUN_LOG(x, y, a, b);
        case LANG_WORD_MIN:
            return LANG_FUN_MIN(x, y, a, b);
        case LANG_WORD_MAX:
            return LANG_FUN_MAX(x, y, a, b);
        case LANG_WORD_SQRT:
            return LANG_FUN_SQRT(x, y, a, b);
        case LANG_WORD_LGAMMA:
            return LANG_FUN_LGAMMA(x, y, a, b);
        case LANG_WORD_HYPBOLIC:
            return LANG_FUN_HYPBOLIC(x, y, a, b);
        case LANG_WORD_A:
            return LANG_FUN_A(x, y, a, b);
        case LANG_WORD_B:
            return LANG_FUN_B(x, y, a, b);
        default:
            assert(0); /* function not found */
            break;
    }
    return 0;
}

int lang_get_degree(lang_word_t fun)
{
    switch(fun) {
        case LANG_WORD_X:
            return 0;
        case LANG_WORD_Y:
            return 0;
        case LANG_WORD_PERLIN:
            return 1;
        case LANG_WORD_RAND:
            return 0;
        case LANG_WORD_ADD:
            return 2;
        case LANG_WORD_SUB:
            return 2;
        case LANG_WORD_MUL:
            return 2;
        case LANG_WORD_DIV:
            return 2;
        case LANG_WORD_POW:
            return 2;
        case LANG_WORD_SIN:
            return 1;
        case LANG_WORD_COS:
            return 1;
        case LANG_WORD_TAN:
            return 1;
        case LANG_WORD_SINH:
            return 1;
        case LANG_WORD_COSH:
            return 1;
        case LANG_WORD_TANH:
            return 1;
        case LANG_WORD_ABS:
            return 1;
        case LANG_WORD_EXP:
            return 1;
        case LANG_WORD_LOG:
            return 1;
        case LANG_WORD_MIN:
            return 2;
        case LANG_WORD_MAX:
            return 2;
        case LANG_WORD_SQRT:
            return 1;
        case LANG_WORD_LGAMMA:
            return 1;
        case LANG_WORD_HYPBOLIC:
            return 2;
        case LANG_WORD_A:
            return 2;
        case LANG_WORD_B:
            return 2;
        default:
            assert(0); /* function not found */
            break;
    }
    return 0;
}

lang_word_t lang_random_word()
{
    return rand() % LANG_TOTAL_WORDS;
}

lang_word_t lang_random_terminal()
{
    return rand() % 4;
}

double lang_current_x = 0.0;
double lang_current_y = 0.0;

int lang_interpret_fn(int nvalues, tree_node_t *values, int values_top, 
                        double *results, int results_top)
{
    switch(nvalues) {
        case 0:
            results[results_top++] = 
                lang_apply_fun(values[values_top-1], 
                                    lang_current_x, lang_current_y, 0, 0);
            break;
        case 1:
            results[results_top - 1] =
                lang_apply_fun(values[values_top-1],
                                    lang_current_x, lang_current_y, 
                                    results[results_top - 1], 0);
            break;
        case 2:
            results[(results_top--) - 2] =
                lang_apply_fun(values[values_top-1],
                                    lang_current_x, lang_current_y, 
                                    results[results_top - 2], 
                                    results[results_top - 1]);
            break;
        default:
            assert(0);
            break;
    }
    return results_top;
}

double *lang_interpret(lang_word_t *exp, int exp_length, double x, double y)
{
    int *degrees = calloc(exp_length, sizeof(int));
    double *results = NULL;
    if (degrees != NULL) {
        for (int i = 0; i < exp_length; ++i) {
            degrees[i] = lang_get_degree(exp[i]);
        }
        results = tree_map(tree_create(exp, degrees, exp_length),
                                lang_interpret_fn);
    }
    free(degrees);
    return results;
}

int lang_generate_exp_maxlength(int maxdepth)
{
    /* By convention, we say the root starts at depth 0. */
    return (int)(pow(2, maxdepth + 1) - 1);
}

/* FIXME */
int lang_generate_exp(int seed, int maxdepth, lang_word_t *result, int alloc_new_result)
{
    assert(maxdepth >= 0);
    srand(seed);
    int maxlength = lang_generate_exp_maxlength(maxdepth);

    if (alloc_new_result) {
        result = calloc(maxlength, sizeof(lang_word_t));
    }

    /* the number of available arguments for the next word */
    int open_exps = 0;
    int current_depth = 0;
    for (int i = 0; i < maxlength; ++i) {
        if (open_exps == 0 || current_depth >= maxdepth) {
            result[i] = lang_random_terminal();
        } else {
            lang_word_t next_word = 0;
            do {
                next_word = lang_random_word();
            } while(lang_get_degree(next_word) > open_exps);
            result[i] = next_word;
        }
        open_exps += lang_get_degree(result[i]) - 1;
    }
    return maxlength;
}

void lang_print_exp(lang_word_t *exp, int exp_length)
{
    for (int i = 0; i < exp_length; ++i) {
        if ((i % 16) == 0 && i != 0) { printf("\n"); }
        printf(" %2x", exp[i]);
    }
}

/* ---------- Lua Interface---------- */

int l_lang_generate_exp(lua_State *L)
{
    int seed = luaL_checkinteger(L, 1);
    int maxdepth = luaL_checkinteger(L, 2);
    int maxlength = lang_generate_exp_maxlength(maxdepth);
    l_lang_exp *new_exp = 
        (l_lang_exp *)lua_newuserdata(L, sizeof(l_lang_exp) + sizeof(lang_word_t) * maxlength);
    int length = lang_generate_exp(seed, maxdepth, new_exp->words, 0);
    new_exp->seed = seed;
    new_exp->length = length;
    return 1;
}

int l_lang_print_exp(lua_State *L)
{
    l_lang_exp *exp = (l_lang_exp *)lua_touserdata(L, 1);
    lang_print_exp(exp->words, exp->length);
    return 0;
}


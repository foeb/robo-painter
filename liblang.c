#include "liblang.h"

/* lang_apply_fun: apply the function referred to by word to x, y, a, b */
double lang_apply_fun(lang_word_t word, double x, double y, double a, double b)
{
    MAKE_KOSHER(a);
    MAKE_KOSHER(b);

    if (LANG_WORD_IS_CONST(word)) {
        return LANG_WORD_TO_CONST(word);
    }

    switch(word) {
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
        case LANG_WORD_PERLIN2:
            return LANG_FUN_PERLIN(x, y, a, b);
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

/* lang_get_degree: return the number of arguments the function referred to
 * by word takes, not counting x and y. */
int lang_get_degree(lang_word_t word)
{
    if (LANG_WORD_IS_CONST(word)) {
        return 0;
    }

    switch(word) {
        case LANG_WORD_X:
            return 0;
        case LANG_WORD_Y:
            return 0;
        case LANG_WORD_PERLIN:
            return 0;
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
        case LANG_WORD_PERLIN2:
            return 1;
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

/* lang_random_word: returns a random, valid word */
lang_word_t lang_random_word()
{
    return rand() % LANG_TOTAL_WORDS;
}

/* lang_random_terminal: returns a random, valid word referring to a function
 * that takes no arguments other than x and y. */
lang_word_t lang_random_terminal()
{
    int result = rand() % 5;
    if (result == 4) {
        return (rand() & 0x7F) | 0x80;
    } else {
        return result;
    }
}

double average(double *values, int length)
{
    double acc = 0;
    for (int i = 0; i < length; ++i) {
        acc += values[i];
    }
    return acc/length;
}

/* lang_map_exp: Evaluates fun at each node of the tree, given access to their
 * children. If do_average is nonzero, then lang_map_exp returns
 * the average of all of the results. Otherwise, only the first result is returned.
 * See TAOCP 2.2.3, Algorithm F. */
double lang_map_exp(l_lang_exp *t,
             int (*fun)(int nwords, lang_word_t *words, int words_top,
                        double *results, int results_top, double x, double y),
             int do_average, double x, double y)
{
    lang_word_t  *stack = calloc(t->length, sizeof(lang_word_t));
    double *results_stack = calloc(t->length, sizeof(double));
    int words_top = 0;
    int results_top = 0;
    int current_degree = 0;
    double result = 0;
    if (stack != NULL && results_stack != NULL) {
        for (int i = 0; i < t->length; ++i) {
            current_degree = lang_get_degree(t->words[i]);
            stack[words_top++] = t->words[i];
            results_top = 
                fun(current_degree+1, stack, words_top, 
                        results_stack, results_top, x, y);
            words_top -= current_degree;
            assert(words_top >= 0);
        }

        if (do_average) {
            result = average(results_stack, results_top);
        } else {
            result = results_stack[0];
        }
    }

    free(stack);
    free(results_stack);

    return result;
}

/* lang_interpret_fn: is passed to lang_map_exp by lang_interpret. */
int lang_interpret_fn(int nwords, lang_word_t *words, int words_top, 
                        double *results, int results_top, double x, double y)
{
    switch(nwords) {
        case 1:
            results[results_top++] = 
                lang_apply_fun(words[words_top-1], 
                                    x, y, 0, 0);
            break;
        case 2:
            results[results_top - 1] =
                lang_apply_fun(words[words_top-1],
                                    x, y, 
                                    results[results_top - 1], 0);
            break;
        case 3:
            results[results_top - 2] =
                lang_apply_fun(words[words_top-1],
                                    x, y, 
                                    results[results_top - 2], 
                                    results[results_top - 1]);
            --results_top;
            break;
        default:
            printf("Invalid nwords: %i, current value: %02x\n", nwords, words[words_top-1]);
            assert(0);
            break;
    }
    return results_top;
}

/* lang_interpret: evaluates an array of words exp of length exp_length at
 * location (x, y). */
double lang_interpret(l_lang_exp *exp, double x, double y)
{
    double result = lang_map_exp(exp, lang_interpret_fn, 1, x, y);
    return MAKE_KOSHER(result);
}

/* lang_generate_exp_maxlength: returns the total nodes of a perfect binary
 * tree of depth maxdepth. */
int lang_generate_exp_maxlength(int maxdepth)
{
    /* By convention, we say the root starts at depth 0. */
    return (int)(pow(2, maxdepth + 1) - 1);
}

/* lang_generate_exp: stores a new, pseudo-randomly generated expression
 * tree in the location pointed to by result. If alloc_new_result is
 * nonzero, lang_generate_exp allocates a new block of memory and sets result
 * to point to that. Returns the length of the new expression. */
int lang_generate_exp(int seed, int maxdepth, lang_word_t *result, int alloc_new_result)
{
    assert(maxdepth >= 0);
    srand(seed);
    int maxlength = lang_generate_exp_maxlength(maxdepth);

    if (alloc_new_result) {
        result = calloc(maxlength, sizeof(lang_word_t));
    }

    if (result != NULL) {
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
            open_exps += 1 - lang_get_degree(result[i]);
        }
    }
    return maxlength;
}

/* lang_print_exp: prints the expression tree as a list of words in rows
 * 16 columns across. */
void lang_print_exp(lang_word_t *exp, int exp_length)
{
    for (int i = 0; i < exp_length; ++i) {
        if ((i % 16) == 0) { printf("\n"); }
        printf(" %02x", exp[i]);
    }
    printf("\n");
}

/* ---------- Lua Interface---------- */

/* l_lang_generate_exp: the lua interface to lang_generate_exp. Takes two
 * integer arguments, seed and maxdepth. */
int l_lang_generate_exp(lua_State *L)
{
    int seed = luaL_checkinteger(L, 1);
    int maxdepth = luaL_checkinteger(L, 2);
    int maxlength = lang_generate_exp_maxlength(maxdepth);
    size_t nbytes = sizeof(l_lang_exp) + sizeof(lang_word_t) * maxlength;
    l_lang_exp *new_exp = 
        (l_lang_exp *)lua_newuserdata(L, nbytes);
    int length = lang_generate_exp(seed, maxdepth, new_exp->words, 0);
    new_exp->seed = seed;
    new_exp->length = length;

    luaL_getmetatable(L, "lang.exp");
    lua_setmetatable(L, -2);

    return 1;
}

/* l_lang_to_exp: takes a lua table and returns a new expression with the
 * table's values as the words. */
int l_lang_to_exp(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    int length = luaL_len(L, 1);
    size_t nbytes = sizeof(l_lang_exp) + sizeof(lang_word_t) * length;
    l_lang_exp *new_exp = 
        (l_lang_exp *)lua_newuserdata(L, nbytes);
    new_exp->seed = 0;
    new_exp->length = length;
    for (int i = 0; i <= length; ++i) {
        lua_rawgeti(L, 1, i);
        new_exp->words[i] = (lang_word_t)(lua_tointeger(L, -1));
        lua_pop(L, 1);
    }

    luaL_getmetatable(L, "lang.exp");
    lua_setmetatable(L, -2);

    return 1;
}

/* l_lang_print_exp: prints the expression at the top of the stack. The lua
 * interface to lang_print_exp. */
int l_lang_print_exp(lua_State *L)
{
    l_lang_exp *exp = luaL_checkudata(L, 1, "lang.exp");
    lang_print_exp(exp->words, exp->length);
    return 0;
}

/* l_lang_interpret: the lua interface to lang_interpret. It takes an
 * expression as the first argument and the x and y coordinates as the second
 * and third. Returns the result. */
int l_lang_interpret(lua_State *L)
{
    l_lang_exp *exp = luaL_checkudata(L, 1, "lang.exp");
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);

    double result = lang_interpret(exp, x, y);
    lua_pushnumber(L, result);

    return 1;
}

/* l_lang_geti: returns the word at location i (starting index at 1) */
int l_lang_geti(lua_State *L)
{
    l_lang_exp *exp = luaL_checkudata(L, 1, "lang.exp");
    int i = luaL_checkinteger(L, 2);
    luaL_argcheck(L, 0 < i && i <= exp->length, 2, "index out of range");
    lua_pushnumber(L, exp->words[i]);
    return 1;
}

/* l_lang_length: returns the length of the given exp */
int l_lang_length(lua_State *L)
{
    l_lang_exp *exp = luaL_checkudata(L, 1, "lang.exp");
    lua_pushnumber(L, exp->length);
    return 1;
}

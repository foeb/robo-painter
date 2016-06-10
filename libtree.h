#ifndef TREE_HEADER
#define TREE_HEADER

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "kosher.h"

typedef uint_fast8_t tree_node_t;

typedef struct {
  tree_node_t *nodes;
  int *degrees;
  int length;
} tree_t;

tree_t *tree_create(tree_node_t *nodes, int *degrees, int length);
tree_t *tree_clone(tree_t *t);
void tree_free(tree_t *t);

tree_node_t tree_geti(tree_t *t, int index);
int tree_geti_degree(tree_t *t, int index);
tree_t *tree_seti(tree_t *t, int index, tree_node_t value, int degree);
/*
tree_t *tree_get(tree_t *t, int index);
tree_t *tree_set(tree_t *t, int index, tree_t *subtree);
*/
int tree_random_index(tree_t *t);

tree_t *tree_eval(tree_t *t, 
             int (*fun)(tree_node_t *values, int nvalues, int stack_top, tree_node_t *results));

#endif

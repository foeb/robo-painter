#include "libtree.h"
#include <stdio.h>

/* tree_create: return a new tree with the given values. Memory is allocated
 * for the new struct and it is the user's responsibility to free it through
 * free(t) or tree_free(t). */
tree_t *tree_create(tree_node_t *nodes, int *degrees, int length)
{
    assert(length >= 0);
    tree_t *self = malloc(sizeof(tree_t));
    if (self != NULL) {
        self->nodes = nodes;
        self->degrees = degrees;
        self->length = length;
    }
    return self;
}

/* tree_clone: return a new tree with a copy of the values of t. Memory is 
 * allocated for the new struct and its arrays and it is the user's 
 * responsibility to free it through tree_free(t). */
tree_t *tree_clone(tree_t *t)
{
    assert(t);
    tree_t *self = malloc(sizeof(tree_t));
    if (self != NULL) {
        self->nodes = calloc(t->length, sizeof(tree_node_t));
        self->degrees = calloc(t->length, sizeof(int));
        self->length = t->length;
        for (int i = 0; i < t->length; ++i) {
            self->nodes[i] = t->nodes[i];
            self->degrees[i] = t->degrees[i];
        }
    }
    return self;
}

/* tree_free: free the memory for the struct and all of its values. */
void tree_free(tree_t *t)
{
    free(t->nodes);
    free(t->degrees);
    free(t);
}

/* -------------------- Setters and Getters -------------------- */

tree_node_t tree_geti(tree_t *t, int index)
{
    return t->nodes[index];
}

int tree_geti_degree(tree_t *t, int index)
{
    return t->degrees[index];
}

tree_t *tree_seti(tree_t *t, int index, tree_node_t value, int degree)
{
    t->nodes[index] = value;
    t->degrees[index] = degree;
    return t;
}

int tree_random_index(tree_t *t)
{
    return rand() % t->length;
}

double average(double *values, int length)
{
    double acc = 0;
    for (int i = 0; i < length; ++i) {
        acc += values[i];
    }
    return acc/length;
}

/* tree_map: Evaluates fun at each node of the tree, given access to their
 * children. If do_average is nonzero, then tree_map returns
 * the average of all of the results. Otherwise, only the first result is returned.
 * See TAOCP 2.2.3, Algorithm F. */
double tree_map(tree_t *t,
             int (*fun)(int nvalues, tree_node_t *values, int values_top,
                      double *results, int results_top), int do_average)
{
    tree_node_t *stack = calloc(t->length, sizeof(tree_node_t));
    double *results_stack = calloc(t->length, sizeof(double));
    int values_top = 0;
    int results_top = 0;
    double result = 0;
    if (stack != NULL && results_stack != NULL) {
        for (int i = 0; i < t->length; ++i) {
            stack[values_top++] = t->nodes[i];
            results_top = fun(t->degrees[i]+1, stack, values_top, results_stack, results_top);
            values_top -= t->degrees[i];
            assert(values_top >= 0);
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

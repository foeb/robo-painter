#include "libtree.h"

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

void tree_print(tree_t *t)
{
    for (int i = 0; i < t->length; ++i) {
        printf("%4i", t->degrees[i]);
    }
    printf("\n");
    for (int i = 0; i < t->length; ++i) {
        printf("%4x", t->nodes[i]);
    }
    printf("\n");
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

/* tree_eval: Evaluates fun at each node of the tree, given access to their
 * children. Allocates memory for a new tree less than or equal to the size 
 * of t, however the degrees field will be filled with zeros.
 * See TAOCP 2.2.3, Algorithm F. */
/* FIXME  */
tree_t *tree_eval(tree_t *t, 
                    int (*fun)(tree_node_t *values, int nvalues, int stack_top, tree_node_t *results))
{
    tree_node_t *stack = calloc(t->length, sizeof(tree_node_t));
    tree_node_t *aux_stack = calloc(t->length, sizeof(tree_node_t));
    int stack_top = 0;
    if (stack != NULL && aux_stack != NULL) {
        for (int i = 0; i < t->length; ++i) {
            stack[stack_top++] = t->nodes[i];
            int result_len = fun(stack, t->degrees[i] + 1, stack_top, aux_stack);
            stack_top -= t->degrees[i] + 1;
            assert(stack_top >= 0);
            for (int j = 0; j < result_len; ++j) {
                stack[stack_top++] = aux_stack[j];
            }
        }
    }
    stack = realloc(stack, sizeof(tree_node_t) * stack_top);
    return tree_create(stack, calloc(stack_top, sizeof(int)), stack_top);
}

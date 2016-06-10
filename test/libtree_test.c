#include "minunit.h"
#include "../libtree.h"


char *test_tree_create_empty()
{
    tree_t *tree = tree_create(NULL, NULL, 0);
    mu_assert("The tree has zero length", 0 == tree->length);
    mu_assert("The tree has no nodes", NULL == tree->nodes);
    mu_assert("The tree has no degrees", NULL == tree->degrees);
    return NULL;
}

char *test_tree_create_nonempty()
{
    int length = 5;
    int degrees[5] = { 0, 0, 2, 0, 2 };
    tree_node_t nodes[5] = { 1, 2, 3, 4, 5 };
    tree_t *tree = tree_create(nodes, degrees, length);
    mu_assert("The tree has given length", length == tree->length);
    mu_assert("The tree has the same nodes", nodes == tree->nodes);
    mu_assert("The tree has the same degrees", degrees == tree->degrees);
    return NULL;
}

char *test_tree_clone()
{
    int length = 5;
    int degrees[5] = { 0, 0, 2, 0, 2 };
    tree_node_t nodes[5] = { 1, 2, 3, 4, 5 };
    tree_t *tree = tree_create(nodes, degrees, length);
    tree_t *new_tree = tree_clone(tree);
    mu_assert("The trees do not point to the same memory", tree != new_tree);
    mu_assert("The trees have the same length", tree->length == new_tree->length);
    mu_assert("The trees do not point to the same node array", 
                tree->nodes != new_tree->nodes);
    mu_assert("The trees do not point to the same degrees array", 
                tree->degrees != new_tree->degrees);
    for (int i = 0; i < length; ++i) {
        mu_assert("The trees have the same elements in their nodes array", 
                    tree->nodes[i] == new_tree->nodes[i]);
        mu_assert("The trees have the same elements in their degrees array", 
                    tree->degrees[i] == new_tree->degrees[i]);
    }
    tree_free(new_tree);
    return NULL;
}

int eval_test_id_fn(tree_node_t *values, int nvalues, int stack_top, tree_node_t *results)
{
    for (int i = stack_top - nvalues; i < stack_top; ++i) {
        results[i - stack_top + nvalues] = values[i];
    }
    return nvalues;
}

char *test_tree_eval_id()
{
    int length = 5;
    int degrees[5] = { 0, 0, 2, 0, 2 };
    tree_node_t nodes[5] = { 1, 2, 3, 4, 5 };
    tree_t *tree = tree_create(nodes, degrees, length);
    tree_t *new_tree = tree_eval(tree, eval_test_id_fn);
    for (int i = 0; i < length; ++i) {
        mu_assert("The trees have the same elements in their nodes array", 
                    tree->nodes[i] == new_tree->nodes[i]);
    }
    return NULL;
}

int eval_test_mul_fn(tree_node_t *values, int nvalues, int stack_top, tree_node_t *results)
{
    int acc = 1;
    for (int i = stack_top - nvalues; i < stack_top; ++i) {
        acc *= values[i];
    }
    results[0] =  acc;
    return 1;
}

char *test_tree_eval_mul()
{
    int length = 5;
    int degrees[5] = { 0, 0, 2, 0, 2 };
    tree_node_t nodes[5] = { 1, 2, 3, 4, 5 };
    tree_t *tree = tree_create(nodes, degrees, length);
    tree_t *new_tree = tree_eval(tree, eval_test_mul_fn);
    mu_assert("The first element of the nodes is the product of the nodes",
                new_tree->nodes[0] == 1*2*3*4*5);
    return NULL;
}

char *libtree_tests()
{
    --tests_run;
    mu_run_test(test_tree_create_empty);
    mu_run_test(test_tree_create_nonempty);
    mu_run_test(test_tree_clone);
    mu_run_test(test_tree_eval_id);
    mu_run_test(test_tree_eval_mul);
    return NULL;
}

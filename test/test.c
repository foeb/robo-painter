#include <stdio.h>
#include "minunit.h"

#include "libtree_test.c"

int tests_run = 0;

char *all_tests() {
    mu_run_test(libtree_tests);
    return NULL;
}

int main()
{
    char *result = all_tests();
    if (result != NULL) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != NULL;
}

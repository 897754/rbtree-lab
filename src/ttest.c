#include "rbtree.h"
#include "rbtree.c"

int main()
{
    rbtree *tree = new_rbtree();

    const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
    const size_t n = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < n; i++)
    {
        rbtree_insert(tree, arr[i]);
    }
    for (int i = 0; i < n; i++)
    {
        node_t *node = rbtree_find(tree, arr[i]);
        rbtree_erase(tree, node);
    }

    printf("%d", tree->root->key);

    return 0;
}
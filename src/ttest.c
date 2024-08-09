#include "rbtree.h"
#include "rbtree.c"

int main()
{
  rbtree * t = new_rbtree();

  rbtree_insert(t, 1);

  node_t * n = rbtree_find(t, 1);
  
}
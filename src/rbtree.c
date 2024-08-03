#include "rbtree.h"

#include <stdlib.h>

void Rotate(rbtree *tree, node_t *target);
void RotateLeft(rbtree *tree, node_t *target);
void RotateRight(rbtree *tree, node_t *target);
void InsertCheck(rbtree *tree, node_t *target);
void InsertCase1(rbtree *tree, node_t *target, node_t *uncle);
void InsertCase2(rbtree *tree, node_t *target);
void InsertCase3(rbtree *tree, node_t *target);
void DeleteRecursion(node_t *nil, node_t *target);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  p->nil = malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->key = 0;
  p->nil->left = NULL;
  p->nil->right = NULL;
  p->nil->parent = NULL;
  
  p->root = p->nil;
  return p;
}

void DeleteRecursion(node_t *nil, node_t *target)
{
  if(target == nil) return;
  DeleteRecursion(nil, target->left);
  DeleteRecursion(nil, target->right);
  free(target);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  DeleteRecursion(t->nil, t->root);
  free(t->nil);
  
  free(t);
}

void Rotate(rbtree *tree, node_t *target)
{
  if(target->parent == tree->nil)
    return;
  
  if(target->parent->key <= target->key)
    RotateLeft(tree, target);
  else
    RotateRight(tree, target);
}
void RotateLeft(rbtree *tree, node_t *target)
{
  node_t* a = target->parent->parent;
  node_t* c = target->left;
  
  node_t* parent = target->parent;

  if(tree->root == parent)
  {
    tree->root = target;
    target->color = RBTREE_BLACK;
  }
  
  parent->parent = target;
  target->left = parent;

  parent->right = c;
  if(a != tree->nil)
    c->parent = parent;
  
  target->parent = a;
  if(a != tree->nil)
  {
    if(a->key <= target->key)
    {
      a->right = target;
    } 
    else
    {
      a->left = target;
    }
  }
}
void RotateRight(rbtree *tree, node_t *target)
{
  node_t* a = target->parent->parent;
  node_t* c = target->right;
  
  node_t* parent = target->parent;
  
  if(tree->root == parent)
  {
    tree->root = target;
    target->color = RBTREE_BLACK;
  }
  
  parent->parent = target;
  target->right = parent;

  parent->left = c;
  c->parent = parent;
  
  target->parent = a;
  if(a != tree->nil)
  {
    if(a->key <= target->key)
    {
      a->left = target;
    } 
    else
    {
      a->right = target;
    }
  }
}

//관성드리프트
void InsertCase3(rbtree *tree, node_t *target)
{
  target->parent->parent->color = RBTREE_RED;
  target->parent->color = RBTREE_BLACK;
  Rotate(tree, target->parent);
}
//척추펴기
void InsertCase2(rbtree *tree, node_t *target)
{
  node_t *newTarget = target->parent;
  Rotate(tree, target);
  
  InsertCase3(tree, newTarget);
}
//엉클샘
void InsertCase1(rbtree *tree, node_t *target, node_t *uncle)
{
  node_t *grand = target->parent->parent;
  grand->color = RBTREE_RED;
  target->parent->color = RBTREE_BLACK;
  uncle->color = RBTREE_BLACK;
  if (tree->root == grand)
    grand->color = RBTREE_BLACK;
  if(grand->parent->color == RBTREE_RED)
    InsertCheck(tree, grand);
}

void InsertCheck(rbtree *tree, node_t *target)
{
  //FLAG : 자신이 오른쪽일 경우 1, 왼쪽 0
  int myFlag = (target->parent->key <= target->key) ? 1:0;
  int parentFlag = (target->parent->parent->key <= target->parent->key) ? 1:0;
  node_t *uncle = (parentFlag == 1 ? target->parent->parent->left : target->parent->parent->right);

  if(uncle->color == RBTREE_BLACK)
  {
    if(myFlag == parentFlag)
    {
      InsertCase3(tree, target);
    }
    else
    {
      InsertCase2(tree, target);
    }
  }
  else
  {
    InsertCase1(tree, target, uncle);
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  if(t->root == t->nil)
  {
    node_t *newNode = (node_t*)malloc(sizeof(node_t));
    t->root = newNode;
    newNode->key = key;
    newNode->color = RBTREE_BLACK;
    newNode->left = t->nil;
    newNode->right = t->nil;
    newNode->parent = t->nil;
    return newNode;
  }
  node_t *pre = t->root, *cur = t->root;
  int count = 0;
  //pre가 넣을 위치의 부모, cur가 넣을 위치가 될 때까지 탐색
  while (cur != t->nil)
  {
    count++;
    if(count > 10000)
      return NULL;
    if(cur->key <= key)
    {
      pre = cur;
      cur = cur->right;
    }
    else if(cur->key > key)
    {
      pre = cur;
      cur = cur->left;
    }
  }

  node_t *newNode = (node_t*)malloc(sizeof(node_t));
  if(pre->key <= key)
  {
    pre->right = newNode;
  }
  else
  {
    pre->left = newNode;
  }
  newNode->key = key;
  newNode->color = RBTREE_RED;
  newNode->left = t->nil;
  newNode->right = t->nil;
  newNode->parent = pre;
  //상위 노드가 빨간색일 경우
  if(pre->color == RBTREE_RED)
  {
    InsertCheck(t, newNode);
  }
  
  
  return newNode;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur = t->root;

  while (cur != t->nil)
  {
    if(cur->key < key)
      cur = cur->right;
    else if(cur->key > key)
      cur = cur->left;
    else
      return cur;
  }
  
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil)
  {
    return t->nil;
  }
  
  node_t *cur = t->root;
  while(cur->left != t->nil)
  {
    cur = cur->left;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil)
  {
    return t->nil;
  }
  
  node_t *cur = t->root;
  while(cur->right != t->nil)
  {
    cur = cur->right;
  }

  return cur;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

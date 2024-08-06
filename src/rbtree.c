#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

void Rotate(rbtree *tree, node_t *target);
void RotateLeft(rbtree *tree, node_t *target);
void RotateRight(rbtree *tree, node_t *target);
void InsertCheck(rbtree *tree, node_t *target);
void InsertCase1(rbtree *tree, node_t *target, node_t *uncle);
void InsertCase2(rbtree *tree, node_t *target);
void InsertCase3(rbtree *tree, node_t *target);
void RemoveCase1(rbtree *tree, node_t *parent, int isRight);
void RemoveCase2(rbtree *tree, node_t *parent, int isRight);
void RemoveCase3(rbtree *tree, node_t *parent, int isRight);
void RemoveCase4(rbtree *tree, node_t *parent, int isRight);
void ExtraBlack(rbtree *tree, node_t *parent, int isRight);
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
  
  if(target->parent->right == target)
    RotateLeft(tree, target);
  else
    RotateRight(tree, target);
}
void RotateLeft(rbtree *tree, node_t *target)
{
  node_t* a = target->parent->parent;
  node_t* c = target->left;
  node_t* parent = target->parent;

  if(parent == tree->root)
    tree->root = target;
  
  parent->parent = target;
  target->left = parent;

  parent->right = c;
  if(c != tree->nil)
    c->parent = parent;
  
  target->parent = a;
  if(a != tree->nil)
  {
    if(a->right == parent)
      a->right = target;
    else
      a->left = target;
  }
}
void RotateRight(rbtree *tree, node_t *target)
{
  node_t* a = target->parent->parent;
  node_t* c = target->right;
  node_t* parent = target->parent;
  
  if(parent == tree->root)
    tree->root = target;
  
  parent->parent = target;
  target->right = parent;

  parent->left = c;
  if(c != tree->nil)
    c->parent = parent;
  
  target->parent = a;
  if(a != tree->nil)
  {
    if(a->right == parent)
      a->right = target;
    else
      a->left = target;
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
  if(target->parent == tree->root)
  {
    target->parent->color = RBTREE_BLACK;
    return;
  }

  //FLAG : 자신이 오른쪽일 경우 1, 왼쪽 0
  int myFlag = (target->parent->right == target) ? 1:0;
  int parentFlag = (target->parent->parent->right == target->parent) ? 1:0;
  node_t *uncle = (parentFlag == 1 ? target->parent->parent->left : target->parent->parent->right);

  if(uncle->color == RBTREE_BLACK)
  {
    if(myFlag == parentFlag)
      InsertCase3(tree, target);
    else
      InsertCase2(tree, target);
  }
  else
    InsertCase1(tree, target, uncle);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  if(t == NULL) return NULL;

    node_t *newNode = (node_t*)malloc(sizeof(node_t));
    newNode->key = key;
    newNode->color = RBTREE_RED;
    newNode->left = t->nil;
    newNode->right = t->nil;
    newNode->parent = t->nil;

  // 트리가 비어있을 경우
  if(t->root == t->nil)
  {
    t->root = newNode;
    newNode->color = RBTREE_BLACK;
    return newNode;
  }
  node_t *pre = t->root, *cur = t->root;
  //pre가 넣을 위치의 부모, cur가 넣을 위치가 될 때까지 탐색
  while (cur != t->nil)
  {
    pre = cur;
    if(cur->key <= key)
      cur = cur->right;
    else if(cur->key > key)
      cur = cur->left;
  }

  if(pre->key <= key)
    pre->right = newNode;
  else
    pre->left = newNode;
  newNode->parent = pre;
  //상위 노드가 빨간색일 경우
  if(pre->color == RBTREE_RED)
    InsertCheck(t, newNode);
  
  
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
  if (t->root == t->nil)    return t->nil;
  
  node_t *cur = t->root;
  while(cur->left != t->nil)
  {
    cur = cur->left;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil)    return t->nil;
  
  node_t *cur = t->root;
  while(cur->right != t->nil)
  {
    cur = cur->right;
  }

  return cur;
}

void RemoveCase1(rbtree *tree, node_t *parent, int isRight)
{
  node_t* brother = (isRight == 1? parent->left:parent->right);
  brother->color = RBTREE_BLACK;
  parent->color = RBTREE_RED;
  Rotate(tree, brother);
  ExtraBlack(tree, parent, isRight);
}
void RemoveCase2(rbtree *tree, node_t *parent, int isRight)
{
  node_t* brother = (isRight == 1? parent->left:parent->right);
  brother->color = RBTREE_RED;
  if(parent->color == RBTREE_RED || parent == tree->root)
  {
    parent->color = RBTREE_BLACK;
    return;
  }
  isRight = (parent->parent->key <= parent->key ? 1:0);
  ExtraBlack(tree, parent->parent, isRight);
}
void RemoveCase3(rbtree *tree, node_t *parent, int isRight)
{
  node_t* brother = (isRight == 1? parent->left:parent->right);
  node_t *Nopposite = (isRight==0? brother->left : brother->right);
  brother->color = RBTREE_RED;
  Nopposite->color = RBTREE_BLACK;
  Rotate(tree, Nopposite);
  RemoveCase4(tree,parent,isRight);
}
void RemoveCase4(rbtree *tree, node_t *parent, int isRight)
{
  node_t* brother = (isRight == 1? parent->left:parent->right);
  node_t *opposite = (isRight==1? brother->left : brother->right);
  brother->color = parent->color;
  opposite->color = RBTREE_BLACK;
  parent->color = RBTREE_BLACK;
  Rotate(tree, brother);
}
void ExtraBlack(rbtree *tree, node_t *parent, int isRight)
{
  node_t* brother = (isRight == 1? parent->left : parent->right);
  if(brother->color == RBTREE_RED)
  {
    RemoveCase1(tree, parent, isRight);
    return;
  }
  node_t *opposite = (isRight==1? brother->left : brother->right);
  if(opposite->color == RBTREE_RED)
  {
    RemoveCase4(tree,parent,isRight);
    return;
  }
  if(brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK)
  {
    RemoveCase2(tree,parent,isRight);
    return;
  }
  RemoveCase3(tree,parent,isRight);
}

int rbtree_erase(rbtree *t, node_t *p) {
  if(t == NULL || t->root == t->nil || t->nil == p) return 0;

  node_t* parent = p->parent;


  //자식중 NIL 있음
  int isRight = 0;
  if(t->root != p)
   isRight = (parent->right == p ? 1:0);

  if(p->left == t->nil || p->right == t->nil)
  {
    node_t *child = (p->left == t->nil ? p->right:p->left);
    // 삭제할 노드가 루트일 경우
    if(t->root == p)
    {
      t->root = child;
    }
    else
    {
      if(isRight == 1)
        parent->right = child;
      else
        parent->left = child;

      if(child != t->nil)
        child->parent = parent;
    }


    if(parent != t->nil && p->color == RBTREE_BLACK)
    {
      //Double Black
      if(child->color == RBTREE_BLACK)
        ExtraBlack(t, parent, isRight);
      else
        child->color = RBTREE_BLACK;
    }
  }
  // 자식 둘다 NIL 아님. 후임자 찾기
  else
  {
    isRight = 1;
    node_t *pre = p, *cur = p->right;
    while (cur != t->nil)
    {
      isRight = 0;
      pre = cur;
      cur = cur->left;
    }
    //pre가 후임자.
    p->key = pre->key;
    return rbtree_erase(t, pre);
  }

  free(p);

  // TODO: implement erase
  return 1;
}


int SearchRec(node_t *n, node_t *nil, key_t *arr, size_t i)
{
  if(n == nil)
    return i;
  i = SearchRec(n->left, nil, arr, i);
  arr[i] = n->key;
  i++;
  i = SearchRec(n->right, nil, arr, i);
  return i;
}
//재귀
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  SearchRec(t->root, t->nil, arr, 0);

  return 0;
}

//벌목
// int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
//   // TODO: implement to_array
//   int i = 0;
//   while (t->root != t->nil)
//   {
//     node_t *cur = rbtree_min(t);
//     arr[i] = cur->key;
//     i++;
//     rbtree_erase((rbtree*)t,cur);
//   }
  

//   return 0;
// }
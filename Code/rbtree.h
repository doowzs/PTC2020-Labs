#ifndef RBTREE_H
#define RBTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * This Red-Black Tree implementation is manually adapted from
 * https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/
 * which was implemented in C++. The adapated version is an
 * abstract RB tree, requiring user providing a cmp function.
 */

enum RBColor {
  RED,
  BLACK
};

typedef struct RBNode {
  void *value;
  enum RBColor color;
  struct RBNode *left, *right, *parent;
} RBNode;

bool RBHasRedChild(RBNode *node);
bool RBIsLeftChild(RBNode *node);
RBNode *RBGetUncle(RBNode *node);
RBNode *RBGetSibling(RBNode *node);
RBNode *RBGetSuccessor(RBNode *node);
RBNode *RBGetReplacement(RBNode *node);
void RBSwapColors(RBNode *n1, RBNode *n2);
void RBSwapValues(RBNode *n1, RBNode *n2);

void RBMoveDown(RBNode *node, RBNode *newParent);
void RBRotateLeft(RBNode **root, RBNode *node);
void RBRotateRight(RBNode **root, RBNode *node);
void RBFixRedRed(RBNode **root, RBNode *node);
void RBFixBlackBlack(RBNode **root, RBNode *node);
void RBDeleteNode(RBNode **root, RBNode *node);

void RBInsert(RBNode **root, void *value, int(*cmp)(const void *, const void *));
bool RBContains(RBNode **root, void *value, int(*cmp)(const void *, const void *));
RBNode *RBSearch(RBNode **root, void *value, int (*cmp)(const void *, const void *));
void RBDelete(RBNode **root, void *value, int (*cmp)(const void *, const void *));
void RBDestroy(RBNode **root, void (*destroy)(void *));


#endif // RBTREE_H

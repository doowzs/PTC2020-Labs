/**
 * The syntax tree structure.
 * Copyright, Tianyun Zhang @ Nanjing University. 2020-03-07
 * */

#ifndef TREE_H
#define TREE_H

#define STDEBUG false // <- syntax tree debugger switch

#include <stdbool.h>
#include "token.h"

struct IRCode *code;

typedef struct STNode {
  int line, column;
  int token, symbol;
  const char* name;
  bool empty;
  union {
    unsigned int    ival;
    float           fval;
    enum ENUM_RELOP rval;
    char            sval[64];
  };
  struct {
    struct IRCode *head, *tail;
  } ir;
  struct STNode *child, *next;
} STNode;

extern STNode *stroot;

void printSyntaxTree();
void printSyntaxTreeAux(STNode *node, int indent);
void teardownSyntaxTree(STNode *node);

#endif


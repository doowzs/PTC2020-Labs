/**
 * The intermediate representation.
 * Copyright, Tianyun Zhang, 2020/04/29.
 * */

#ifndef IR_H
#define IR_H

#include <stdio.h>
#include <stdbool.h>
#include "token.h"
#include "rbtree.h"

#define IRDebug false // <- debug switch
#if IRDebug
#define DEBUG
#endif

struct STNode;
struct SEType;
struct SEField;

enum IROperandType {
  IR_OP_NULL,
  IR_OP_TEMP,
  IR_OP_LABEL,
  IR_OP_VARIABLE,
  IR_OP_VADDRESS,
  IR_OP_MEMBLOCK,
  IR_OP_CONSTANT,
  IR_OP_RELOP,
  IR_OP_FUNCTION,
};

enum IRCodeType {
  IR_CODE_LABEL,
  IR_CODE_FUNCTION,
  IR_CODE_ASSIGN,
  IR_CODE_ADD,
  IR_CODE_SUB,
  IR_CODE_MUL,
  IR_CODE_DIV,
  IR_CODE_LOAD,
  IR_CODE_SAVE,
  IR_CODE_JUMP,
  IR_CODE_JUMP_COND,
  IR_CODE_RETURN,
  IR_CODE_DEC,
  IR_CODE_ARG,
  IR_CODE_CALL,
  IR_CODE_PARAM,
  IR_CODE_READ,
  IR_CODE_WRITE,
};

typedef struct IROperand {
  enum IROperandType kind;
  size_t size; // size of memory used by operand
  size_t offset; // offset in memory from $fp
  union {
    unsigned int number;
    int ivalue;
    float fvalue;
    const char *name;
    unsigned int address;
    enum ENUM_RELOP relop;
  };
} IROperand;

typedef struct IRCode {
  enum IRCodeType kind;
  union {
    struct {
      struct IROperand label;
    } label;
    struct {
      struct IROperand right, left;
    } assign, addr, load, save;
    struct {
      struct IROperand result, op1, op2;
    } binop, addr_offset;
    struct {
      struct IROperand dest;
    } jump;
    struct {
      struct IROperand op1, relop, op2, dest;
    } jump_cond;
    struct {
      struct IROperand value;
    } ret;
    struct {
      struct IROperand result, function;
    } call;
    struct {
      struct IROperand variable, size;
    } dec;
    struct {
      struct IROperand variable;
    } fence, read, write, arg, param;
    struct {
      struct IROperand function;
      struct RBNode *root; // RB tree of local variables
    } function;
  };
  struct IRCode *prev, *next;
  struct IRCode *parent; // used in asm.c
} IRCode;

typedef struct IRCodeList {
  struct IRCode *head, *tail;
} IRCodeList;

// List+Type+Addr, for Exp only
typedef struct IRCodePair {
  struct IRCodeList list;
  struct SEType *type;
  bool addr;
} IRCodePair;

struct IRCodePair IRTranslateExp(struct STNode *exp, struct IROperand place, bool deref);
struct IRCodeList IRTranslateCondPre(struct STNode *exp, struct IROperand place);
struct IRCodeList IRTranslateCond(struct STNode *exp, struct IROperand label_true, struct IROperand label_false);
struct IRCodeList IRTranslateCompSt(struct STNode *comp);
struct IRCodeList IRTranslateDefList(struct STNode *list);
struct IRCodeList IRTranslateDef(struct STNode *def);
struct IRCodeList IRTranslateDecList(struct STNode *list);
struct IRCodeList IRTranslateDec(struct STNode *dec);
struct IRCodeList IRTranslateVarDec(struct STNode *var);
struct IRCodeList IRTranslateStmtList(struct STNode *list);
struct IRCodeList IRTranslateStmt(struct STNode *stmt);
struct IRCodeList IRTranslateArgs(struct STNode *args, struct SEField *field, struct IRCodeList *arg_list);

// This function is unique, it operates on the global variable irlist.
void IRTranslateFunc(const char *name, struct STNode *comp);

struct IROperand IRNewNullOperand();
struct IROperand IRNewTempOperand();
struct IROperand IRNewLabelOperand();
struct IROperand IRNewVariableOperand(const char *name);
struct IROperand IRNewConstantOperand(int value);
struct IROperand IRNewRelopOperand(enum ENUM_RELOP relop);
struct IROperand IRNewFunctionOperand(const char *name);

size_t IRParseOperand(char *s, IROperand *op);
size_t IRParseCode(char *s, IRCode *code);
size_t IRWriteCode(FILE *f, IRCode *code);

struct IRCode *IRNewCode(enum IRCodeType kind);
struct IRCodeList IRWrapCode(struct IRCode *code);
struct IRCodePair IRWrapPair(struct IRCodeList list, struct SEType *type, bool addr);
struct IRCodeList IRAppendCode(struct IRCodeList list, struct IRCode *code);
struct IRCodeList IRRemoveCode(struct IRCodeList list, struct IRCode *code);
struct IRCodeList IRConcatLists(struct IRCodeList list1, struct IRCodeList list2);
void IRDestroyList(struct IRCodeList list);

#endif

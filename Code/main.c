#include "asm.h"
#include "ir.h"
#include "opt.h"
#include "semantics.h"
#include "tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

extern void yyrestart(FILE *);
extern int yyparse_wrap(); // defined in syntax.y

int errLineno = 0;
bool hasErrorA = false;
bool hasErrorB = false;
bool hasErrorS = false;
STNode *stroot = NULL;
IRCodeList irlist = {NULL, NULL};

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    fprintf(stderr, "Usage: parser source_file output_file\n");
    return 1;
  }
  FILE *fin = fopen(argv[1], "r");
  if (fin == NULL) {
    perror(argv[1]);
    return 2;
  }
  FILE *fout = fopen(argv[2], "w+");
  if (fout == NULL) {
    perror(argv[2]);
    return 2;
  }

  // Step 1: call yyparse to get syntax tree.
  yyrestart(fin);
  yyparse_wrap();
  if (hasErrorA || hasErrorB) {
    return 3;
  }
  // printSyntaxTree();

  // Step 2: conduct a full semantic scan.
  // Step 3: translate to IR during the scan.
  semanticScan();
  if (hasErrorS) {
    return 4;
  }

  // Step 4: do IR optimization.
  optimize();
  //for (IRCode *code = irlist.head; code != NULL; code = code->next) {
  //  IRWriteCode(fout, code);
  //}

  // Step 5: translate to ASM and output.
  assemble(fout);

  // do not teardown until all work is done!
  teardownSyntaxTree(stroot);
  IRDestroyList(irlist);

  return 0;
}

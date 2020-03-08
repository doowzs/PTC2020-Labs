%{
  /* Copyright, Tianyun Zhang @ Nanjing University. 2020-03-07 */
  #define YYDEBUG true // <- parser debugger switch
%}

%{
  #include <stdio.h>
  #include "lex.yy.c"
  void yyerror(char *);
%}

%token TYPE INT FLOAT ID
%token ASSIGNOP RELOP
%token PLUS MINUS STAR DIV
%token AND OR DOT NOT
%token SEMI COMMA
%token LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE

%%
Term : INT | FLOAT;

%%
void yyerror(char *msg) {
  fprintf(stderr, "yyerror: %s\n", msg);
}
int _warp_yyparse() {
#if YYDEBUG
  yydebug = 1;
#endif
  return yyparse();
}
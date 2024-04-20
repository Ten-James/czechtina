#ifndef CZECHTINA_czechtina__parser
#define CZECHTINA_czechtina__parser
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__lexer__token.c"
#include "czechtina__parser__ast.c"
#include "czechtina__parser__programAST.c"
#include "czechtina__compiler__enums.c"
#include "czechtina__compiler__strcts.c"
#include "czechtina__compiler__functions.c"
#include "czechtina__compiler__types.c"
#include "czechtina__utils__printer.c"
#include "czechtina__utils__error.c"
#include "std.c"
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _PARSER PARSER;
typedef struct _PARSER {
TOKEN** tokens;
char* text;
char* filePath;
int pos;
int max;
} PARSER;
Error* CZ_PARSER_generateError(PARSER* this);
bool CZ_PARSER_isNext(PARSER* this, int type);
bool CZ_PARSER_isNextOneOf(PARSER* this, int* types, int size);
bool CZ_PARSER_hasInvalidTokens(PARSER* this);
NODE* CZ_PARSER_type(PARSER* this);
NODE* CZ_PARSER_leftSingle(PARSER* this);
NODE* CZ_PARSER_singleExp(PARSER* this);
NODE* CZ_PARSER_unaryExp(PARSER* this, bool negative);
NODE* CZ_PARSER_retypedUnary(PARSER* this, bool negative);
NODE* CZ_PARSER_factor(PARSER* this, bool negative);
NODE* CZ_PARSER_terms(PARSER* this, bool negative);
NODE* CZ_PARSER_comparation(PARSER* this, bool negative);
NODE* CZ_PARSER_expr(PARSER* this, bool negative);
NODE* CZ_PARSER_expression(PARSER* this);
NODE* CZ_PARSER_varDefinition(PARSER* this);
int CZ_PARSER_isNextAssignment(PARSER* this);
NODE* CZ_PARSER_NewNode(PARSER* this);
NODE* CZ_PARSER_assignment(PARSER* this);
NODE* CZ_PARSER_unaryAssignment(PARSER* this);
NODE* CZ_PARSER_innerlines(PARSER* this);
NODE* CZ_PARSER_lines(PARSER* this);
NODE* CZ_PARSER_blockLines(PARSER* this);
NODE* CZ_PARSER_blockOrIterExpr(PARSER* this);
NODE* CZ_PARSER_range(PARSER* this);
NODE* CZ_PARSER_rangedFor(PARSER* this);
NODE* CZ_PARSER_ifStatement(PARSER* this);
NODE* CZ_PARSER_whileStatement(PARSER* this);
NODE* CZ_PARSER_flow(PARSER* this);
void CZ_PARSER_consumeFunctionDecorators(PARSER* this, FunctionNode* func);
FunctionNode* CZ_PARSER_function(PARSER* this);
STR* CZ_PARSER_parsePackageName(PARSER* this);
ENUM* CZ_PARSER_enm(PARSER* this);
STRCTENTRY* CZ_PARSER_strctDefs(PARSER* this);
PROGRAMAST* CZ_PARSER_parseProgram(PARSER* this);
PROGRAMAST* CZ_PARSER_parse(PARSER* this);
Error* CZ_PARSER_generateError(PARSER* this) {
Error* err=malloc(sizeof(Error));
CZ_Error_init(err);
err->level=2;
err->pos=this->pos;
err->text=this->text;
err->tokens=this->tokens;
err->filePath=this->filePath;
CZ_STR_appendPtr(err->message, "Code cloudn't be parsed");
CZ_STR_appendPtr(err->secMessage, "syntax error: ");
;
return err;
;
}
bool CZ_PARSER_isNext(PARSER* this, int type) {
if (this->pos>=this->max) {
;
return false;
;
};
return this->tokens[this->pos]->type == type;
;
}
bool CZ_PARSER_isNextOneOf(PARSER* this, int* types, int size) {
if (this->pos>=this->max) {
;
return false;
;
};
TOKEN** tokens=this->tokens;
for(int i = 0; i < size; i++) {
if (tokens[this->pos]->type==types[i]) {
;
return true;
;
};
;
};
;
return false;
;
}
bool CZ_PARSER_hasInvalidTokens(PARSER* this) {
fprintf(czStdOut,"pos: "); fprintf(czStdOut,"%d", this->pos); fprintf(czStdOut," max: "); fprintf(czStdOut,"%d", this->max); fputs("\n",czStdOut);
;
return this->pos<this->max;
;
}
NODE* CZ_PARSER_type(PARSER* this) {
debugPrint("type");
int cur=this->pos;
if (CZ_PARSER_isNext(this, 5)) {
this->pos+=1;
NODE* tryType=CZ_PARSER_type(this);
if (tryType==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing type after MemMove type");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:75:52\n");fprintf(stderr,"syntax error in dynamic type");exit(1);
//somehow destruct: err 
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=3;
CZ_NODE_consumeUnary(node, tryType, 3);
;
return node;
;
};
if (CZ_PARSER_isNext(this, 6)) {
this->pos+=1;
NODE* tryType=CZ_PARSER_type(this);
if (tryType==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing type after MemMove type");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:92:52\n");fprintf(stderr,"syntax error in dynamic type");exit(1);
//somehow destruct: err 
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=3;
CZ_NODE_consumeUnary(node, tryType, 4);
;
return node;
;
};
if (CZ_PARSER_isNext(this, 63)) {
this->pos+=1;
if ((CZ_PARSER_isNext(this, 43))==false) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing < in pointer type");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:110:52\n");fprintf(stderr,"syntax error in pointer type");exit(1);
//somehow destruct: err 
;
};
this->pos+=1;
NODE* tryType=CZ_PARSER_type(this);
if (tryType==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing type in pointer type");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:118:52\n");fprintf(stderr,"syntax error in pointer type");exit(1);
//somehow destruct: err 
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=3;
CZ_NODE_consumeUnary(node, tryType, 0);
if ((CZ_PARSER_isNext(this, 44))==false) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing > in pointer type");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:130:52\n");fprintf(stderr,"syntax error in pointer type");exit(1);
//somehow destruct: err 
;
};
this->pos+=1;
;
return node;
;
}
else if ((CZ_PARSER_isNext(this, 58))||(CZ_PARSER_isNext(this, 59))||(CZ_PARSER_isNext(this, 60))||(CZ_PARSER_isNext(this, 62))||(CZ_PARSER_isNext(this, 61))) {
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=3;
node->tokensStart=cur;
node->tokensEnd=cur;
this->pos+=1;
;
return node;
;
}
else if (CZ_PARSER_isNext(this, 27)) {
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=3;
node->tokensStart=cur;
node->tokensEnd=cur;
this->pos+=1;
;
return node;
;
};
this->pos=cur;
;
return 0;
;
}
NODE* CZ_PARSER_leftSingle(PARSER* this) {
debugPrint("left_single");
int cur=this->pos;
if (CZ_PARSER_isNext(this, 27)) {
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=19;
node->tokensStart=cur;
node->tokensEnd=cur;
this->pos+=1;
while (1) {
int secCur=this->pos;
if (CZ_PARSER_isNext(this, 41)) {
this->pos+=1;
NODE* tryExp=CZ_PARSER_expression(this);
if (tryExp==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing expression in array access");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:184:60\n");fprintf(stderr,"syntax error in array access");exit(1);
//somehow destruct: err 
;
};
if ((CZ_PARSER_isNext(this, 42))==false) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing ] in array access");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:190:60\n");fprintf(stderr,"syntax error in array access");exit(1);
//somehow destruct: err 
;
};
this->pos+=1;
NODE* oldNode=node;
node=malloc(sizeof(NODE));
node->tokensStart=oldNode->tokensStart;
node->tokensEnd=this->pos-1;
node->type=2;
CZ_NODE_consumeBinary(node, oldNode, tryExp, 0);
;
}
else if (CZ_PARSER_isNext(this, 38)) {
this->pos+=1;
if ((CZ_PARSER_isNext(this, 27))==false) {
this->pos=secCur;
;
return node;
;
};
NODE* secNode=malloc(sizeof(NODE));
CZ_NODE_init(secNode);
secNode->type=19;
secNode->tokensStart=this->pos;
secNode->tokensEnd=this->pos;
this->pos+=1;
NODE* oldNode=node;
node=malloc(sizeof(NODE));
node->tokensStart=oldNode->tokensStart;
node->tokensEnd=this->pos-1;
node->type=1;
CZ_NODE_consumeBinary(node, oldNode, secNode, 0);
;
}
else {
;
return node;
;
};
;
};
//somehow destruct: node 
;
};
this->pos=cur;
;
return 0;
;
}
NODE* CZ_PARSER_singleExp(PARSER* this) {
debugPrint("singleExp");
int cur=this->pos;
if (CZ_PARSER_isNext(this, 28)) {
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=18;
node->tokensStart=cur;
node->tokensEnd=cur;
this->pos+=1;
;
return node;
;
};
NODE* tryLeft=CZ_PARSER_leftSingle(this);
if (tryLeft!=0) {
;
return tryLeft;
;
};
//somehow destruct: tryLeft 
;
return 0;
//somehow destruct: tryLeft 
;
}
NODE* CZ_PARSER_unaryExp(PARSER* this, bool negative) {
debugPrint("unaryExp");
int cur=this->pos;
if (negative) {
if (CZ_PARSER_isNext(this, 2)) {
this->pos+=1;
NODE* tryUnary=CZ_PARSER_unaryExp(this, negative);
if (tryUnary==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing expression after -");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:268:63\n");fprintf(stderr,"syntax error in negative expression");exit(1);
//somehow destruct: err 
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=17;
CZ_NODE_consumeUnary(node, tryUnary, 15);
debugPrint("unaryExp negative found");
;
return node;
;
};
;
};
if (CZ_PARSER_isNext(this, 9)) {
this->pos+=1;
NODE* tryUnary=CZ_PARSER_unaryExp(this, negative);
if (tryUnary==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing expression in not");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:287:54\n");fprintf(stderr,"syntax error in not expression");exit(1);
//somehow destruct: err 
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=17;
CZ_NODE_consumeUnary(node, tryUnary, 1);
debugPrint("unaryExp found");
;
return node;
;
};
if (CZ_PARSER_isNext(this, 24)) {
this->pos+=1;
NODE* trySingle=CZ_PARSER_singleExp(this);
if (trySingle==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing expression in call");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:306:55\n");fprintf(stderr,"syntax error in call expression");exit(1);
//somehow destruct: err 
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=21;
CZ_NODE_consumeUnary(node, trySingle, 0);
debugPrint("unaryExp found");
;
return node;
;
};
if (CZ_PARSER_isNext(this, 33)) {
this->pos+=1;
NODE* tryExp=CZ_PARSER_expression(this);
if (tryExp==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing expression in brackets");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:324:59\n");fprintf(stderr,"syntax error in brackets expression");exit(1);
//somehow destruct: err 
;
};
if ((CZ_PARSER_isNext(this, 34))==false) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing ) in brackets");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:330:59\n");fprintf(stderr,"syntax error in brackets expression");exit(1);
//somehow destruct: err 
;
};
this->pos+=1;
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=17;
CZ_NODE_consumeUnary(node, tryExp, 5);
;
return node;
;
};
if (CZ_PARSER_isNext(this, 5)) {
this->pos+=1;
if ((CZ_PARSER_isNext(this, 27))==false) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing identifier after &");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:347:60\n");fprintf(stderr,"syntax error in reference expression");exit(1);
//somehow destruct: err 
;
};
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=19;
node->tokensStart=this->pos;
node->tokensEnd=this->pos;
NODE* node2=malloc(sizeof(NODE));
CZ_NODE_init(node2);
node2->type=17;
node2->tokensStart=this->pos-1;
node2->tokensEnd=this->pos;
this->pos+=1;
CZ_NODE_consumeUnary(node2, node, 3);
;
return node2;
;
};
if (CZ_PARSER_isNext(this, 29)) {
debugPrint("number found");
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=18;
node->tokensStart=cur;
node->tokensEnd=cur;
this->pos+=1;
;
return node;
;
};
if (CZ_PARSER_isNext(this, 30)) {
debugPrint("character found");
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=18;
node->tokensStart=cur;
node->tokensEnd=cur;
this->pos+=1;
;
return node;
;
};
if (CZ_PARSER_isNext(this, 31)) {
debugPrint("bool true found");
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=18;
node->tokensStart=cur;
node->tokensEnd=cur;
this->pos+=1;
;
return node;
;
};
if (CZ_PARSER_isNext(this, 32)) {
debugPrint("bool false found");
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=18;
node->tokensStart=cur;
node->tokensEnd=cur;
this->pos+=1;
;
return node;
;
};
NODE* trySingle=CZ_PARSER_singleExp(this);
if (trySingle!=0) {
debugPrint("singleExp found");
NODE* tryExp=CZ_PARSER_expr(this, negative);
if (tryExp==0) {
//somehow destruct: tryExp 
;
return trySingle;
;
};
debugPrint("function call found");
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->tokensStart=cur;
node->type=21;
CZ_NODE_createArr(node);
CZ_NODE_add(node, trySingle);
CZ_NODE_add(node, tryExp);
while (1) {
int secCur=this->pos;
if ((CZ_PARSER_isNext(this, 37))==false) {
;
return node;
;
};
debugPrint("found comma");
this->pos+=1;
NODE* tryExp=CZ_PARSER_expr(this, negative);
if (tryExp==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing expression in function call");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:444:57\n");fprintf(stderr,"syntax error in function call");exit(1);
//somehow destruct: err 
;
};
CZ_NODE_add(node, tryExp);
;
};
fprintf(stderr,"src/parser/parser.cz:449:44\n");fprintf(stderr,"this should never happen");exit(1);
//somehow destruct: node 
;
};
this->pos=cur;
//somehow destruct: trySingle 
;
return 0;
//somehow destruct: trySingle 
;
}
NODE* CZ_PARSER_retypedUnary(PARSER* this, bool negative) {
debugPrint("retypedUnary");
int cur=this->pos;
NODE* tryUnary=CZ_PARSER_unaryExp(this, negative);
if (tryUnary!=0) {
if (CZ_PARSER_isNext(this, 23)) {
this->pos+=1;
NODE* tryType=CZ_PARSER_type(this);
if (tryType==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing type after as");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:471:48\n");fprintf(stderr,"syntax error in cast");exit(1);
//somehow destruct: err 
;
};
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=22;
CZ_NODE_consumeBinary(node, tryUnary, tryType, 0);
;
return node;
;
};
;
return tryUnary;
;
};
//somehow destruct: tryUnary 
;
return 0;
//somehow destruct: tryUnary 
;
}
NODE* CZ_PARSER_factor(PARSER* this, bool negative) {
debugPrint("factor");
int cur=this->pos;
NODE* tryUnary=CZ_PARSER_retypedUnary(this, negative);
if (tryUnary==0) {
this->pos=cur;
//somehow destruct: tryUnary 
;
return 0;
;
};
int operand=0;
if (CZ_PARSER_isNext(this, 7)) {
operand=17;
;
}
else if (CZ_PARSER_isNext(this, 8)) {
operand=18;
;
}
else if (CZ_PARSER_isNext(this, 4)) {
operand=19;
;
}
else {
;
return tryUnary;
;
};
this->pos+=1;
NODE* trySecUnary=CZ_PARSER_retypedUnary(this, negative);
if (trySecUnary==0) {
this->pos=cur;
free(tryUnary);
//somehow destruct: trySecUnary tryUnary 
;
return 0;
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->type=16;
CZ_NODE_consumeBinary(node, tryUnary, trySecUnary, operand);
debugPrint("factor found");
while (1) {
debugPrint("factor loop");
int secCur=this->pos;
if (CZ_PARSER_isNext(this, 7)) {
operand=17;
;
}
else if (CZ_PARSER_isNext(this, 8)) {
operand=18;
;
}
else if (CZ_PARSER_isNext(this, 4)) {
operand=19;
;
}
else {
this->pos=secCur;
node->tokensEnd=secCur-1;
;
return node;
;
};
this->pos+=1;
trySecUnary=CZ_PARSER_retypedUnary(this, negative);
if (trySecUnary==0) {
this->pos=secCur;
node->tokensEnd=secCur-1;
;
return node;
;
};
NODE* oldNode=node;
node=malloc(sizeof(NODE));
oldNode->tokensEnd=secCur-1;
node->tokensStart=oldNode->tokensStart;
node->type=16;
CZ_NODE_consumeBinary(node, oldNode, trySecUnary, operand);
;
};
fprintf(stderr,"src/parser/parser.cz:564:40\n");fprintf(stderr,"this should never happen");exit(1);
this->pos=cur;
//somehow destruct: node 
;
return 0;
//somehow destruct: node 
;
}
NODE* CZ_PARSER_terms(PARSER* this, bool negative) {
debugPrint("terms");
int cur=this->pos;
NODE* tryFactor=CZ_PARSER_factor(this, negative);
if (tryFactor==0) {
this->pos=cur;
//somehow destruct: tryFactor 
;
return 0;
;
};
int operand=0;
if (CZ_PARSER_isNext(this, 0)) {
operand=13;
;
}
else if (CZ_PARSER_isNext(this, 2)) {
operand=15;
;
}
else {
;
return tryFactor;
;
};
this->pos+=1;
NODE* trySecFactor=CZ_PARSER_factor(this, negative);
if (trySecFactor==0) {
this->pos=cur;
free(tryFactor);
//somehow destruct: trySecFactor tryFactor 
;
return 0;
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->type=15;
CZ_NODE_consumeBinary(node, tryFactor, trySecFactor, operand);
debugPrint("terms found");
while (1) {
debugPrint("terms loop");
int secCur=this->pos;
if (CZ_PARSER_isNext(this, 0)) {
operand=13;
;
}
else if (CZ_PARSER_isNext(this, 2)) {
operand=15;
;
}
else {
this->pos=secCur;
node->tokensEnd=secCur-1;
;
return node;
;
};
this->pos+=1;
trySecFactor=CZ_PARSER_factor(this, negative);
if (trySecFactor==0) {
this->pos=secCur;
node->tokensEnd=secCur-1;
;
return node;
;
};
NODE* oldNode=node;
node=malloc(sizeof(NODE));
oldNode->tokensEnd=secCur-1;
node->tokensStart=oldNode->tokensStart;
node->type=15;
CZ_NODE_consumeBinary(node, oldNode, trySecFactor, operand);
;
};
fprintf(stderr,"src/parser/parser.cz:639:40\n");fprintf(stderr,"this should never happen");exit(1);
this->pos=cur;
//somehow destruct: node 
;
return 0;
//somehow destruct: node 
;
}
NODE* CZ_PARSER_comparation(PARSER* this, bool negative) {
debugPrint("comparation");
int cur=this->pos;
NODE* tryTerms=CZ_PARSER_terms(this, negative);
if (tryTerms==0) {
this->pos=cur;
//somehow destruct: tryTerms 
;
return 0;
;
};
int operand=0;
if (CZ_PARSER_isNext(this, 47)) {
operand=7;
;
}
else if (CZ_PARSER_isNext(this, 48)) {
operand=8;
;
}
else if (CZ_PARSER_isNext(this, 43)) {
operand=9;
;
}
else if (CZ_PARSER_isNext(this, 45)) {
operand=11;
;
}
else if (CZ_PARSER_isNext(this, 44)) {
operand=10;
;
}
else if (CZ_PARSER_isNext(this, 46)) {
operand=12;
;
}
else {
//somehow destruct: tryTerms 
;
return tryTerms;
;
};
this->pos+=1;
NODE* trySecTerms=CZ_PARSER_terms(this, negative);
if (trySecTerms==0) {
this->pos=cur;
free(tryTerms);
//somehow destruct: trySecTerms tryTerms 
;
return 0;
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->type=12;
CZ_NODE_consumeBinary(node, tryTerms, trySecTerms, operand);
debugPrint("comparation found");
while (1) {
debugPrint("comparation loop");
int secCur=this->pos;
if (CZ_PARSER_isNext(this, 47)) {
operand=7;
;
}
else if (CZ_PARSER_isNext(this, 48)) {
operand=8;
;
}
else if (CZ_PARSER_isNext(this, 43)) {
operand=9;
;
}
else if (CZ_PARSER_isNext(this, 45)) {
operand=11;
;
}
else if (CZ_PARSER_isNext(this, 44)) {
operand=10;
;
}
else if (CZ_PARSER_isNext(this, 46)) {
operand=12;
;
}
else {
this->pos=secCur;
node->tokensEnd=secCur-1;
;
return node;
;
};
this->pos+=1;
trySecTerms=CZ_PARSER_terms(this, negative);
if (trySecTerms==0) {
this->pos=secCur;
node->tokensEnd=secCur;
;
return node;
;
};
NODE* oldNode=node;
node=malloc(sizeof(NODE));
oldNode->tokensEnd=secCur-1;
node->tokensStart=oldNode->tokensStart;
node->type=12;
CZ_NODE_consumeBinary(node, oldNode, trySecTerms, operand);
;
};
fprintf(stderr,"src/parser/parser.cz:740:40\n");fprintf(stderr,"this should never happen");exit(1);
//somehow destruct: node 
;
}
NODE* CZ_PARSER_expr(PARSER* this, bool negative) {
debugPrint("expression");
int cur=this->pos;
NODE* tryComparation=CZ_PARSER_comparation(this, negative);
if (tryComparation==0) {
tryComparation=CZ_PARSER_comparation(this, negative);
;
};
if (tryComparation==0) {
this->pos=cur;
//somehow destruct: tryComparation 
;
return 0;
;
};
int operand=0;
if (CZ_PARSER_isNext(this, 49)) {
operand=27;
;
}
else if (CZ_PARSER_isNext(this, 50)) {
operand=28;
;
}
else {
//somehow destruct: tryComparation 
;
return tryComparation;
;
};
this->pos+=1;
NODE* trySecComparation=CZ_PARSER_comparation(this, negative);
if (trySecComparation==0) {
trySecComparation=CZ_PARSER_comparation(this, negative);
;
};
if (trySecComparation==0) {
this->pos=cur;
free(tryComparation);
//somehow destruct: trySecComparation tryComparation 
;
return 0;
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->type=11;
CZ_NODE_consumeBinary(node, tryComparation, trySecComparation, operand);
debugPrint("expression found");
while (1) {
debugPrint("expression loop");
int secCur=this->pos;
if (CZ_PARSER_isNext(this, 49)) {
operand=27;
;
}
else if (CZ_PARSER_isNext(this, 50)) {
operand=28;
;
}
else {
this->pos=secCur;
node->tokensEnd=secCur-1;
;
return node;
;
};
this->pos+=1;
trySecComparation=CZ_PARSER_comparation(this, false);
if (trySecComparation==0) {
trySecComparation=CZ_PARSER_comparation(this, true);
;
};
if (trySecComparation==0) {
this->pos=secCur;
node->tokensEnd=secCur-1;
;
return node;
;
};
NODE* oldNode=node;
node=malloc(sizeof(NODE));
oldNode->tokensEnd=secCur-1;
node->tokensStart=oldNode->tokensStart;
node->type=11;
CZ_NODE_consumeBinary(node, oldNode, trySecComparation, operand);
;
};
fprintf(stderr,"src/parser/parser.cz:827:40\n");fprintf(stderr,"this should never happen");exit(1);
//somehow destruct: node 
;
}
NODE* CZ_PARSER_expression(PARSER* this) {
NODE* tryExp=CZ_PARSER_expr(this, false);
if (tryExp==0) {
NODE* tryExp=CZ_PARSER_expr(this, true);
;
};
if (tryExp==0) {
//somehow destruct: tryExp 
;
return 0;
;
};
//somehow destruct: tryExp 
;
return tryExp;
//somehow destruct: tryExp 
;
}
NODE* CZ_PARSER_varDefinition(PARSER* this) {
debugPrint("varDefinition");
int cur=this->pos;
if ((CZ_PARSER_isNext(this, 27))==false) {
;
return 0;
;
};
NODE* idenNode=malloc(sizeof(NODE));
CZ_NODE_init(idenNode);
idenNode->type=19;
idenNode->tokensStart=this->pos;
idenNode->tokensEnd=this->pos;
this->pos+=1;
if ((CZ_PARSER_isNext(this, 39))==false) {
this->pos=cur;
//somehow destruct: idenNode 
;
return 0;
;
};
this->pos+=1;
NODE* tryType=CZ_PARSER_type(this);
if (tryType==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing type in variable declaration");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:870:56\n");fprintf(stderr,"syntax error in variable declaration");exit(1);
//somehow destruct: err 
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=20;
CZ_NODE_consumeBinary(node, idenNode, tryType, 0);
;
return node;
;
}
int CZ_PARSER_isNextAssignment(PARSER* this) {
if (CZ_PARSER_isNext(this, 52)) {
;
return 20;
;
};
if (CZ_PARSER_isNext(this, 53)) {
;
return 21;
;
};
if (CZ_PARSER_isNext(this, 54)) {
;
return 23;
;
};
if (CZ_PARSER_isNext(this, 55)) {
;
return 26;
;
};
if (CZ_PARSER_isNext(this, 56)) {
;
return 25;
;
};
if (CZ_PARSER_isNext(this, 48)) {
;
return 22;
;
};
if (CZ_PARSER_isNext(this, 57)) {
;
return 24;
;
};
;
return 0;
;
}
NODE* CZ_PARSER_NewNode(PARSER* this) {
debugPrint("NewNode");
int cur=this->pos;
if ((CZ_PARSER_isNext(this, 16))==false) {
;
return 0;
;
};
this->pos+=1;
NODE* tryType=CZ_PARSER_type(this);
if (tryType==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing type in new");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:923:49\n");fprintf(stderr,"syntax error in new statement");exit(1);
//somehow destruct: err 
;
};
if (CZ_PARSER_isNext(this, 37)) {
this->pos+=1;
NODE* tryExp=CZ_PARSER_expression(this);
if (tryExp==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing expression in new");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:933:53\n");fprintf(stderr,"syntax error in new statement");exit(1);
//somehow destruct: err 
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=23;
CZ_NODE_consumeBinary(node, tryType, tryExp, 0);
;
return node;
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=23;
CZ_NODE_consumeUnary(node, tryType, 0);
;
return node;
;
}
NODE* CZ_PARSER_assignment(PARSER* this) {
debugPrint("assignment");
int cur=this->pos;
NODE* trySingle=CZ_PARSER_leftSingle(this);
if (trySingle==0) {
//somehow destruct: trySingle 
;
return 0;
;
};
int operand=CZ_PARSER_isNextAssignment(this);
if (operand==0) {
this->pos=cur;
//somehow destruct: trySingle 
;
return 0;
;
};
debugPrint("operand found");
this->pos+=1;
NODE* trynew=CZ_PARSER_NewNode(this);
if (trynew!=0) {
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=13;
CZ_NODE_consumeBinary(node, trySingle, trynew, operand);
;
return node;
;
};
NODE* tryExp=CZ_PARSER_expression(this);
if (tryExp==0) {
this->pos=cur;
//somehow destruct: tryExp trynew trySingle 
;
return 0;
;
};
debugPrint("expression found");
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=13;
CZ_NODE_consumeBinary(node, trySingle, tryExp, operand);
//somehow destruct: trynew 
;
return node;
//somehow destruct: trynew 
;
}
NODE* CZ_PARSER_unaryAssignment(PARSER* this) {
debugPrint("unaryAssigment");
int cur=this->pos;
NODE* trySingle=CZ_PARSER_leftSingle(this);
if (trySingle==0) {
//somehow destruct: trySingle 
;
return 0;
;
};
int operand=0;
if (CZ_PARSER_isNext(this, 1)) {
operand=14;
;
}
else if (CZ_PARSER_isNext(this, 3)) {
operand=16;
;
}
else {
this->pos=cur;
//somehow destruct: trySingle 
;
return 0;
;
};
this->pos+=1;
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=14;
CZ_NODE_consumeUnary(node, trySingle, operand);
;
return node;
;
}
NODE* CZ_PARSER_innerlines(PARSER* this) {
debugPrint("lines");
int cur=this->pos;
NODE* tryVarDef=CZ_PARSER_varDefinition(this);
if (tryVarDef!=0) {
if (CZ_PARSER_isNext(this, 52)) {
this->pos+=1;
NODE* tryNew=CZ_PARSER_NewNode(this);
if (tryNew!=0) {
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=13;
CZ_NODE_consumeBinary(node, tryVarDef, tryNew, 20);
if (CZ_PARSER_isNext(this, 40)) {
this->pos+=1;
;
}
else {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing semicolon");
CZ_Error_print(err, true);
this->pos=cur;
//somehow destruct: err node 
;
return 0;
//somehow destruct: err 
;
};
;
return node;
;
};
NODE* tryExp=CZ_PARSER_expression(this);
if (tryExp==0) {
this->pos=cur;
//somehow destruct: tryExp tryNew tryVarDef 
;
return 0;
;
};
NODE* oldNode=tryVarDef;
tryVarDef=malloc(sizeof(NODE));
tryVarDef->tokensStart=oldNode->tokensStart;
tryVarDef->tokensEnd=this->pos-1;
tryVarDef->type=13;
CZ_NODE_consumeBinary(tryVarDef, oldNode, tryExp, 20);
//somehow destruct: tryNew 
;
};
if (CZ_PARSER_isNext(this, 40)) {
this->pos+=1;
;
}
else {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing semicolon");
CZ_Error_print(err, true);
this->pos=cur;
//somehow destruct: err tryVarDef 
;
return 0;
//somehow destruct: err 
;
};
;
return tryVarDef;
;
};
NODE* tryUnaryAss=CZ_PARSER_unaryAssignment(this);
if (tryUnaryAss!=0) {
if (CZ_PARSER_isNext(this, 40)) {
this->pos+=1;
;
}
else {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing semicolon");
CZ_Error_print(err, true);
this->pos=cur;
//somehow destruct: err tryUnaryAss tryVarDef 
;
return 0;
//somehow destruct: err 
;
};
//somehow destruct: tryVarDef 
;
return tryUnaryAss;
;
};
NODE* tryAssignment=CZ_PARSER_assignment(this);
if (tryAssignment!=0) {
if (CZ_PARSER_isNext(this, 40)) {
this->pos+=1;
;
}
else {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing semicolon");
CZ_Error_print(err, true);
this->pos=cur;
//somehow destruct: err tryAssignment tryUnaryAss tryVarDef 
;
return 0;
//somehow destruct: err 
;
};
//somehow destruct: tryUnaryAss tryVarDef 
;
return tryAssignment;
;
};
NODE* tryExp=CZ_PARSER_expression(this);
if (tryExp!=0) {
if (CZ_PARSER_isNext(this, 40)) {
this->pos+=1;
;
}
else {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing semicolon");
CZ_Error_print(err, true);
this->pos=cur;
//somehow destruct: err tryExp tryAssignment tryUnaryAss tryVarDef 
;
return 0;
//somehow destruct: err 
;
};
//somehow destruct: tryAssignment tryUnaryAss tryVarDef 
;
return tryExp;
;
};
NODE* tryFlow=CZ_PARSER_flow(this);
if (tryFlow!=0) {
//somehow destruct: tryExp tryAssignment tryUnaryAss tryVarDef 
;
return tryFlow;
;
};
NODE* tryBlock=CZ_PARSER_blockLines(this);
if (tryBlock!=0) {
//somehow destruct: tryFlow tryExp tryAssignment tryUnaryAss tryVarDef 
;
return tryBlock;
;
};
//somehow destruct: tryBlock tryFlow tryExp tryAssignment tryUnaryAss tryVarDef 
;
return 0;
//somehow destruct: tryBlock tryFlow tryExp tryAssignment tryUnaryAss tryVarDef 
;
}
NODE* CZ_PARSER_lines(PARSER* this) {
debugPrint("lines");
int cur=this->pos;
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=10;
node->tokensStart=cur;
CZ_NODE_createArr(node);
while (1) {
NODE* tryInner=CZ_PARSER_innerlines(this);
if (tryInner==0) {
node->tokensEnd=this->pos-1;
//somehow destruct: tryInner 
;
return node;
;
};
CZ_NODE_add(node, tryInner);
;
};
//somehow destruct: node 
;
}
NODE* CZ_PARSER_blockLines(PARSER* this) {
debugPrint("block");
int cur=this->pos;
if ((CZ_PARSER_isNext(this, 35))==false) {
;
return 0;
;
};
this->pos+=1;
NODE* lines=CZ_PARSER_lines(this);
if ((CZ_PARSER_isNext(this, 36))==false) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing } in block");
CZ_Error_print(err, true);
this->pos=cur;
//somehow destruct: err lines 
;
return 0;
//somehow destruct: err 
;
};
this->pos+=1;
//somehow destruct: lines 
;
return lines;
//somehow destruct: lines 
;
}
NODE* CZ_PARSER_blockOrIterExpr(PARSER* this) {
debugPrint("blockOrIterExpr");
int cur=this->pos;
if (CZ_PARSER_isNext(this, 51)) {
this->pos+=1;
NODE* tryExp=CZ_PARSER_innerlines(this);
if (tryExp==0) {
this->pos=cur;
//somehow destruct: tryExp 
;
return 0;
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=tryExp->tokensStart;
node->tokensEnd=tryExp->tokensEnd;
node->type=10;
CZ_NODE_consumeUnary(node, tryExp, 0);
;
return node;
;
};
NODE* tryBlock=CZ_PARSER_blockLines(this);
if (tryBlock==0) {
this->pos=cur;
//somehow destruct: tryBlock 
;
return 0;
;
};
;
return tryBlock;
;
}
NODE* CZ_PARSER_range(PARSER* this) {
debugPrint("range");
int cur=this->pos;
NODE* tryExp=CZ_PARSER_expression(this);
if (tryExp==0) {
//somehow destruct: tryExp 
;
return 0;
;
};
int operand=0;
if (CZ_PARSER_isNext(this, 25)) {
operand=9;
;
}
else if (CZ_PARSER_isNext(this, 26)) {
operand=11;
;
}
else {
this->pos=cur;
free(tryExp);
//somehow destruct: tryExp 
;
return 0;
;
};
this->pos+=1;
NODE* trySecExp=CZ_PARSER_expression(this);
if (trySecExp==0) {
this->pos=cur;
free(tryExp);
//somehow destruct: trySecExp tryExp 
;
return 0;
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=5;
CZ_NODE_consumeBinary(node, tryExp, trySecExp, operand);
;
return node;
;
}
NODE* CZ_PARSER_rangedFor(PARSER* this) {
debugPrint("rangedFor");
int cur=this->pos;
if ((CZ_PARSER_isNext(this, 13))==false) {
debugPrint("missing for");
;
return 0;
;
};
this->pos+=1;
NODE* tryVarDef=CZ_PARSER_varDefinition(this);
if (tryVarDef==0) {
this->pos=cur;
//somehow destruct: tryVarDef 
;
return 0;
;
};
NODE* forNode=malloc(sizeof(NODE));
CZ_NODE_createArr(forNode);
forNode->type=6;
forNode->tokensStart=cur;
if ((CZ_PARSER_isNext(this, 51))==false) {
this->pos=cur;
free(tryVarDef);
//somehow destruct: forNode tryVarDef 
;
return 0;
;
};
this->pos+=1;
NODE* tryRange=CZ_PARSER_range(this);
if (tryRange==0) {
this->pos=cur;
free(tryVarDef);
//somehow destruct: tryRange forNode tryVarDef 
;
return 0;
;
};
NODE* tryBlock=CZ_PARSER_blockLines(this);
if (tryBlock==0) {
this->pos=cur;
free(tryVarDef);
free(tryRange);
//somehow destruct: tryBlock tryRange forNode tryVarDef 
;
return 0;
;
};
CZ_NODE_add(forNode, tryVarDef);
CZ_NODE_add(forNode, tryRange);
CZ_NODE_add(forNode, tryBlock);
forNode->tokensEnd=this->pos-1;
//somehow destruct: forNode 
;
return forNode;
//somehow destruct: forNode 
;
}
NODE* CZ_PARSER_ifStatement(PARSER* this) {
debugPrint("ifStatement");
int cur=this->pos;
if ((CZ_PARSER_isNext(this, 10))==false) {
;
return 0;
;
};
this->pos+=1;
NODE* tryExp=CZ_PARSER_expression(this);
if (tryExp==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing expression in if");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:1300:48\n");fprintf(stderr,"syntax error in if statement");exit(1);
//somehow destruct: err 
;
};
NODE* blockoriter=CZ_PARSER_blockOrIterExpr(this);
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=9;
CZ_NODE_consumeBinary(node, tryExp, blockoriter, 6);
while (1) {
int secCur=this->pos;
if (CZ_PARSER_isNext(this, 11)) {
this->pos+=1;
if (CZ_PARSER_isNext(this, 10)) {
this->pos+=1;
NODE* tryExp2=CZ_PARSER_expression(this);
if (tryExp2==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing expression in else if");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:1322:70\n");fprintf(stderr,"syntax error in else if expr statement");exit(1);
//somehow destruct: err 
;
};
NODE* blockoriter2=CZ_PARSER_blockOrIterExpr(this);
if (blockoriter2==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing block in else if");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:1329:65\n");fprintf(stderr,"syntax error in else if statement");exit(1);
//somehow destruct: err 
;
};
CZ_NODE_add(node, tryExp2);
CZ_NODE_add(node, blockoriter2);
;
}
else {
NODE* blockoriter2=CZ_PARSER_blockOrIterExpr(this);
if (blockoriter2==0) {
Error* err=CZ_PARSER_generateError(this);
CZ_STR_appendPtr(err->secMessage, "missing block in else");
CZ_Error_print(err, true);
fprintf(stderr,"src/parser/parser.cz:1341:62\n");fprintf(stderr,"syntax error in else statement");exit(1);
//somehow destruct: err 
;
};
CZ_NODE_add(node, blockoriter2);
;
};
;
}
else {
;
return node;
;
};
;
};
//somehow destruct: node 
;
}
NODE* CZ_PARSER_whileStatement(PARSER* this) {
debugPrint("whileStatement");
int cur=this->pos;
if ((CZ_PARSER_isNext(this, 12))==false) {
;
return 0;
;
};
this->pos+=1;
NODE* tryExp=CZ_PARSER_expression(this);
if (tryExp==0) {
printError(this->pos, this->tokens, this->text, "missing expression in while");
fprintf(stderr,"src/parser/parser.cz:1363:51\n");fprintf(stderr,"syntax error in while statement");exit(1);
;
};
NODE* blockoriter=CZ_PARSER_blockOrIterExpr(this);
if (blockoriter==0) {
printError(this->pos, this->tokens, this->text, "missing block in while");
fprintf(stderr,"src/parser/parser.cz:1368:51\n");fprintf(stderr,"syntax error in while statement");exit(1);
;
};
NODE* node=malloc(sizeof(NODE));
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
node->type=8;
CZ_NODE_consumeBinary(node, tryExp, blockoriter, 6);
;
return node;
;
}
NODE* CZ_PARSER_flow(PARSER* this) {
debugPrint("flow");
int cur=this->pos;
if (CZ_PARSER_isNext(this, 17)) {
this->pos+=1;
if (CZ_PARSER_isNext(this, 40)) {
this->pos+=1;
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=4;
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
;
return node;
;
}
else {
printError(this->pos, this->tokens, this->text, "missing semicolon");
fprintf(stderr,"src/parser/parser.cz:1398:58\n");fprintf(stderr,"syntax error in continue statement");exit(1);
;
};
;
};
this->pos=cur;
if (CZ_PARSER_isNext(this, 15)) {
this->pos+=1;
if (CZ_PARSER_isNext(this, 40)) {
this->pos+=1;
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=4;
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
;
return node;
;
}
else {
printError(this->pos, this->tokens, this->text, "missing semicolon");
fprintf(stderr,"src/parser/parser.cz:1419:55\n");fprintf(stderr,"syntax error in break statement");exit(1);
;
};
;
};
this->pos=cur;
if (CZ_PARSER_isNext(this, 14)) {
this->pos+=1;
if (CZ_PARSER_isNext(this, 40)) {
this->pos+=1;
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=4;
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
;
return node;
;
};
NODE* tryExp=CZ_PARSER_expression(this);
if (tryExp==0) {
this->pos=cur;
//somehow destruct: tryExp 
;
return 0;
;
};
if (CZ_PARSER_isNext(this, 40==false)) {
printError(this->pos, this->tokens, this->text, "missing semicolon");
fprintf(stderr,"src/parser/parser.cz:1445:56\n");fprintf(stderr,"syntax error in return statement");exit(1);
;
};
this->pos+=1;
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=4;
node->tokensStart=cur;
node->tokensEnd=this->pos-1;
CZ_NODE_consumeUnary(node, tryExp, 0);
;
return node;
;
};
this->pos=cur;
NODE* tryIf=CZ_PARSER_ifStatement(this);
if (tryIf!=0) {
;
return tryIf;
;
};
NODE* tryWhile=CZ_PARSER_whileStatement(this);
if (tryWhile!=0) {
//somehow destruct: tryIf 
;
return tryWhile;
;
};
NODE* tryRangeFor=CZ_PARSER_rangedFor(this);
if (tryRangeFor!=0) {
//somehow destruct: tryWhile tryIf 
;
return tryRangeFor;
;
};
this->pos=cur;
//somehow destruct: tryRangeFor tryWhile tryIf 
;
return 0;
//somehow destruct: tryRangeFor tryWhile tryIf 
;
}
void CZ_PARSER_consumeFunctionDecorators(PARSER* this, FunctionNode* func) {
while (1) {
if (CZ_PARSER_isNext(this, 64)) {
this->pos+=1;
func->isDepricated=true;
;
continue;
;
};
;
return;
;
};
;
return;
;
}
FunctionNode* CZ_PARSER_function(PARSER* this) {
debugPrint("function");
int cur=this->pos;
FunctionNode* fncNode=malloc(sizeof(FunctionNode));
CZ_FunctionNode_init(fncNode);
CZ_PARSER_consumeFunctionDecorators(this, fncNode);
if ((CZ_PARSER_isNext(this, 27))==false) {
//somehow destruct: fncNode 
;
return 0;
;
};
NODE* fun=malloc(sizeof(NODE));
CZ_NODE_init(fun);
CZ_NODE_createArr(fun);
fun->type=0;
fun->tokensStart=this->pos;
fun->tokensEnd=this->pos;
this->pos+=1;
while (1) {
int secCur=this->pos;
NODE* tryVarDef=CZ_PARSER_varDefinition(this);
if (tryVarDef==0) {
//somehow destruct: tryVarDef 
;
break;
;
};
CZ_NODE_add(fun, tryVarDef);
if ((CZ_PARSER_isNext(this, 37))==false) {
;
break;
;
};
this->pos+=1;
;
};
if (CZ_PARSER_isNext(this, 35)) {
this->pos+=1;
debugPrint("function return type");
NODE* tryType=CZ_PARSER_type(this);
if (tryType==0) {
printError(this->pos-1, this->tokens, this->text, "missing return type");
this->pos=fun->tokensStart;
//somehow destruct: tryType fun fncNode 
;
return 0;
;
};
CZ_NODE_add(fun, tryType);
debugPrint("function block");
NODE* lines=CZ_PARSER_lines(this);
if (CZ_PARSER_isNext(this, 36)) {
this->pos+=1;
CZ_NODE_add(fun, lines);
fun->tokensEnd=this->pos-1;
char* name=this->tokens[fun->tokensStart]->value;
CZ_FunctionNode_consume(fncNode, fun, name, this->tokens);
//somehow destruct: fncNode 
;
return fncNode;
;
};
printError(this->pos, this->tokens, this->text, "missing right cloudnt parse line");
fprintf(stderr,"src/parser/parser.cz:1559:50\n");fprintf(stderr,"unhandled function block error");exit(1);
//somehow destruct: lines 
;
}
else {
this->pos=cur;
//somehow destruct: fun fncNode 
;
return 0;
;
};
fprintf(stderr,"src/parser/parser.cz:1568:40\n");fprintf(stderr,"Unhandled function error");exit(1);
//somehow destruct: fun fncNode 
;
}
STR* CZ_PARSER_parsePackageName(PARSER* this) {
debugPrint("parsePackageName");
STR* str=malloc(sizeof(STR));
CZ_STR_init(str);
CZ_STR_alloc(str, 100);
while (1) {
if ((CZ_PARSER_isNext(this, 27))==false) {
debugPrint("invalid package no identifier");
//somehow destruct: str 
;
return 0;
;
};
CZ_STR_appendPtr(str, this->tokens[this->pos]->value);
this->pos+=1;
if ((CZ_PARSER_isNext(this, 39))==false) {
;
return str;
;
};
this->pos+=1;
if ((CZ_PARSER_isNext(this, 39))==false) {
printError(this->pos, this->tokens, this->text, "missing 4-dot in package name");
fprintf(stderr,"src/parser/parser.cz:1592:53\n");fprintf(stderr,"missing 4-dot in package name");exit(1);
;
};
this->pos+=1;
CZ_STR_appendPtr(str, "::");
;
};
fprintf(stderr,"src/parser/parser.cz:1599:40\n");fprintf(stderr,"this should never happen");exit(1);
//somehow destruct: str 
;
}
ENUM* CZ_PARSER_enm(PARSER* this) {
debugPrint("enum");
int cur=this->pos;
if ((CZ_PARSER_isNext(this, 20))==false) {
;
return 0;
;
};
this->pos+=1;
if ((CZ_PARSER_isNext(this, 27))==false) {
printError(this->pos, this->tokens, this->text, "enum doesnt have name");
fprintf(stderr,"src/parser/parser.cz:1614:46\n");fprintf(stderr,"missing identifier in enum");exit(1);
;
};
char* enumName=this->tokens[this->pos]->value;
this->pos+=1;
if ((CZ_PARSER_isNext(this, 35))==false) {
printError(this->pos, this->tokens, this->text, "missing left brace in enum");
fprintf(stderr,"src/parser/parser.cz:1620:46\n");fprintf(stderr,"missing left brace in enum");exit(1);
;
};
this->pos+=1;
ENUM* en=malloc(sizeof(ENUM));
CZ_ENUM_init(en, enumName);
while (1) {
if ((CZ_PARSER_isNext(this, 27))==false) {
printError(this->pos, this->tokens, this->text, "invalid token in enum");
fprintf(stderr,"src/parser/parser.cz:1628:50\n");fprintf(stderr,"missing identifier in enum");exit(1);
;
};
CZ_ENUM_add(en, this->tokens[this->pos]->value);
this->pos+=1;
if (CZ_PARSER_isNext(this, 37)) {
this->pos+=1;
;
}
else {
;
break;
;
};
;
};
if ((CZ_PARSER_isNext(this, 36))==false) {
printError(this->pos, this->tokens, this->text, "missing right brace in enum");
fprintf(stderr,"src/parser/parser.cz:1641:47\n");fprintf(stderr,"missing right brace in enum");exit(1);
;
};
this->pos+=1;
;
return en;
;
}
STRCTENTRY* CZ_PARSER_strctDefs(PARSER* this) {
debugPrint("strctDefs");
int cur=this->pos;
if ((CZ_PARSER_isNext(this, 19))==false) {
;
return 0;
;
};
this->pos+=1;
if ((CZ_PARSER_isNext(this, 27))==false) {
printError(this->pos, this->tokens, this->text, "missing identifier in struct");
fprintf(stderr,"src/parser/parser.cz:1659:48\n");fprintf(stderr,"missing identifier in struct");exit(1);
;
};
char* strctName=this->tokens[this->pos]->value;
this->pos+=1;
if ((CZ_PARSER_isNext(this, 35))==false) {
printError(this->pos, this->tokens, this->text, "missing left brace in struct");
fprintf(stderr,"src/parser/parser.cz:1665:48\n");fprintf(stderr,"missing left brace in struct");exit(1);
;
};
this->pos+=1;
STRCTENTRY* strctEntry=malloc(sizeof(STRCTENTRY));
CZ_STRCTENTRY_init(strctEntry);
strctEntry->name=toSTR(strctName);
while (1) {
FunctionNode* tryfun=CZ_PARSER_function(this);
if (tryfun!=0) {
STR* funNameStr=toSTR(tryfun->funName);
NODE* retNode=tryfun->ast->children[tryfun->ast->size-2];
tryfun->returnType=generateType(retNode, this->tokens);
CZ_STRCTENTRY_addFun(strctEntry, funNameStr, tryfun);
;
continue;
;
};
STR* propName;
if (CZ_PARSER_isNext(this, 27)) {
char* propNameptr=this->tokens[this->pos]->value;
propName=toSTR(propNameptr);
this->pos+=1;
;
}
else {
//somehow destruct: propName tryfun 
;
break;
;
};
if ((CZ_PARSER_isNext(this, 39))==false) {
printError(this->pos-1, this->tokens, this->text, "missing colon in struct");
fprintf(stderr,"src/parser/parser.cz:1695:47\n");fprintf(stderr,"missing colon in struct");exit(1);
;
};
this->pos+=1;
NODE* tryType=CZ_PARSER_type(this);
if (tryType==0) {
printError(this->pos-1, this->tokens, this->text, "missing type in struct");
fprintf(stderr,"src/parser/parser.cz:1701:46\n");fprintf(stderr,"missing type in struct");exit(1);
;
};
TYPE* generatedType=generateType(tryType, this->tokens);
CZ_STRCTENTRY_addProp(strctEntry, propName, generatedType);
if ((CZ_PARSER_isNext(this, 40))==false) {
printError(this->pos-1, this->tokens, this->text, "missing semicolon in struct");
fprintf(stderr,"src/parser/parser.cz:1707:51\n");fprintf(stderr,"missing semicolon in struct");exit(1);
;
};
this->pos+=1;
//somehow destruct: tryType tryfun 
;
};
if ((CZ_PARSER_isNext(this, 36))==false) {
printError(this->pos-1, this->tokens, this->text, "missing right brace in struct");
fprintf(stderr,"src/parser/parser.cz:1713:49\n");fprintf(stderr,"missing right brace in struct");exit(1);
;
};
this->pos+=1;
;
return strctEntry;
;
}
PROGRAMAST* CZ_PARSER_parseProgram(PARSER* this) {
debugPrint("parseProgram");
PROGRAMAST* program=malloc(sizeof(PROGRAMAST));
CZ_PROGRAMAST_init(program);
while (1) {
if (CZ_PARSER_isNext(this, 22)) {
if (program->packageName!=0) {
fprintf(stderr,"src/parser/parser.cz:1727:51\n");fprintf(stderr,"package already defined");exit(1);
;
};
this->pos+=1;
STR* tryPackage=CZ_PARSER_parsePackageName(this);
if ((CZ_PARSER_isNext(this, 40))==false) {
printError(this->pos-1, this->tokens, this->text, "missing semicolon");
fprintf(stderr,"src/parser/parser.cz:1733:45\n");fprintf(stderr,"missing semicolon");exit(1);
;
};
this->pos+=1;
if (tryPackage==0) {
fprintf(stderr,"src/parser/parser.cz:1737:48\n");fprintf(stderr,"invalid package name");exit(1);
;
};
program->packageName=tryPackage;
;
continue;
;
};
if (CZ_PARSER_isNext(this, 21)) {
this->pos+=1;
STR* tryUseName=CZ_PARSER_parsePackageName(this);
if ((CZ_PARSER_isNext(this, 40))==false) {
printError(this->pos-1, this->tokens, this->text, "missing semicolon");
fprintf(stderr,"src/parser/parser.cz:1749:45\n");fprintf(stderr,"missing semicolon");exit(1);
;
};
this->pos+=1;
if (tryUseName==0) {
fprintf(stderr,"src/parser/parser.cz:1753:44\n");fprintf(stderr,"invalid use name");exit(1);
;
};
CZ_PROGRAMAST_consumeImport(program, tryUseName);
;
continue;
;
};
STRCTENTRY* tryStruct=CZ_PARSER_strctDefs(this);
if (tryStruct!=0) {
CZ_PROGRAMAST_consumeStruct(program, tryStruct);
;
continue;
;
};
ENUM* tryEnum=CZ_PARSER_enm(this);
if (tryEnum!=0) {
CZ_PROGRAMAST_consumeEnum(program, tryEnum);
//somehow destruct: tryStruct 
;
continue;
;
};
FunctionNode* tryFun=CZ_PARSER_function(this);
if (tryFun!=0) {
char* name=tryFun->funName;
bool isMain=true;
char* mainIdent=(char*)("main");
NODE* retNode;
retNode=tryFun->ast->children[tryFun->ast->size -2];
tryFun->returnType=generateType(retNode, this->tokens);
for(int i = 0; i < 4; i++) {
if (name[i]!=mainIdent[i]) {
isMain=false;
;
break;
;
};
;
};
if (isMain) {
CZ_PROGRAMAST_consumeMain(program, tryFun);
;
}
else {
CZ_PROGRAMAST_consumeFunction(program, tryFun);
;
};
;
continue;
//somehow destruct: retNode 
;
};
//somehow destruct: tryFun tryEnum tryStruct 
;
return program;
//somehow destruct: tryFun tryEnum tryStruct 
;
};
//somehow destruct: program 
;
}
PROGRAMAST* CZ_PARSER_parse(PARSER* this) {
debugPrint("parse");
this->pos=0;
PROGRAMAST* node=CZ_PARSER_parseProgram(this);
if (node==0) {
fprintf(stderr,"src/parser/parser.cz:1806:43\n");fprintf(stderr,"invalid cloud not parse");exit(1);
;
};
if (CZ_PARSER_isNext(this, 65)) {
this->pos+=1;
;
};
;
return node;
;
}
#endif

#ifndef CZECHTINA_czechtina__optimalization__borrowchecker
#define CZECHTINA_czechtina__optimalization__borrowchecker
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__parser__ast.c"
#include "czechtina__compiler.c"
#include "czechtina__parser__programAST.c"
#include "czechtina__compiler__scopes.c"
#include "czechtina__compiler__enums.c"
#include "czechtina__compiler__strcts.c"
#include "czechtina__compiler__functions.c"
#include "czechtina__compiler__types.c"
#include "czechtina__lexer__token.c"
#include "std.c"
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _BorrowChecker BorrowChecker;
typedef struct _BorrowChecker {
COMPILEPROCESS* process;
SCOPE* scope;
SCOPE* safeScope;
SCOPE* loopSafeScope;
bool shouldPrint;
} BorrowChecker;
void CZ_BorrowChecker_scopePop(BorrowChecker* this);
void CZ_BorrowChecker_scopePush(BorrowChecker* this);
void CZ_BorrowChecker_printLeakedUntilSafe(BorrowChecker* this);
void CZ_BorrowChecker_testFunctionLeak(BorrowChecker* this, FunctionNode* ast);
void CZ_BorrowChecker_testStructLeak(BorrowChecker* this, STRCTENTRY* strct);
void CZ_BorrowChecker_testRecursive(BorrowChecker* this, NODE* ast, NODE* parentAst);
void CZ_BorrowChecker_scopePop(BorrowChecker* this) {
SCOPE* scope;
scope = this->scope;
if (scope->parent==0) {
fprintf(stderr,"src/optimalization/borrowChecker.cz:26:41\n");fprintf(stderr,"Cant pop global scope");exit(1);
;
};
this->scope = scope->parent;
if (!this->shouldPrint) {
//somehow destruct: scope 
;
return;
;
};
for(int i = 0; i < scope->size; i++) {
DEFINEDVARS* def=scope->vars[scope->size-1-i];
if (def->typ->isMemMove&&def->moved==false) {
fprintf(czStdOut,"\t\033[91m  Variable "); fprintf(czStdOut,"%s", def->name->ptr); fprintf(czStdOut," leaks memory\033[39m"); fputs("\n",czStdOut);
;
};
;
};
//somehow destruct: scope 
;
}
void CZ_BorrowChecker_scopePush(BorrowChecker* this) {
SCOPE* scope=malloc(sizeof(SCOPE));
CZ_SCOPE_init(scope);
scope->parent = this->scope;
this->scope=scope;
;
}
void CZ_BorrowChecker_printLeakedUntilSafe(BorrowChecker* this) {
SCOPE* scope=this->scope;
while (scope!=this->safeScope) {
for(int i = 0; i < scope->size; i++) {
DEFINEDVARS* def=scope->vars[scope->size-1-i];
if (def->typ->isMemMove&&def->moved==false) {
fprintf(czStdOut,"\t\033[91m  Variable "); fprintf(czStdOut,"%s", def->name->ptr); fprintf(czStdOut," leaks memory\033[39m"); fputs("\n",czStdOut);
;
};
;
};
scope=scope->parent;
;
};
;
}
void CZ_BorrowChecker_testFunctionLeak(BorrowChecker* this, FunctionNode* ast) {
if (this->shouldPrint) {
fprintf(czStdOut, "Testing function %s: %s\n", this->process->filePath->ptr, ast->funName);
;
};
this->scope=genererateGlobalScope();
CZ_SCOPE_init(this->scope);
this->safeScope=this->scope;
CZ_BorrowChecker_scopePush(this);
int pos=0;
while (pos<ast->ast->size-2) {
CZ_BorrowChecker_testRecursive(this, ast->ast->children[pos], ast->ast);
pos+=1;
;
};
CZ_BorrowChecker_testRecursive(this, ast->ast->children[ast->ast->size-1], ast->ast);
CZ_BorrowChecker_scopePop(this);
;
}
void CZ_BorrowChecker_testStructLeak(BorrowChecker* this, STRCTENTRY* strct) {
for(int i = 0; i < strct->fSize; i++) {
this->scope=genererateGlobalScope();
CZ_SCOPE_init(this->scope);
this->safeScope=this->scope;
FunctionNode* ast=strct->funs[i]->ast;
if (this->shouldPrint) {
fprintf(czStdOut, "Testing struct function %s: %s: %s\n", this->process->filePath->ptr, strct->name->ptr, ast->funName);
;
};
NODE** childs;
childs = (NODE**)strct->funs[i]->ast->ast->children;
CZ_BorrowChecker_scopePush(this);
STR* thisStr=toSTR("this");
TYPE* thisType=malloc(sizeof(TYPE));
thisType->name=toSTR(strct->name->ptr);
thisType->variant=6;
DEFINEDVARS* thisVar=malloc(sizeof(DEFINEDVARS));
thisVar->moved=ptrEqual(ast->funName, "destruct");
thisVar->name=thisStr;
thisVar->typ=thisType;
CZ_SCOPE_define(this->scope, thisVar);
int pos=0;
while (pos<ast->ast->size-2) {
CZ_BorrowChecker_testRecursive(this, childs[pos], ast->ast);
pos=pos+1;
;
};
CZ_BorrowChecker_testRecursive(this, childs[ast->ast->size-1], ast->ast);
CZ_BorrowChecker_scopePop(this);
;
};
;
}
void CZ_BorrowChecker_testRecursive(BorrowChecker* this, NODE* ast, NODE* parentAst) {
if (ast->type==10) {
int curSize=ast->size;
for(int i = 0; i < curSize; i++) {
CZ_BorrowChecker_testRecursive(this, ast->children[i], ast);
;
};
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=24;
node->strs=malloc(sizeof(STR*) *(2));
node->reserved=2;
SCOPE* scope=this->scope;
for(int i = 0; i < scope->size; i++) {
DEFINEDVARS* def=scope->vars[scope->size-1-i];
if (def->typ->isMemMove&&def->moved==false) {
STR* varstr=toSTR(def->name->ptr);
node->strs[node->size]=varstr;
node->size=node->size+1;
if (node->size==node->reserved) {
CZ_NODE_reallocStrs(node);
;
};
;
};
;
};
CZ_NODE_add(ast, node);
;
}
else if (ast->type==13) {
if (this->process->enableDeducer) {
if (ast->children[0]->type==19) {
TOKEN* token=this->process->tokens[ast->children[0]->tokensStart];
STR* strname=toSTR(token->value);
if ((CZ_SCOPE_isDefined(this->scope, strname))==false) {
DEFINEDVARS* vardef=malloc(sizeof(DEFINEDVARS));
vardef->moved=false;
vardef->name=strname;
vardef->typ=CZ_COMPILEPROCESS_deduceType(this->process, ast->children[1], this->scope);
CZ_SCOPE_define(this->scope, vardef);
if (ast->children[1]->type==23) {
vardef->typ->isMemMove=true;
;
}
else if (ast->children[1]->type==21) {
TYPE* deducedType=CZ_COMPILEPROCESS_deduceType(this->process, ast->children[1], this->scope);
if (deducedType->isMemMove) {
vardef->typ->isMemMove=true;
;
};
//somehow destruct: deducedType 
;
};
;
};
//somehow destruct: strname 
;
}
else if (ast->children[0]->type==20) {
TOKEN* token=this->process->tokens[ast->children[0]->children[0]->tokensStart];
STR* strname=toSTR(token->value);
if ((CZ_SCOPE_isDefined(this->scope, strname))==false) {
DEFINEDVARS* vardef=malloc(sizeof(DEFINEDVARS));
vardef->moved=false;
vardef->name=strname;
vardef->typ=generateType(ast->children[0]->children[1], this->process->tokens);
CZ_SCOPE_define(this->scope, vardef);
if (ast->children[1]->type==23) {
vardef->typ->isMemMove=true;
;
}
else if (ast->children[1]->type==21) {
TYPE* deducedType=CZ_COMPILEPROCESS_deduceType(this->process, ast->children[1], this->scope);
if (deducedType->isMemMove) {
vardef->typ->isMemMove=true;
;
};
//somehow destruct: deducedType 
;
};
;
};
//somehow destruct: strname 
;
}
else {
CZ_BorrowChecker_testRecursive(this, ast->children[0], ast);
;
};
;
};
CZ_BorrowChecker_testRecursive(this, ast->children[1], ast);
;
}
else if (ast->type==20) {
TOKEN* token=this->process->tokens[ast->children[0]->tokensStart];
STR* strname=toSTR(token->value);
DEFINEDVARS* vardef=malloc(sizeof(DEFINEDVARS));
vardef->moved=false;
vardef->name=strname;
vardef->typ=generateType(ast->children[1], this->process->tokens);
CZ_SCOPE_define(this->scope, vardef);
;
}
else if (ast->type==9) {
CZ_BorrowChecker_scopePush(this);
CZ_BorrowChecker_testRecursive(this, ast->children[1], ast);
CZ_BorrowChecker_scopePop(this);
int pos=2;
while (pos<ast->size-1) {
CZ_BorrowChecker_scopePush(this);
CZ_BorrowChecker_testRecursive(this, ast->children[pos+1], ast);
CZ_BorrowChecker_scopePop(this);
pos=pos+2;
;
};
if (pos<ast->size) {
CZ_BorrowChecker_scopePush(this);
CZ_BorrowChecker_testRecursive(this, ast->children[pos], ast);
CZ_BorrowChecker_scopePop(this);
;
};
;
}
else if (ast->type==8) {
this->loopSafeScope=this->scope;
CZ_BorrowChecker_scopePush(this);
CZ_BorrowChecker_testRecursive(this, ast->children[1], ast);
CZ_BorrowChecker_scopePop(this);
;
}
else if (ast->type==6) {
this->loopSafeScope=this->scope;
CZ_BorrowChecker_scopePush(this);
CZ_BorrowChecker_testRecursive(this, ast->children[0], ast);
CZ_BorrowChecker_testRecursive(this, ast->children[2], ast);
CZ_BorrowChecker_scopePop(this);
;
}
else if (ast->type==17) {
if (ast->operand==3) {
if (ast->children[0]->type==19) {
TOKEN* token=this->process->tokens[ast->children[0]->tokensStart];
STR* strname=toSTR(token->value);
bool onScope=false;
for(int i = 0; i < this->scope->size; i++) {
DEFINEDVARS* def=this->scope->vars[this->scope->size-1-i];
if (ptrEqual(def->name->ptr, strname->ptr)) {
def->moved=true;
onScope=true;
;
break;
;
};
;
};
if (onScope==false) {
DEFINEDVARS* def=malloc(sizeof(DEFINEDVARS));
def->name=strname;
TYPE* typ=CZ_SCOPE_getType(this->scope, strname);
def->typ=CZ_TYPE_copy(typ);
def->moved=true;
CZ_SCOPE_define(this->scope, def);
;
};
//somehow destruct: strname 
;
};
;
};
CZ_BorrowChecker_testRecursive(this, ast->children[0], ast);
;
}
else if (ast->type==4) {
if (ast->size>0) {
CZ_BorrowChecker_testRecursive(this, ast->children[0], ast);
;
};
TOKEN* token=this->process->tokens[ast->tokensStart];
if (token->type==14) {
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=24;
node->strs=malloc(sizeof(STR*) *(2));
node->reserved=2;
SCOPE* scope=this->scope;
while (scope!=this->safeScope) {
for(int i = 0; i < scope->size; i++) {
DEFINEDVARS* def=scope->vars[scope->size-1-i];
if (def->typ->isMemMove) {
DEFINEDVARS* getDef=CZ_SCOPE_get(this->scope, def->name->ptr);
if (getDef->moved==false) {
STR* varstr=toSTR(def->name->ptr);
node->strs[node->size]=varstr;
node->size=node->size+1;
if (node->size==node->reserved) {
CZ_NODE_reallocStrs(node);
;
};
;
};
;
};
;
};
scope=scope->parent;
;
};
CZ_NODE_addBeforeNode(parentAst, ast, node);
;
}
else if (token->type==15||token->type==17) {
NODE* node=malloc(sizeof(NODE));
CZ_NODE_init(node);
node->type=24;
node->strs=malloc(sizeof(STR*) *(2));
node->reserved=2;
SCOPE* scope=this->scope;
while (scope!=this->loopSafeScope&&scope!=this->safeScope) {
for(int i = 0; i < scope->size; i++) {
DEFINEDVARS* def=scope->vars[scope->size-1-i];
if (def->typ->isMemMove) {
DEFINEDVARS* getDef=CZ_SCOPE_get(this->scope, def->name->ptr);
if (getDef->moved==false) {
STR* varstr=toSTR(def->name->ptr);
node->strs[node->size]=varstr;
node->size=node->size+1;
if (node->size==node->reserved) {
CZ_NODE_reallocStrs(node);
;
};
;
};
;
};
;
};
scope=scope->parent;
;
};
CZ_NODE_addBeforeNode(parentAst, ast, node);
;
};
;
}
else if (ast->type==21) {
for(int i = 1; i < ast->size; i++) {
CZ_BorrowChecker_testRecursive(this, ast->children[i], ast);
;
};
;
};
;
}
#endif

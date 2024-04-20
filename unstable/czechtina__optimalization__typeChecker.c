#ifndef CZECHTINA_czechtina__optimalization__typeChecker
#define CZECHTINA_czechtina__optimalization__typeChecker
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__compiler__types.c"
#include "czechtina__parser__ast.c"
#include "czechtina__compiler.c"
#include "czechtina__parser__programAST.c"
#include "czechtina__compiler__scopes.c"
#include "czechtina__compiler__enums.c"
#include "czechtina__compiler__strcts.c"
#include "czechtina__compiler__functions.c"
#include "czechtina__lexer__token.c"
#include "czechtina__utils__error.c"
#include "std.c"
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _TypeChecker TypeChecker;
bool isPrimitiveNumeric(TYPE* type);
bool isCastAble(TYPE* t1, TYPE* t2);
typedef struct _TypeChecker {
COMPILEPROCESS* process;
SCOPE* scope;
} TypeChecker;
void CZ_TypeChecker_scopePop(TypeChecker* this);
void CZ_TypeChecker_scopePush(TypeChecker* this);
void CZ_TypeChecker_testFunctionLeak(TypeChecker* this, FunctionNode* ast);
void CZ_TypeChecker_testStructLeak(TypeChecker* this, STRCTENTRY* strct);
void CZ_TypeChecker_testRecursive(TypeChecker* this, NODE* ast, NODE* parentAst);
bool isPrimitiveNumeric(TYPE* type) {
;
return type->variant==3||type->variant==4||type->variant==2||type->variant==1;
;
}
bool isCastAble(TYPE* t1, TYPE* t2) {
if ((isPrimitiveNumeric(t1))&&(isPrimitiveNumeric(t2))) {
;
return true;
;
};
if (t1->isMemMove!=t2->isMemMove) {
;
return false;
;
};
if (t1->variant==5&&t2->variant==5) {
;
return isCastAble(t1->dereference, t2->dereference);
;
};
if (t1->variant==6&&t2->variant==6) {
;
return ptrEqual(t1->name->ptr, t2->name->ptr);
;
};
;
return false;
;
}
void CZ_TypeChecker_scopePop(TypeChecker* this) {
if (this->scope->parent==0) {
fprintf(stderr,"src/optimalization/typeChecker.cz:49:56\n");fprintf(stderr,"Cant pop global scope in typechecker");exit(1);
;
};
this->scope = this->scope->parent;
;
}
void CZ_TypeChecker_scopePush(TypeChecker* this) {
SCOPE* scope=malloc(sizeof(SCOPE));
CZ_SCOPE_init(scope);
scope->parent = this->scope ;
this->scope=scope;
;
}
void CZ_TypeChecker_testFunctionLeak(TypeChecker* this, FunctionNode* ast) {
this->scope=malloc(sizeof(SCOPE));
CZ_SCOPE_init(this->scope);
CZ_TypeChecker_scopePush(this);
int pos=0;
while (pos<ast->ast->size-2) {
CZ_TypeChecker_testRecursive(this, ast->ast->children[pos], ast->ast);
pos=pos+1;
;
};
CZ_TypeChecker_testRecursive(this, ast->ast->children[ast->ast->size-1], ast->ast);
CZ_TypeChecker_scopePop(this);
;
}
void CZ_TypeChecker_testStructLeak(TypeChecker* this, STRCTENTRY* strct) {
for(int i = 0; i < strct->fSize; i++) {
this->scope=malloc(sizeof(SCOPE));
CZ_SCOPE_init(this->scope);
FunctionNode* ast=strct->funs[i]->ast;
NODE** childs;
childs = (NODE**)strct->funs[i]->ast->ast->children;
CZ_TypeChecker_scopePush(this);
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
CZ_TypeChecker_testRecursive(this, childs[pos], ast->ast);
pos=pos+1;
;
};
CZ_TypeChecker_testRecursive(this, childs[ast->ast->size-1], ast->ast);
CZ_TypeChecker_scopePop(this);
;
};
;
}
void CZ_TypeChecker_testRecursive(TypeChecker* this, NODE* ast, NODE* parentAst) {
if (ast->type==10) {
int curSize=ast->size;
for(int i = 0; i < curSize; i++) {
CZ_TypeChecker_testRecursive(this, ast->children[i], ast);
;
};
;
}
else if (ast->type==22) {
CZ_TypeChecker_testRecursive(this, ast->children[1], ast);
TYPE* lefType=CZ_COMPILEPROCESS_deduceType(this->process, ast->children[0], this->scope);
TYPE* rigType=generateType(ast->children[1], this->process->tokens);
if ((isCastAble(rigType, lefType))==false) {
Error* err=CZ_COMPILEPROCESS_generateError(this->process, ast->children[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Cast type mismatch");
CZ_STR_appendPtr(err->secMessage, "Expected ");
STR* leftstr=CZ_TYPE_toSTR(lefType);
CZ_STR_appendPtr(err->secMessage, leftstr->ptr);
CZ_STR_appendPtr(err->secMessage, " got ");
STR* rightstr=CZ_TYPE_toSTR(rigType);
CZ_STR_appendPtr(err->secMessage, rightstr->ptr);
CZ_Error_print(err, true);
fprintf(stderr,"src/optimalization/typeChecker.cz:126:24\n");fprintf(stderr,"");exit(1);
//somehow destruct: rightstr leftstr err 
;
};
//somehow destruct: rigType lefType 
;
}
else if (ast->type==13) {
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
};
if (ast->children[1]->type!=23&&ast->children[1]->type!=21&&ast->children[1]->operand!=3) {
TYPE* typ=vardef->typ;
while (1) {
typ->isMemMove=false;
if (typ->variant==5) {
typ=typ->dereference;
;
}
else {
;
break;
;
};
;
};
//somehow destruct: typ 
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
;
};
//somehow destruct: strname 
;
}
else {
CZ_TypeChecker_testRecursive(this, ast->children[0], ast);
;
};
CZ_TypeChecker_testRecursive(this, ast->children[1], ast);
TYPE* lefType=CZ_COMPILEPROCESS_deduceType(this->process, ast->children[0], this->scope);
TYPE* rigType=CZ_COMPILEPROCESS_deduceType(this->process, ast->children[1], this->scope);
if (ast->children[1]->type!=23&&ast->children[1]->type!=21&&ast->children[1]->operand!=3) {
TYPE* type=rigType;
while (1) {
type->isMemMove=false;
if (type->variant==5) {
type=type->dereference;
;
}
else {
;
break;
;
};
;
};
;
};
if ((isCastAble(rigType, lefType))==false) {
if (lefType->variant==5&&rigType->variant==3) {
if (ast->operand==13) {
//somehow destruct: rigType lefType 
;
return;
;
}
else if (ast->operand==15) {
//somehow destruct: rigType lefType 
;
return;
;
}
else if (ast->operand==21) {
//somehow destruct: rigType lefType 
;
return;
;
}
else if (ast->operand==23) {
//somehow destruct: rigType lefType 
;
return;
;
};
;
};
if (ast->children[1]->type==21) {
if (ptrEqual(this->process->tokens[ast->children[1]->children[0]->tokensStart]->value, "inC")) {
Error* err=CZ_COMPILEPROCESS_generateError(this->process, ast->children[1]->tokensStart);
err->level=1;
CZ_STR_appendPtr(err->message, "InC in assignment");
CZ_Error_print(err, false);
//somehow destruct: err rigType lefType 
;
return;
//somehow destruct: err 
;
};
;
};
Error* err=CZ_COMPILEPROCESS_generateError(this->process, ast->children[1]->tokensStart);
err->level=1;
CZ_STR_appendPtr(err->message, "Assignment type mismatch");
CZ_STR_appendPtr(err->secMessage, "Expected ");
STR* leftstr=CZ_TYPE_toSTR(lefType);
CZ_STR_appendPtr(err->secMessage, leftstr->ptr);
CZ_STR_appendPtr(err->secMessage, " got ");
STR* rightstr=CZ_TYPE_toSTR(rigType);
CZ_STR_appendPtr(err->secMessage, rightstr->ptr);
CZ_Error_print(err, true);
//somehow destruct: rightstr leftstr err 
;
};
//somehow destruct: rigType lefType 
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
CZ_TypeChecker_scopePush(this);
CZ_TypeChecker_testRecursive(this, ast->children[1], ast);
CZ_TypeChecker_scopePop(this);
int pos=2;
while (pos<ast->size-1) {
CZ_TypeChecker_scopePush(this);
CZ_TypeChecker_testRecursive(this, ast->children[pos+1], ast);
CZ_TypeChecker_scopePop(this);
pos=pos+2;
;
};
if (pos<ast->size) {
CZ_TypeChecker_scopePush(this);
CZ_TypeChecker_testRecursive(this, ast->children[pos], ast);
CZ_TypeChecker_scopePop(this);
;
};
;
}
else if (ast->type==8) {
CZ_TypeChecker_scopePush(this);
CZ_TypeChecker_testRecursive(this, ast->children[1], ast);
CZ_TypeChecker_scopePop(this);
;
}
else if (ast->type==6) {
CZ_TypeChecker_scopePush(this);
CZ_TypeChecker_testRecursive(this, ast->children[0], ast);
CZ_TypeChecker_testRecursive(this, ast->children[2], ast);
CZ_TypeChecker_scopePop(this);
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
CZ_TypeChecker_testRecursive(this, ast->children[0], ast);
;
}
else if (ast->type==4) {
if (ast->size>0) {
CZ_TypeChecker_testRecursive(this, ast->children[0], ast);
;
};
TOKEN* token=this->process->tokens[ast->tokensStart];
;
}
else if (ast->type==21) {
FunctionNode* fun;
if (ast->children[0]->type==19) {
TOKEN* token=this->process->tokens[ast->children[0]->tokensStart];
if (ptrEqual(token->value, "new")) {
;
return;
;
}
else if (ptrEqual(token->value, "inC")) {
;
return;
;
}
else if (ptrEqual(token->value, "throw")) {
;
return;
;
}
else if (ptrEqual(token->value, "typeof")) {
;
return;
;
}
else if (ptrEqual(token->value, "sizeof")) {
;
return;
;
}
else if (ptrEqual(token->value, "printf")) {
;
return;
;
}
else if (ptrEqual(token->value, "print")) {
;
return;
;
}
else if (ptrEqual(token->value, "println")) {
;
return;
;
}
else if (ptrEqual(token->value, "hodnota")) {
;
return;
;
};
fun=CZ_COMPILEPROCESS_getFunctionByName(this->process, token->value);
;
}
else if (ast->children[0]->type==1) {
NODE** accessChilds=ast->children[0]->children;
TYPE* type=CZ_COMPILEPROCESS_deduceType(this->process, accessChilds[0], this->scope);
if (type->variant!=6) {
fprintf(stderr,"src/optimalization/typeChecker.cz:343:60\n");fprintf(stderr,"Cant access member of non struct");exit(1);
;
};
STRCTENTRY* strct=CZ_COMPILEPROCESS_getStructByName(this->process, type->name->ptr);
STRCTFUN* strctFun=CZ_STRCTENTRY_getFunction(strct, this->process->tokens[accessChilds[1]->tokensStart]->value);
fun=strctFun->ast;
//somehow destruct: type accessChilds 
;
};
if (fun==0) {
Error* err=CZ_COMPILEPROCESS_generateError(this->process, ast->children[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Function not found");
CZ_Error_print(err, true);
fprintf(stderr,"src/optimalization/typeChecker.cz:359:42\n");fprintf(stderr,"Function not found");exit(1);
//somehow destruct: err 
;
};
if (fun->ast->size-1!=ast->size) {
int pos=0;
if (fun->ast->size-2>=ast->size-1) {
pos=ast->size-1;
;
}
else {
pos=fun->ast->size-1;
;
};
Error* err=CZ_COMPILEPROCESS_generateError(this->process, ast->children[pos]->tokensStart);
CZ_STR_appendPtr(err->message, "Function call arguments count mismatch");
CZ_STR_appendPtr(err->secMessage, "Expected ");
CZ_STR_appendInt(err->secMessage, fun->ast->size-2);
CZ_STR_appendPtr(err->secMessage, " args got ");
CZ_STR_appendInt(err->secMessage, ast->size-1);
CZ_STR_appendPtr(err->secMessage, " args.");
CZ_Error_print(err, true);
fprintf(stderr,"src/optimalization/typeChecker.cz:378:24\n");fprintf(stderr,"");exit(1);
//somehow destruct: err 
;
};
for(int i = 0; i < fun->variants[0]->size; i++) {
STR* strdefined=CZ_TYPE_toSTR(fun->variants[0]->args[i]);
TYPE* funCalledType=CZ_COMPILEPROCESS_deduceType(this->process, ast->children[i+1], this->scope);
if (ast->children[i+1]->operand!=3&&ast->children[i+1]->type!=21) {
TYPE* typ=funCalledType;
while (1) {
typ->isMemMove=false;
if (typ->variant==5) {
typ=typ->dereference;
;
}
else {
;
break;
;
};
;
};
//somehow destruct: typ 
;
};
STR* strcalled=CZ_TYPE_toSTR(funCalledType);
if ((isCastAble(funCalledType, fun->variants[0]->args[i]))==false) {
Error* err=CZ_COMPILEPROCESS_generateError(this->process, ast->children[i+1]->tokensStart);
err->level=1;
CZ_STR_appendPtr(err->message, "Function call argument type mismatch");
CZ_STR_appendPtr(err->secMessage, "Expected ");
CZ_STR_appendPtr(err->secMessage, strdefined->ptr);
CZ_STR_appendPtr(err->secMessage, " got ");
CZ_STR_appendPtr(err->secMessage, strcalled->ptr);
CZ_Error_print(err, true);
//somehow destruct: err 
;
};
//somehow destruct: strcalled funCalledType strdefined 
;
};
for(int i = 1; i < ast->size; i++) {
CZ_TypeChecker_testRecursive(this, ast->children[i], ast);
;
};
;
};
;
}
#endif

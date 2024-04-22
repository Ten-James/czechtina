#ifndef CZECHTINA_czechtina__compiler
#define CZECHTINA_czechtina__compiler
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__parser__ast.c"
#include "czechtina__parser__programAST.c"
#include "czechtina__lexer__token.c"
#include "czechtina__lexer.c"
#include "czechtina__parser.c"
#include "czechtina__compiler__scopes.c"
#include "czechtina__compiler__enums.c"
#include "czechtina__compiler__strcts.c"
#include "czechtina__compiler__functions.c"
#include "czechtina__compiler__types.c"
#include "czechtina__utils__printer.c"
#include "czechtina__utils__levenshtein.c"
#include "czechtina__utils__error.c"
#include "std.c"
#include "std__str.c"
#include "std__files.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _TYPEDEFS TYPEDEFS;
typedef struct _COMPILEPROCESS COMPILEPROCESS;
void preludeDeclare(int i);
void preludeDefine(int i);
typedef struct _TYPEDEFS {
STR* name;
TYPE* type;
} TYPEDEFS;
typedef struct _COMPILEPROCESS {
bool debug;
TYPEDEFS** defs;
int dSize;
int dReserved;
STR* filePath;
PROGRAMAST* program;
char* text;
TOKEN** tokens;
SCOPE* scope;
bool enableDeducer;
COMPILEPROCESS** sources;
int sSize;
} COMPILEPROCESS;
void CZ_COMPILEPROCESS_scopePop(COMPILEPROCESS* this);
void CZ_COMPILEPROCESS_scopePush(COMPILEPROCESS* this);
void CZ_COMPILEPROCESS_init(COMPILEPROCESS* this, STR* text, bool enableDeducer);
Error* CZ_COMPILEPROCESS_generateError(COMPILEPROCESS* this, int pos);
bool CZ_COMPILEPROCESS_isPackageImported(COMPILEPROCESS* this, char* name);
bool CZ_COMPILEPROCESS_isFunctionDefined(COMPILEPROCESS* this, char* name, int pos);
FunctionNode* CZ_COMPILEPROCESS_getFunctionByName(COMPILEPROCESS* this, char* ptr);
bool CZ_COMPILEPROCESS_isStructDefined(COMPILEPROCESS* this, char* name, int pos);
STRCTENTRY* CZ_COMPILEPROCESS_getStructByName(COMPILEPROCESS* this, char* ptr);
char* CZ_COMPILEPROCESS_getClosestStructByName(COMPILEPROCESS* this, char* ptr);
bool CZ_COMPILEPROCESS_isEnumDefined(COMPILEPROCESS* this, TOKEN* token, int pos);
ENUM* CZ_COMPILEPROCESS_getEnumByName(COMPILEPROCESS* this, char* ptr);
bool CZ_COMPILEPROCESS_isTypeDefined(COMPILEPROCESS* this, char* name);
TYPE* CZ_COMPILEPROCESS_geTypeByName(COMPILEPROCESS* this, char* name);
TYPE* CZ_COMPILEPROCESS_deduceType(COMPILEPROCESS* this, NODE* ast, SCOPE* scope);
void CZ_COMPILEPROCESS_virtualPrint(COMPILEPROCESS* this, NODE* ast);
void CZ_COMPILEPROCESS_printScope(COMPILEPROCESS* this);
void CZ_COMPILEPROCESS_compile(COMPILEPROCESS* this, char* buildDir);
void CZ_COMPILEPROCESS_toCAst(COMPILEPROCESS* this, NODE* ast);
void CZ_COMPILEPROCESS_funToCAst(COMPILEPROCESS* this, FunctionNode* ast, bool declr, bool isMain);
void CZ_COMPILEPROCESS_strctfunToCAst(COMPILEPROCESS* this, FunctionNode* ast, bool declr, STR* strctName);
void CZ_COMPILEPROCESS_strctPrintf(COMPILEPROCESS* this, STRCTENTRY* strct, bool declr);
void CZ_COMPILEPROCESS_printUnusedEnumsValue(COMPILEPROCESS* this);
void preludeDeclare(int i) {
fprintf(czStdOut, "extern void* czStdOut;\n");
fprintf(czStdOut, "extern void* czStdIn;\n");
fprintf(czStdOut, "typedef void* FileStream;\n");
;
}
void preludeDefine(int i) {
fprintf(czStdOut, "void* czStdOut = 0;\n");
fprintf(czStdOut, "void* czStdIn = 0;\n");
fprintf(czStdOut, "typedef void* FileStream;\n");
;
}
void CZ_COMPILEPROCESS_scopePop(COMPILEPROCESS* this) {
SCOPE* scope=this->scope;
if (scope->parent==0) {
fprintf(stderr,"src/compiler/compiler.cz:57:41\n");fprintf(stderr,"Cant pop global scope");exit(1);
;
};
this->scope=scope->parent;
free(scope);
//somehow destruct: scope 
;
}
void CZ_COMPILEPROCESS_scopePush(COMPILEPROCESS* this) {
SCOPE* scope=malloc(sizeof(SCOPE));
CZ_SCOPE_init(scope);
scope->parent=this->scope;
this->scope=scope;
;
}
void CZ_COMPILEPROCESS_init(COMPILEPROCESS* this, STR* text, bool enableDeducer) {
this->filePath=CZ_STR_copy(text);
this->text=readFile(text->ptr);
ARR* tokens=lex(this->text);
int tokenSize=tokens->size;
this->defs=malloc(sizeof(TYPEDEFS*) *(1));
TYPEDEFS* filestream=malloc(sizeof(TYPEDEFS));
filestream->name=toSTR("FileStream");
this->debug=false;
TYPE* voidType=malloc(sizeof(TYPE));
voidType->variant=0;
TYPE* pointerType=malloc(sizeof(TYPE));
pointerType->variant=5;
pointerType->dereference=voidType;
filestream->type=pointerType;
//somehow destruct: filestream 
;
;
this->defs[0]=filestream;
this->dSize=1;
this->dReserved=1;
this->tokens=(TOKEN**)tokens->arr;
PARSER* parser=malloc(sizeof(PARSER));
parser->filePath=this->filePath->ptr;
parser->tokens=this->tokens;
parser->text=this->text;
parser->max=tokenSize;
this->program=CZ_PARSER_parse(parser);
this->enableDeducer=enableDeducer;
//somehow destruct: parser 
;
}
Error* CZ_COMPILEPROCESS_generateError(COMPILEPROCESS* this, int pos) {
Error* err=malloc(sizeof(Error));
CZ_Error_init(err);
err->pos=pos;
err->tokens=this->tokens;
err->text=this->text;
err->filePath=this->filePath->ptr;
err->pos=pos;
err->level=3;
;
return err;
;
}
bool CZ_COMPILEPROCESS_isPackageImported(COMPILEPROCESS* this, char* name) {
for(int i = 0; i < this->program->iSize; i++) {
if (ptrEqual(this->program->imports[i]->ptr, name)) {
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
bool CZ_COMPILEPROCESS_isFunctionDefined(COMPILEPROCESS* this, char* name, int pos) {
for(int i = 0; i < this->program->fSize; i++) {
if (ptrEqual(this->program->functions[i]->funName, name)) {
;
return true;
;
};
;
};
for(int i = 0; i < this->sSize; i++) {
for(int j = 0; j < this->sources[i]->program->fSize; j++) {
if (ptrEqual(this->sources[i]->program->functions[j]->funName, name)) {
if (CZ_COMPILEPROCESS_isPackageImported(this, this->sources[i]->program->packageName->ptr)) {
;
return true;
;
};
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, pos);
CZ_STR_appendPtr(err->message, "Function not imported");
CZ_STR_appendPtr(err->secMessage, "Package ");
CZ_STR_appendPtr(err->secMessage, this->sources[i]->program->packageName->ptr);
CZ_STR_appendPtr(err->secMessage, " not imported");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:154:48\n");fprintf(stderr,"Package not imported");exit(1);
//somehow destruct: err 
;
};
;
};
;
};
int cur=100;
char* closest=0;
for(int i = 0; i < this->program->fSize; i++) {
int dist=levenshteinDistance(name, this->program->functions[i]->funName);
if (dist<cur) {
cur=dist;
closest=this->program->functions[i]->funName;
;
};
;
};
for(int i = 0; i < this->sSize; i++) {
for(int j = 0; j < this->sources[i]->program->fSize; j++) {
int dist=levenshteinDistance(name, this->sources[i]->program->functions[j]->funName);
if (dist<cur) {
cur=dist;
closest=this->sources[i]->program->functions[j]->funName;
;
};
;
};
;
};
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, pos);
err->level=2;
CZ_STR_appendPtr(err->message, "Function not found");
CZ_STR_appendPtr(err->secMessage, "Did you mean ");
CZ_STR_appendPtr(err->secMessage, closest);
CZ_STR_appendPtr(err->secMessage, "?");
CZ_Error_print(err, true);
CZ_Error_destruct(err);
free(err);
//somehow destruct: err 
;
return false;
//somehow destruct: err 
;
}
FunctionNode* CZ_COMPILEPROCESS_getFunctionByName(COMPILEPROCESS* this, char* ptr) {
for(int i = 0; i < this->program->fSize; i++) {
if (ptrEqual(this->program->functions[i]->funName, ptr)) {
;
return this->program->functions[i];
;
};
;
};
for(int i = 0; i < this->sSize; i++) {
for(int j = 0; j < this->sources[i]->program->fSize; j++) {
if (ptrEqual(this->sources[i]->program->functions[j]->funName, ptr)) {
;
return this->sources[i]->program->functions[j];
;
};
;
};
;
};
fprintf(stderr,"src/compiler/compiler.cz:208:34\n");fprintf(stderr,"Function not found");exit(1);
;
}
bool CZ_COMPILEPROCESS_isStructDefined(COMPILEPROCESS* this, char* name, int pos) {
for(int i = 0; i < this->program->sSize; i++) {
if (ptrEqual(this->program->strcts[i]->name->ptr, name)) {
;
return true;
;
};
;
};
for(int i = 0; i < this->sSize; i++) {
for(int j = 0; j < this->sources[i]->program->sSize; j++) {
if (ptrEqual(this->sources[i]->program->strcts[j]->name->ptr, name)) {
if (CZ_COMPILEPROCESS_isPackageImported(this, this->sources[i]->program->packageName->ptr)) {
;
return true;
;
};
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, pos);
CZ_STR_appendPtr(err->message, "Struct not imported");
CZ_STR_appendPtr(err->secMessage, "Package ");
CZ_STR_appendPtr(err->secMessage, this->sources[i]->program->packageName->ptr);
CZ_STR_appendPtr(err->secMessage, " not imported");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:231:48\n");fprintf(stderr,"Package not imported");exit(1);
//somehow destruct: err 
;
};
;
};
;
};
;
return false;
;
}
STRCTENTRY* CZ_COMPILEPROCESS_getStructByName(COMPILEPROCESS* this, char* ptr) {
for(int i = 0; i < this->program->sSize; i++) {
if (ptrEqual(this->program->strcts[i]->name->ptr, ptr)) {
;
return this->program->strcts[i];
;
};
;
};
for(int i = 0; i < this->sSize; i++) {
for(int j = 0; j < this->sources[i]->program->sSize; j++) {
if (ptrEqual(this->sources[i]->program->strcts[j]->name->ptr, ptr)) {
;
return this->sources[i]->program->strcts[j];
;
};
;
};
;
};
fprintf(stderr,"src/compiler/compiler.cz:253:32\n");fprintf(stderr,"Struct not found");exit(1);
;
}
char* CZ_COMPILEPROCESS_getClosestStructByName(COMPILEPROCESS* this, char* ptr) {
int cur=100;
char* closest=0;
for(int i = 0; i < this->program->sSize; i++) {
int dist=levenshteinDistance(ptr, this->program->strcts[i]->name->ptr);
if (dist<cur) {
cur=dist;
closest=this->program->strcts[i]->name->ptr;
;
};
;
};
for(int i = 0; i < this->sSize; i++) {
for(int j = 0; j < this->sources[i]->program->sSize; j++) {
int dist=levenshteinDistance(ptr, this->sources[i]->program->strcts[j]->name->ptr);
if (dist<cur) {
cur=dist;
closest=this->sources[i]->program->strcts[j]->name->ptr;
;
};
;
};
;
};
;
return closest;
;
}
bool CZ_COMPILEPROCESS_isEnumDefined(COMPILEPROCESS* this, TOKEN* token, int pos) {
for(int i = 0; i < this->program->eSize; i++) {
if (ptrEqual(this->program->enums[i]->name->ptr, token->value)) {
;
return true;
;
};
;
};
for(int i = 0; i < this->sSize; i++) {
for(int j = 0; j < this->sources[i]->program->eSize; j++) {
if (ptrEqual(this->sources[i]->program->enums[j]->name->ptr, token->value)) {
if (CZ_COMPILEPROCESS_isPackageImported(this, this->sources[i]->program->packageName->ptr)) {
;
return true;
;
};
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, pos);
CZ_STR_appendPtr(err->message, "Enum not imported");
CZ_STR_appendPtr(err->secMessage, "Package ");
CZ_STR_appendPtr(err->secMessage, this->sources[i]->program->packageName->ptr);
CZ_STR_appendPtr(err->secMessage, " not imported");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:299:48\n");fprintf(stderr,"Package not imported");exit(1);
//somehow destruct: err 
;
};
;
};
;
};
;
return false;
;
}
ENUM* CZ_COMPILEPROCESS_getEnumByName(COMPILEPROCESS* this, char* ptr) {
for(int i = 0; i < this->program->eSize; i++) {
if (ptrEqual(this->program->enums[i]->name->ptr, ptr)) {
;
return this->program->enums[i];
;
};
;
};
for(int i = 0; i < this->sSize; i++) {
for(int j = 0; j < this->sources[i]->program->eSize; j++) {
if (ptrEqual(this->sources[i]->program->enums[j]->name->ptr, ptr)) {
;
return this->sources[i]->program->enums[j];
;
};
;
};
;
};
fprintf(stderr,"src/compiler/compiler.cz:322:30\n");fprintf(stderr,"Enum not found");exit(1);
;
}
bool CZ_COMPILEPROCESS_isTypeDefined(COMPILEPROCESS* this, char* name) {
for(int i = 0; i < this->dSize; i++) {
if (ptrEqual(this->defs[i]->name->ptr, name)) {
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
TYPE* CZ_COMPILEPROCESS_geTypeByName(COMPILEPROCESS* this, char* name) {
for(int i = 0; i < this->dSize; i++) {
if (ptrEqual(this->defs[i]->name->ptr, name)) {
;
return this->defs[i]->type;
;
};
;
};
fprintf(stderr,"src/compiler/compiler.cz:340:30\n");fprintf(stderr,"Type not found");exit(1);
;
}
TYPE* CZ_COMPILEPROCESS_deduceType(COMPILEPROCESS* this, NODE* ast, SCOPE* scope) {
NODE** childs;
childs = (NODE**)ast->children;
TOKEN** tokens=this->tokens;
if (ast->type==1) {
if (ast->children[0]->type==19) {
if (CZ_COMPILEPROCESS_isEnumDefined(this, tokens[ast->children[0]->tokensStart], ast->tokensStart)) {
ENUM* en=CZ_COMPILEPROCESS_getEnumByName(this, tokens[ast->children[0]->tokensStart]->value);
for(int i = 0; i < en->size; i++) {
if (ptrEqual(en->values[i]->name->ptr, tokens[ast->children[1]->tokensStart]->value)) {
TYPE* type=malloc(sizeof(TYPE));
type->variant=3;
;
return type;
;
};
;
};
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->children[1]->tokensStart);
CZ_STR_appendPtr(err->message, "Enum doesnt contain value ");
CZ_STR_appendPtr(err->message, tokens[ast->children[1]->tokensStart]->value);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:364:53\n");fprintf(stderr,"Enum doesnt contain value");exit(1);
//somehow destruct: err 
;
};
;
};
TYPE* strType=CZ_COMPILEPROCESS_deduceType(this, childs[0], scope);
char* namePtr=strType->name->ptr;
if (strType->variant!=6) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Not a struct ");
CZ_STR_appendPtr(err->message, namePtr);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:377:36\n");fprintf(stderr,"Not a struct");exit(1);
//somehow destruct: err 
;
};
bool isDefined=CZ_COMPILEPROCESS_isStructDefined(this, namePtr, ast->tokensStart);
if (isDefined==false) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Undefined struct ");
CZ_STR_appendPtr(err->message, namePtr);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:386:40\n");fprintf(stderr,"Undefined struct");exit(1);
//somehow destruct: err 
;
};
STRCTENTRY* strct=CZ_COMPILEPROCESS_getStructByName(this, namePtr);
char* propName=tokens[childs[1]->tokensStart]->value;
if (CZ_STRCTENTRY_containsProp(strct, propName)) {
TYPE* type=CZ_STRCTENTRY_getPropertyType(strct, propName);
//somehow destruct: strType 
;
return CZ_TYPE_copy(type);
;
};
CZ_COMPILEPROCESS_printScope(this);
czStdOut = stdout;
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Property not found ");
CZ_STR_appendPtr(err->message, namePtr);
CZ_STR_appendPtr(err->message, ".");
CZ_STR_appendPtr(err->message, propName);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:403:38\n");fprintf(stderr,"Property not found");exit(1);
//somehow destruct: err strType 
;
};
if (ast->type==2) {
TYPE* arr=CZ_COMPILEPROCESS_deduceType(this, childs[0], scope);
if (arr->variant!=5) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Not a pointer ");
CZ_STR_appendPtr(err->message, arr->name->ptr);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:417:36\n");fprintf(stderr,"Not an array");exit(1);
//somehow destruct: err 
;
};
//somehow destruct: arr 
;
return CZ_TYPE_copy(arr->dereference);
//somehow destruct: arr 
;
};
if (ast->type==19) {
TOKEN* token=tokens[ast->tokensStart];
STR* strname=toSTR(token->value);
if ((CZ_SCOPE_isDefined(scope, strname))==false) {
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Undefined variable ");
CZ_STR_appendPtr(err->message, strname->ptr);
char* closest=CZ_SCOPE_getClosestVariable(scope, strname->ptr, 100);
CZ_STR_appendPtr(err->secMessage, "Did you mean ");
CZ_STR_appendPtr(err->secMessage, closest);
CZ_STR_appendPtr(err->secMessage, "?");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:438:57\n");fprintf(stderr,"Undefined variable enable deducer");exit(1);
//somehow destruct: err 
;
};
TYPE* type=CZ_SCOPE_getType(scope, strname);
//somehow destruct: strname 
;
return CZ_TYPE_copy(type);
//somehow destruct: strname 
;
};
if (ast->type==23) {
if (ast->size==1) {
TYPE* type=generateType(childs[0], this->tokens);
type->isMemMove=true;
;
return type;
;
}
else {
TYPE* type=generateType(childs[0], this->tokens);
TYPE* pointerType=malloc(sizeof(TYPE));
pointerType->variant=5;
pointerType->dereference=type;
pointerType->isMemMove=true;
;
return pointerType;
;
};
;
};
if (ast->type==18) {
TOKEN* token=tokens[ast->tokensStart];
if (token->type==28) {
TYPE* type=malloc(sizeof(TYPE));
type->variant=5;
type->dereference=malloc(sizeof(TYPE));
type->dereference->variant=2;
;
return type;
;
}
else if (token->type==29) {
TYPE* type=malloc(sizeof(TYPE));
type->variant=3;
;
return type;
;
}
else if (token->type==31||token->type==32) {
TYPE* type=malloc(sizeof(TYPE));
type->variant=1;
;
return type;
;
}
else if (token->type==30) {
TYPE* type=malloc(sizeof(TYPE));
type->variant=2;
;
return type;
;
};
fprintf(stderr,"src/compiler/compiler.cz:487:35\n");fprintf(stderr,"Unknown literal");exit(1);
;
};
if (ast->type==21) {
TOKEN* token=tokens[childs[0]->tokensStart];
if (childs[0]->type==19) {
if (ptrEqual(token->value, "typeof")) {
TYPE* type=malloc(sizeof(TYPE));
type->variant=5;
type->dereference=malloc(sizeof(TYPE));
type->dereference->variant=2;
;
return type;
;
}
else if (ptrEqual(token->value, "sizeof")) {
TYPE* type=malloc(sizeof(TYPE));
type->variant=3;
;
return type;
;
}
else if ((ptrEqual(token->value, "printf"))||(ptrEqual(token->value, "print"))||(ptrEqual(token->value, "println"))||(ptrEqual(token->value, "throw"))||(ptrEqual(token->value, "inC"))) {
TYPE* typ=malloc(sizeof(TYPE));
typ->variant=0;
;
return typ;
;
}
else if (ptrEqual(token->value, "hodnota")) {
TYPE* typ=CZ_COMPILEPROCESS_deduceType(this, childs[1], scope);
if (typ->variant!=5) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, childs[1]->tokensStart);
CZ_STR_appendPtr(err->message, "Not a pointer ");
CZ_STR_appendPtr(err->message, typ->name->ptr);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:519:45\n");fprintf(stderr,"Not a pointer");exit(1);
//somehow destruct: err 
;
};
;
return typ;
;
}
else {
if (CZ_COMPILEPROCESS_isFunctionDefined(this, token->value, childs[0]->tokensStart)) {
FunctionNode* func=CZ_COMPILEPROCESS_getFunctionByName(this, token->value);
;
return CZ_TYPE_copy(func->returnType);
;
}
else {
Error* err=CZ_COMPILEPROCESS_generateError(this, childs[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Undefined function ");
CZ_STR_appendPtr(err->message, token->value);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:534:48\n");fprintf(stderr,"Unknown function");exit(1);
//somehow destruct: err 
;
};
;
};
;
}
else if (childs[0]->type==1) {
NODE** accessChilds;
accessChilds = (NODE**)childs[0]->children;
TYPE* strctType=CZ_COMPILEPROCESS_deduceType(this, accessChilds[0], scope);
if (strctType->variant!=6) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Not a struct ");
CZ_STR_appendPtr(err->message, strctType->name->ptr);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:548:40\n");fprintf(stderr,"Not a struct");exit(1);
//somehow destruct: err 
;
};
if (CZ_COMPILEPROCESS_isStructDefined(this, strctType->name->ptr, ast->tokensStart)) {
STRCTENTRY* strct=CZ_COMPILEPROCESS_getStructByName(this, strctType->name->ptr);
if (CZ_STRCTENTRY_containsFun(strct, tokens[accessChilds[1]->tokensStart]->value)) {
TYPE* type=CZ_STRCTENTRY_getFunctionReturnType(strct, tokens[accessChilds[1]->tokensStart]->value);
//somehow destruct: strctType 
;
return CZ_TYPE_copy(type);
;
}
else {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, accessChilds[1]->tokensStart);
CZ_STR_appendPtr(err->message, "Struct doesnt contain function ");
CZ_STR_appendPtr(err->message, tokens[accessChilds[1]->tokensStart]->value);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:562:62\n");fprintf(stderr,"Struct doesnt contain function");exit(1);
//somehow destruct: err 
;
};
;
}
else {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Struct not defined ");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:570:46\n");fprintf(stderr,"Struct not defined");exit(1);
//somehow destruct: err 
;
};
//somehow destruct: strctType 
;
};
;
};
if (ast->type==20) {
TOKEN* token=tokens[childs[0]->tokensStart];
STR* str=toSTR(token->value);
if (CZ_SCOPE_isDefined(scope, str)) {
TYPE* type=CZ_SCOPE_getType(scope, str);
//somehow destruct: str 
;
return CZ_TYPE_copy(type);
;
};
//somehow destruct: str 
;
return generateType(childs[1], tokens);
//somehow destruct: str 
;
};
if (ast->type==22) {
;
return generateType(childs[1], tokens);
;
};
if (ast->type==12) {
TYPE* type=malloc(sizeof(TYPE));
type->variant=1;
;
return type;
;
};
if (ast->type==15) {
TYPE* child0Type=CZ_COMPILEPROCESS_deduceType(this, childs[0], scope);
TYPE* child1Type=CZ_COMPILEPROCESS_deduceType(this, childs[1], scope);
if (child0Type->variant==5&&child1Type->variant==5) {
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Pointer aritmethic not allowed");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:607:54\n");fprintf(stderr,"Pointer aritmethic not allowed");exit(1);
//somehow destruct: err 
;
};
if (child0Type->variant==5&&child1Type->variant==3) {
//somehow destruct: child1Type 
;
return child0Type;
;
};
if (child1Type->variant==5&&child0Type->variant==3) {
//somehow destruct: child0Type 
;
return child1Type;
;
};
//somehow destruct: child1Type child0Type 
;
};
if (ast->type==17) {
if (ast->operand==3) {
TYPE* type=CZ_COMPILEPROCESS_deduceType(this, childs[0], scope);
type->isMemMove=true;
;
return type;
;
};
;
return CZ_COMPILEPROCESS_deduceType(this, childs[0], scope);
;
};
if (ast->type==16) {
TYPE* child0Type=CZ_COMPILEPROCESS_deduceType(this, childs[0], scope);
TYPE* child1Type=CZ_COMPILEPROCESS_deduceType(this, childs[1], scope);
if (child0Type->variant==5||child1Type->variant==5) {
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Pointer aritmethic not allowed");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:636:54\n");fprintf(stderr,"Pointer aritmethic not allowed");exit(1);
//somehow destruct: err 
;
};
if (child0Type->variant>child1Type->variant) {
//somehow destruct: child1Type 
;
return child0Type;
;
};
//somehow destruct: child0Type 
;
return child1Type;
//somehow destruct: child0Type 
;
};
if (ast->type==15) {
TYPE* child0Type=CZ_COMPILEPROCESS_deduceType(this, childs[0], scope);
TYPE* child1Type=CZ_COMPILEPROCESS_deduceType(this, childs[1], scope);
if (child0Type->variant==5&&child1Type->variant==5) {
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Pointer aritmethic not allowed");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:653:54\n");fprintf(stderr,"Pointer aritmethic not allowed");exit(1);
//somehow destruct: err 
;
};
if (child0Type->variant==5&&child1Type->variant==3) {
//somehow destruct: child1Type 
;
return child0Type;
;
};
if (child1Type->variant==5&&child0Type->variant==3) {
//somehow destruct: child0Type 
;
return child1Type;
;
};
if (child0Type->variant==5||child1Type->variant==5) {
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Pointer aritmethic not allowed");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:666:54\n");fprintf(stderr,"Pointer aritmethic not allowed");exit(1);
//somehow destruct: err 
;
};
if (child0Type->variant>child1Type->variant) {
//somehow destruct: child1Type 
;
return child0Type;
;
};
//somehow destruct: child0Type 
;
return child1Type;
//somehow destruct: child0Type 
;
};
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "deduceType not implemented for ");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:681:35\n");fprintf(stderr,"Not implemented yet");exit(1);
//somehow destruct: err 
;
}
void CZ_COMPILEPROCESS_virtualPrint(COMPILEPROCESS* this, NODE* ast) {
TOKEN* token=this->tokens[ast->tokensStart];
if (ast->type==18) {
TOKEN* token=this->tokens[ast->tokensStart];
if (token->type==28) {
fprintf(czStdOut, "fprintf(czStdOut,%c%s%c)", 34, token->value, 34);
;
return;
;
}
else if (token->type==29) {
fprintf(czStdOut, "fprintf(czStdOut%c%%d%c, %d)", 34, 34, token->nValue);
;
return;
;
}
else if (token->type==31) {
fprintf(czStdOut, "fprintf(czStdOut,%c%%s%c, %ctrue%c)", 34, 34, 34, 34);
;
return;
;
}
else if (token->type==32) {
fprintf(czStdOut, "fprintf(czStdOut,%c%%s%c, %cfalse%c)", 34, 34, 34, 34);
;
return;
;
}
else if (token->type==30) {
fprintf(czStdOut, "fprintf(czStdOut,%c%%c%c, %d)", 34, 34, token->nValue);
;
return;
;
};
fprintf(stderr,"src/compiler/compiler.cz:710:35\n");fprintf(stderr,"Unknown literal");exit(1);
;
};
TYPE* typ=CZ_COMPILEPROCESS_deduceType(this, ast, this->scope);
if (typ->variant==3) {
fprintf(czStdOut, "fprintf(czStdOut,%c%%d%c, ", 34, 34);
CZ_COMPILEPROCESS_toCAst(this, ast);
fprintf(czStdOut, ")");
;
}
else if (typ->variant==2) {
fprintf(czStdOut, "fprintf(czStdOut,%c%%c%c, ", 34, 34);
CZ_COMPILEPROCESS_toCAst(this, ast);
fprintf(czStdOut, ")");
;
}
else if (typ->variant==5) {
if (typ->dereference->variant==2) {
fprintf(czStdOut, "fprintf(czStdOut,%c%%s%c, ", 34, 34);
CZ_COMPILEPROCESS_toCAst(this, ast);
fprintf(czStdOut, ")");
//somehow destruct: typ 
;
return;
;
};
fprintf(czStdOut, "fprintf(czStdOut,%c%%p%c, ", 34, 34);
CZ_COMPILEPROCESS_toCAst(this, ast);
fprintf(czStdOut, ")");
;
}
else if (typ->variant==6) {
fprintf(czStdOut, "fprintf(czStdOut,%c%s%c)", 34, typ->name->ptr, 34);
;
}
else {
fprintf(stderr,"src/compiler/compiler.cz:739:32\n");fprintf(stderr,"Unknown type");exit(1);
;
};
//somehow destruct: typ 
;
}
void CZ_COMPILEPROCESS_printScope(COMPILEPROCESS* this) {
void* old;
old = czStdOut;
czStdOut = stderr;
SCOPE* scope=this->scope;
while (scope!=0) {
for(int i = 0; i < scope->size; i++) {
fprintf(czStdOut, "%s:", scope->vars[i]->name->ptr);
CZ_TYPE_printType(scope->vars[i]->typ);
fprintf(czStdOut, ", ");
;
};
fprintf(czStdOut, "\n");
scope=scope->parent;
;
};
czStdOut = old;
;
}
void CZ_COMPILEPROCESS_compile(COMPILEPROCESS* this, char* buildDir) {
if (this->program==0) {
;
return;
;
};
this->scope=genererateGlobalScope();
FILEWRITER* writer=malloc(sizeof(FILEWRITER));
STR* name=toSTR(buildDir);
if (name->ptr[name->size-1]!='/') {
CZ_STR_appendPtr(name, "/");
;
};
int size=name->size;
if (this->program->packageName!=0) {
CZ_STR_appendPtr(name, this->program->packageName->ptr);
;
}
else {
CZ_STR_appendPtr(name, "main");
;
};
for(int i = size; i < name->size; i++) {
if (name->ptr[i]==':'||name->ptr[i]=='.'||name->ptr[i]=='/'||name->ptr[i]==92||name->ptr[i]==' ') {
name->ptr[i]='_';
;
};
;
};
CZ_STR_appendPtr(name, ".c");
CZ_FILEWRITER_init(writer, name->ptr);
czStdOut = writer->stream;
name->ptr[name->size-1]=0;
name->size-=1;
name->ptr[name->size-1]=0;
name->size-=1;
if (this->program->packageName!=0) {
fprintf(czStdOut, "#ifndef CZECHTINA_%s\n", name->ptr+size);
fprintf(czStdOut, "#define CZECHTINA_%s\n", name->ptr+size);
;
};
fprintf(czStdOut, "#include <stdio.h>\n");
fprintf(czStdOut, "#include <stdlib.h>\n");
fprintf(czStdOut, "#include <stdbool.h>\n");
fprintf(czStdOut, "#include <string.h>\n");
for(int i = 0; i < this->program->iSize; i++) {
char* ptr=this->program->imports[i]->ptr;
if (ptr[0]=='c'&&ptr[1]==':') {
ptr+=3;
fprintf(czStdOut,"#include <");
while (*ptr) {
if ((*ptr)==':') {
fprintf(czStdOut,"/");
ptr+=2;
;
}
else {
fprintf(czStdOut, "%c", (*ptr));
ptr+=1;
;
};
;
};
fprintf(czStdOut,".h>\n");
;
}
else {
fprintf(czStdOut, "#include \"");
while (*ptr) {
if (*ptr==':') {
fprintf(czStdOut, "_");
;
}
else {
fprintf(czStdOut, "%c", (*ptr));
;
};
ptr+=1;
;
};
fprintf(czStdOut, ".c\"\n");
;
};
;
};
if (this->program->packageName!=0) {
preludeDeclare(0);
;
}
else {
preludeDefine(0);
;
};
for(int i = 0; i < this->program->sSize; i++) {
STRCTENTRY* strct=this->program->strcts[i];
fprintf(czStdOut, "typedef struct _%s %s;\n", strct->name->ptr, strct->name->ptr);
;
};
for(int i = 0; i < this->program->fSize; i++) {
CZ_COMPILEPROCESS_funToCAst(this, this->program->functions[i], true, false);
;
};
for(int i = 0; i < this->program->sSize; i++) {
STRCTENTRY* strct=this->program->strcts[i];
CZ_COMPILEPROCESS_strctPrintf(this, strct, true);
;
};
for(int i = 0; i < this->program->fSize; i++) {
CZ_COMPILEPROCESS_funToCAst(this, this->program->functions[i], false, false);
;
};
for(int i = 0; i < this->program->sSize; i++) {
STRCTENTRY* strct=this->program->strcts[i];
CZ_COMPILEPROCESS_strctPrintf(this, strct, false);
;
};
if (this->program->main!=0) {
CZ_COMPILEPROCESS_funToCAst(this, this->program->main, false, true);
;
};
if (this->program->packageName!=0) {
fprintf(czStdOut, "#endif\n");
;
};
czStdOut = stdout;
CZ_FILEWRITER_close(writer);
if (this->program->packageName!=0) {
fprintf(czStdOut,"compiled: \033[92m["); fprintf(czStdOut,"%s", this->program->packageName->ptr); fprintf(czStdOut,"]\033[39m"); fputs("\n",czStdOut);
;
};
//somehow destruct: name writer 
;
}
void CZ_COMPILEPROCESS_toCAst(COMPILEPROCESS* this, NODE* ast) {
if (ast==0) {
;
return;
;
};
NODE** childs;
childs = (NODE**)ast->children;
TOKEN** tokens=this->tokens;
if (ast->type==18) {
TOKEN* token=this->tokens[ast->tokensStart];
if (token->type==28) {
fprintf(czStdOut, "%c%s%c", 34, token->value, 34);
;
}
else if (token->type==29) {
fprintf(czStdOut,"%d", token->nValue);
;
}
else if (token->type==32) {
fprintf(czStdOut,"false");
;
}
else if (token->type==31) {
fprintf(czStdOut,"true");
;
}
else if (token->type==30) {
if (token->nValue==92) {
fprintf(czStdOut, "'%c%c'", 92, 92);
;
}
else if (token->nValue==34) {
fprintf(czStdOut, "'%c%c'", 92, 34);
;
}
else if (token->nValue==10) {
fprintf(czStdOut, "'%cn'", 92);
;
}
else if (token->nValue==9) {
fprintf(czStdOut, "'%ct'", 92);
;
}
else if (token->nValue==13) {
fprintf(czStdOut, "'%cr'", 92);
;
}
else if (token->nValue==0) {
fprintf(czStdOut, "'%c0'", 92);
;
}
else if (token->nValue==39) {
fprintf(czStdOut, "'%c%c'", 92, 39);
;
}
else {
fprintf(czStdOut, "'%c'", token->nValue);
;
};
;
};
;
}
else if (ast->type==19) {
TOKEN* token=tokens[ast->tokensStart];
STR* strname=toSTR(token->value);
if ((CZ_SCOPE_isDefined(this->scope, strname))==false) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Undefined variable ");
CZ_STR_appendPtr(err->message, strname->ptr);
char* closest=CZ_SCOPE_getClosestVariable(this->scope, strname->ptr, 100);
CZ_STR_appendPtr(err->secMessage, "Did you mean ");
CZ_STR_appendPtr(err->secMessage, closest);
CZ_STR_appendPtr(err->secMessage, "?");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:957:57\n");fprintf(stderr,"Undefined variable enable deducer");exit(1);
//somehow destruct: err 
;
};
fprintf(czStdOut, "%s", token->value);
//somehow destruct: strname 
;
}
else if (ast->type==16||ast->type==15||ast->type==11) {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
printAstOperandType(ast->operand);
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
;
}
else if (ast->type==12) {
if (childs[0]->type==12) {
fprintf(czStdOut, "(");
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
fprintf(czStdOut, "&&");
NODE** secChilds;
secChilds = (NODE**)childs[0]->children;
CZ_COMPILEPROCESS_toCAst(this, secChilds[1]);
printAstOperandType(ast->operand);
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
fprintf(czStdOut, ")");
;
return;
;
};
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
printAstOperandType(ast->operand);
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
;
}
else if (ast->type==2) {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
fprintf(czStdOut, "[");
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
fprintf(czStdOut, "]");
;
}
else if (ast->type==1) {
char* name=tokens[childs[0]->tokensStart]->value;
char* secName=tokens[childs[1]->tokensStart]->value;
if (childs[0]->type==19) {
if (childs[1]->type==19) {
if (CZ_COMPILEPROCESS_isEnumDefined(this, tokens[childs[0]->tokensStart], ast->tokensStart)) {
ENUM* en=CZ_COMPILEPROCESS_getEnumByName(this, name);
for(int i = 0; i < en->size; i++) {
if (ptrEqual(en->values[i]->name->ptr, secName)) {
fprintf(czStdOut, "%d", en->values[i]->value);
en->values[i]->used+=1;
;
return;
;
};
;
};
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, childs[1]->tokensStart);
CZ_STR_appendPtr(err->message, "Enum doesnt contain value ");
CZ_STR_appendPtr(err->message, secName);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1010:57\n");fprintf(stderr,"Enum doesnt contain value");exit(1);
//somehow destruct: err 
;
};
;
};
;
};
TYPE* strctType=CZ_COMPILEPROCESS_deduceType(this, childs[0], this->scope);
if (strctType->variant!=6) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Type is not struct");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1021:43\n");fprintf(stderr,"undefined behaivior");exit(1);
//somehow destruct: err 
;
};
if (CZ_COMPILEPROCESS_isStructDefined(this, strctType->name->ptr, ast->tokensStart)) {
STRCTENTRY* strct=CZ_COMPILEPROCESS_getStructByName(this, strctType->name->ptr);
if (CZ_STRCTENTRY_containsProp(strct, secName)) {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
fprintf(czStdOut, "->%s", secName);
//somehow destruct: strctType 
;
return;
;
}
else if (CZ_STRCTENTRY_containsFun(strct, secName)) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, childs[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Struct doesnt contain property ");
CZ_STR_appendPtr(err->message, secName);
CZ_STR_appendPtr(err->secMessage, "Use `zavolej` or `call` to call function without params");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1037:58\n");fprintf(stderr,"Struct doesnt contain property");exit(1);
//somehow destruct: err 
;
}
else {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, childs[1]->tokensStart);
CZ_STR_appendPtr(err->message, "Struct doesnt contain property ");
CZ_STR_appendPtr(err->message, secName);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1045:58\n");fprintf(stderr,"Struct doesnt contain property");exit(1);
//somehow destruct: err 
;
};
;
}
else {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, childs[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Struct not defined ");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1053:42\n");fprintf(stderr,"Struct not defined");exit(1);
//somehow destruct: err 
;
};
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Struct not defined ");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1060:36\n");fprintf(stderr,"Unfinish compile");exit(1);
//somehow destruct: err strctType 
;
}
else if (ast->type==17) {
if (ast->operand==1) {
fprintf(czStdOut, "!");
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
;
}
else if (ast->operand==15) {
fprintf(czStdOut, "-");
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
;
}
else if (ast->operand==5) {
fprintf(czStdOut, "(");
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
fprintf(czStdOut, ")");
;
}
else if (ast->operand==3) {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
;
}
else {
fprintf(stderr,"src/compiler/compiler.cz:1083:45\n");fprintf(stderr,"Unknown unary operand");exit(1);
;
};
;
}
else if (ast->type==22) {
fprintf(czStdOut, "(");
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
fprintf(czStdOut, ")");
fprintf(czStdOut, "(");
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
fprintf(czStdOut, ")");
;
}
else if (ast->type==21) {
if (childs[0]->type==19) {
TOKEN* token=tokens[childs[0]->tokensStart];
if (ptrEqual(token->value, "inC")) {
TOKEN* strToken=tokens[childs[1]->tokensStart];
char* ptr=strToken->value;
while (*ptr) {
if (*ptr==92) {
ptr++;
if (*ptr==110) {
fprintf(czStdOut,""); fputs("\n",czStdOut);
;
}
else if (*ptr==34) {
fprintf(czStdOut, "%c", 34);
;
}
else if (*ptr==92) {
fprintf(czStdOut, "%c", 92);
;
}
else {
fprintf(czStdOut, "%c", (*ptr));
;
};
ptr++;
;
continue;
;
};
fprintf(czStdOut, "%c", (*ptr));
ptr++;
;
};
;
return;
;
}
else if (ptrEqual(token->value, "throw")) {
TOKEN* strToken=tokens[childs[1]->tokensStart];
fprintf(czStdOut, "fprintf(stderr,\"%s:%d:%d\\n\");", this->filePath->ptr, strToken->line, strToken->col);
fprintf(czStdOut, "fprintf(stderr,%c%s%c);exit(1)", 34, strToken->value, 34);
;
return;
;
}
else if (ptrEqual(token->value, "typeof")) {
TYPE* vardef=CZ_COMPILEPROCESS_deduceType(this, childs[1], this->scope);
fprintf(czStdOut, "%c", 34);
CZ_TYPE_printType(vardef);
fprintf(czStdOut, "%c", 34);
//somehow destruct: vardef 
;
return;
//somehow destruct: vardef 
;
}
else if (ptrEqual(token->value, "sizeof")) {
TOKEN* nextToken=tokens[childs[1]->tokensStart];
if (nextToken->type==27) {
fprintf(czStdOut, "sizeof(void*)");
;
return;
;
};
fprintf(czStdOut, "sizeof");
;
}
else if (ptrEqual(token->value, "printf")) {
fprintf(czStdOut, "fprintf(czStdOut, ");
for(int i = 1; i < ast->size; i++) {
CZ_COMPILEPROCESS_toCAst(this, childs[i]);
if (i<ast->size-1) {
fprintf(czStdOut, ", ");
;
};
;
};
fprintf(czStdOut, ")");
;
return;
;
}
else if (ptrEqual(token->value, "print")) {
for(int i = 1; i < ast->size; i++) {
CZ_COMPILEPROCESS_virtualPrint(this, childs[i]);
if (i<ast->size-1) {
fprintf(czStdOut, "; ");
;
};
;
};
;
return;
;
}
else if (ptrEqual(token->value, "hodnota")) {
fprintf(czStdOut, "*");
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
;
return;
;
}
else if (ptrEqual(token->value, "println")) {
for(int i = 1; i < ast->size; i++) {
CZ_COMPILEPROCESS_virtualPrint(this, childs[i]);
if (i<ast->size) {
fprintf(czStdOut, "; ");
;
};
;
};
fprintf(czStdOut, "fputs(%c%cn%c,czStdOut)", 34, 92, 34);
;
return;
;
}
else {
if (CZ_COMPILEPROCESS_isFunctionDefined(this, token->value, childs[0]->tokensStart)) {
FunctionNode* fun=CZ_COMPILEPROCESS_getFunctionByName(this, token->value);
if (fun->isDepricated) {
Error* err=CZ_COMPILEPROCESS_generateError(this, childs[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Using depricated function ");
CZ_STR_appendPtr(err->message, token->value);
CZ_STR_appendPtr(err->secMessage, " at this position");
err->level=1;
CZ_Error_print(err, false);
CZ_Error_destruct(err);
free(err);
//somehow destruct: err 
;
};
fprintf(czStdOut, "%s", token->value);
;
}
else {
Error* err=CZ_COMPILEPROCESS_generateError(this, childs[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Undefined function ");
CZ_STR_appendPtr(err->message, token->value);
CZ_STR_appendPtr(err->secMessage, " at this position");
err->level=3;
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1211:48\n");fprintf(stderr,"Unknown function");exit(1);
//somehow destruct: err 
;
};
;
};
;
}
else if (childs[0]->type==1) {
NODE** accessChilds;
accessChilds = (NODE**)childs[0]->children;
char* name=tokens[accessChilds[0]->tokensStart]->value;
char* secName=tokens[accessChilds[1]->tokensStart]->value;
if (accessChilds[0]->type==19) {
if (accessChilds[1]->type==19) {
STR* strname=toSTR(name);
if ((CZ_SCOPE_isDefined(this->scope, strname))==false) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, accessChilds[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Undefined variable ");
CZ_STR_appendPtr(err->message, strname->ptr);
char* closest=CZ_SCOPE_getClosestVariable(this->scope, strname->ptr, 100);
CZ_STR_appendPtr(err->secMessage, "Did you mean ");
CZ_STR_appendPtr(err->secMessage, closest);
CZ_STR_appendPtr(err->secMessage, "?");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1236:69\n");fprintf(stderr,"Undefined variable enable deducer");exit(1);
//somehow destruct: err 
;
};
TYPE* vardef=CZ_SCOPE_getType(this->scope, strname);
if (vardef->variant!=6) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, accessChilds[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Type is not struct ");
CZ_STR_appendPtr(err->message, strname->ptr);
CZ_STR_appendPtr(err->secMessage, "Type ");
CZ_STR_appendPtr(err->secMessage, vardef->name->ptr);
CZ_STR_appendPtr(err->secMessage, " is not struct.");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1249:55\n");fprintf(stderr,"undefined behaivior");exit(1);
//somehow destruct: err 
;
};
if (CZ_COMPILEPROCESS_isStructDefined(this, vardef->name->ptr, ast->tokensStart)) {
STRCTENTRY* strct=CZ_COMPILEPROCESS_getStructByName(this, vardef->name->ptr);
if (ptrEqual(vardef->name->ptr, strct->name->ptr)) {
if (CZ_STRCTENTRY_containsFun(strct, secName)) {
fprintf(czStdOut, "CZ_%s_%s", strct->name->ptr, secName);
fprintf(czStdOut, "(%s", name);
if (ast->size>1) {
fprintf(czStdOut, ", ");
;
};
for(int i = 1; i < ast->size; i++) {
CZ_COMPILEPROCESS_toCAst(this, childs[i]);
if (i<ast->size-1) {
fprintf(czStdOut, ", ");
;
};
;
};
fprintf(czStdOut, ")");
//somehow destruct: strname 
;
return;
;
}
else {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, accessChilds[1]->tokensStart);
CZ_STR_appendPtr(err->message, "Struct doesnt contain function ");
CZ_STR_appendPtr(err->message, secName);
CZ_STR_appendPtr(err->secMessage, "Struct ");
CZ_STR_appendPtr(err->secMessage, strct->name->ptr);
CZ_STR_appendPtr(err->secMessage, " doesnt contain function.");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1280:74\n");fprintf(stderr,"Struct doesnt contain function");exit(1);
//somehow destruct: err 
;
};
;
};
;
};
//somehow destruct: strname 
;
};
;
};
TYPE* deducedType=CZ_COMPILEPROCESS_deduceType(this, accessChilds[0], this->scope);
if (deducedType->variant!=6) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, accessChilds[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Type is not struct ");
CZ_STR_appendPtr(err->message, deducedType->name->ptr);
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1297:47\n");fprintf(stderr,"undefined behaivior");exit(1);
//somehow destruct: err 
;
};
if (CZ_COMPILEPROCESS_isStructDefined(this, deducedType->name->ptr, ast->tokensStart)) {
STRCTENTRY* strct=CZ_COMPILEPROCESS_getStructByName(this, deducedType->name->ptr);
if (CZ_STRCTENTRY_containsFun(strct, secName)) {
fprintf(czStdOut, "CZ_%s_%s", strct->name->ptr, secName);
fprintf(czStdOut, "(");
CZ_COMPILEPROCESS_toCAst(this, accessChilds[0]);
if (ast->size>1) {
fprintf(czStdOut, ", ");
;
};
for(int i = 1; i < ast->size; i++) {
CZ_COMPILEPROCESS_toCAst(this, childs[i]);
if (i<ast->size-1) {
fprintf(czStdOut, ", ");
;
};
;
};
fprintf(czStdOut, ")");
//somehow destruct: deducedType 
;
return;
;
}
else {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, accessChilds[1]->tokensStart);
CZ_STR_appendPtr(err->message, "Struct doesnt contain function ");
CZ_STR_appendPtr(err->message, secName);
CZ_STR_appendPtr(err->secMessage, "Struct ");
CZ_STR_appendPtr(err->secMessage, strct->name->ptr);
CZ_STR_appendPtr(err->secMessage, " doesnt contain function.");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1328:62\n");fprintf(stderr,"Struct doesnt contain function");exit(1);
//somehow destruct: err 
;
};
;
}
else {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, accessChilds[0]->tokensStart);
CZ_STR_appendPtr(err->message, "Struct not defined ");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1336:46\n");fprintf(stderr,"Struct not defined");exit(1);
//somehow destruct: err 
;
};
//somehow destruct: deducedType 
;
}
else {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
;
};
fprintf(czStdOut, "(");
for(int i = 1; i < ast->size; i++) {
CZ_COMPILEPROCESS_toCAst(this, childs[i]);
if (i<ast->size-1) {
fprintf(czStdOut, ", ");
;
};
;
};
fprintf(czStdOut, ")");
;
}
else if (ast->type==10) {
for(int i = 0; i < ast->size; i++) {
CZ_COMPILEPROCESS_toCAst(this, childs[i]);
fprintf(czStdOut, ";\n");
;
};
;
}
else if (ast->type==13) {
if (this->enableDeducer==true) {
if (childs[0]->type==19) {
TOKEN* token=tokens[childs[0]->tokensStart];
STR* strname=toSTR(token->value);
if ((CZ_SCOPE_isDefined(this->scope, strname))==false) {
DEFINEDVARS* vardef=malloc(sizeof(DEFINEDVARS));
vardef->moved=false;
vardef->name=strname;
vardef->typ=CZ_COMPILEPROCESS_deduceType(this, childs[1], this->scope);
CZ_SCOPE_define(this->scope, vardef);
CZ_TYPE_toC(vardef->typ);
fprintf(czStdOut, " ");
fprintf(czStdOut, "%s", token->value);
;
}
else {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
;
};
//somehow destruct: strname 
;
}
else {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
;
};
;
}
else {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
;
};
printAstOperandType(ast->operand);
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
;
}
else if (ast->type==14) {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
printAstOperandType(ast->operand);
;
}
else if (ast->type==20) {
TOKEN* token=tokens[childs[0]->tokensStart];
STR* strname=toSTR(token->value);
DEFINEDVARS* vardef=malloc(sizeof(DEFINEDVARS));
vardef->moved=false;
vardef->name=strname;
vardef->typ=generateType(childs[1], tokens);
CZ_SCOPE_define(this->scope, vardef);
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
fprintf(czStdOut, " ");
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
;
}
else if (ast->type==3) {
TOKEN* token=tokens[ast->tokensStart];
if (token->type==58) {
fprintf(czStdOut, "int");
;
}
else if (token->type==60) {
fprintf(czStdOut, "void");
;
}
else if (token->type==62) {
fprintf(czStdOut, "bool");
;
}
else if (token->type==61) {
fprintf(czStdOut, "float");
;
}
else if (token->type==59) {
fprintf(czStdOut, "char");
;
}
else if (token->type==63) {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
fprintf(czStdOut, "*");
;
}
else if (token->type==5) {
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
;
}
else if (token->type==27) {
if (CZ_COMPILEPROCESS_isTypeDefined(this, token->value)) {
TYPE* typ=CZ_COMPILEPROCESS_geTypeByName(this, token->value);
CZ_TYPE_toC(typ);
;
return;
;
};
bool exist=CZ_COMPILEPROCESS_isStructDefined(this, token->value, ast->tokensStart);
if (exist==false) {
CZ_COMPILEPROCESS_printScope(this);
Error* err=CZ_COMPILEPROCESS_generateError(this, ast->tokensStart);
CZ_STR_appendPtr(err->message, "Undefined struct ");
CZ_STR_appendPtr(err->message, token->value);
char* closest=CZ_COMPILEPROCESS_getClosestStructByName(this, token->value);
CZ_STR_appendPtr(err->secMessage, "Did you mean ");
CZ_STR_appendPtr(err->secMessage, closest);
CZ_STR_appendPtr(err->secMessage, "?");
CZ_Error_print(err, true);
fprintf(stderr,"src/compiler/compiler.cz:1458:42\n");fprintf(stderr,"Undefined type");exit(1);
//somehow destruct: err 
;
};
fprintf(czStdOut, "%s*", token->value);
;
}
else {
fprintf(stderr,"src/compiler/compiler.cz:1464:36\n");fprintf(stderr,"Unknown type");exit(1);
;
};
;
}
else if (ast->type==4) {
TOKEN* token=tokens[ast->tokensStart];
printTokenType(token->type);
if (ast->size>0) {
fprintf(czStdOut, " ");
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
;
};
;
}
else if (ast->type==9) {
fprintf(czStdOut, "if (");
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
fprintf(czStdOut, ") {\n");
CZ_COMPILEPROCESS_scopePush(this);
NODE* firstChild=childs[1];
CZ_COMPILEPROCESS_toCAst(this, firstChild);
if (firstChild->type!=10) {
fprintf(czStdOut, ";\n");
;
};
CZ_COMPILEPROCESS_scopePop(this);
fprintf(czStdOut, "}");
int pos=2;
while (pos<ast->size-1) {
fprintf(czStdOut, "\nelse if (");
CZ_COMPILEPROCESS_toCAst(this, childs[pos]);
fprintf(czStdOut, ") {\n");
CZ_COMPILEPROCESS_scopePush(this);
CZ_COMPILEPROCESS_toCAst(this, childs[pos+1]);
if (childs[pos+1]->type!=10) {
fprintf(czStdOut, ";\n");
;
};
CZ_COMPILEPROCESS_scopePop(this);
fprintf(czStdOut, "}");
pos+=2;
;
};
if (pos<ast->size) {
fprintf(czStdOut, "\nelse {\n");
CZ_COMPILEPROCESS_scopePush(this);
CZ_COMPILEPROCESS_toCAst(this, childs[pos]);
if (childs[pos]->type!=10) {
fprintf(czStdOut, ";\n");
;
};
CZ_COMPILEPROCESS_scopePop(this);
fprintf(czStdOut, "}");
;
};
;
}
else if (ast->type==8) {
fprintf(czStdOut, "while (");
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
fprintf(czStdOut, ") {\n");
CZ_COMPILEPROCESS_scopePush(this);
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
if (childs[1]->type!=10) {
fprintf(czStdOut, ";\n");
;
};
CZ_COMPILEPROCESS_scopePop(this);
fprintf(czStdOut, "}");
;
}
else if (ast->type==7) {
fprintf(czStdOut, "for (");
CZ_COMPILEPROCESS_scopePush(this);
CZ_COMPILEPROCESS_toCAst(this, childs[0]);
fprintf(czStdOut, ";");
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
fprintf(czStdOut, ";");
CZ_COMPILEPROCESS_toCAst(this, childs[2]);
fprintf(czStdOut, ") {\n");
CZ_COMPILEPROCESS_toCAst(this, childs[3]);
CZ_COMPILEPROCESS_scopePop(this);
fprintf(czStdOut, "}");
;
}
else if (ast->type==6) {
NODE* varDec=childs[0];
char* varName=tokens[varDec->tokensStart]->value;
NODE* range=childs[1];
NODE** rangeChild;
rangeChild = (NODE**)range->children;
NODE* rangeStart=rangeChild[0];
NODE* rangeEnd=rangeChild[1];
fprintf(czStdOut, "for(");
CZ_COMPILEPROCESS_scopePush(this);
CZ_COMPILEPROCESS_toCAst(this, varDec);
fprintf(czStdOut, " = ");
CZ_COMPILEPROCESS_toCAst(this, rangeStart);
fprintf(czStdOut, "; ");
fprintf(czStdOut, "%s < ", varName);
CZ_COMPILEPROCESS_toCAst(this, rangeEnd);
fprintf(czStdOut, "; %s++) {\n", varName);
CZ_COMPILEPROCESS_toCAst(this, childs[2]);
CZ_COMPILEPROCESS_scopePop(this);
fprintf(czStdOut, "}");
;
}
else if (ast->type==23) {
TYPE* type=generateType(childs[0], tokens);
if (ast->size==1) {
fprintf(czStdOut, "malloc(sizeof(");
if (type->variant==6) {
fprintf(czStdOut, "%s", type->name->ptr);
;
}
else {
CZ_TYPE_toC(type);
;
};
fprintf(czStdOut, "))");
;
}
else {
fprintf(czStdOut, "malloc(sizeof(");
if (type->variant==6) {
fprintf(czStdOut, "%s*", type->name->ptr);
;
}
else {
CZ_TYPE_toC(type);
fprintf(czStdOut, "*");
;
};
fprintf(czStdOut, ") *(");
CZ_COMPILEPROCESS_toCAst(this, childs[1]);
fprintf(czStdOut, "))");
;
};
//somehow destruct: type 
;
}
else if (ast->type==24) {
if (ast->size==0) {
;
return;
;
};
fprintf(czStdOut, "//somehow destruct: ");
for(int i = 0; i < ast->size; i++) {
fprintf(czStdOut, "%s ", ast->strs[i]->ptr);
;
};
fprintf(czStdOut, "\n");
;
}
else {
fprintf(stderr,"%c%d%c  ", 34,ast->type, 34);
fprintf(stderr,"src/compiler/compiler.cz:1600:36\n");fprintf(stderr,"Unknown ast type");exit(1);
;
};
;
}
void CZ_COMPILEPROCESS_funToCAst(COMPILEPROCESS* this, FunctionNode* ast, bool declr, bool isMain) {
NODE** childs;
childs = (NODE**)ast->ast->children;
CZ_COMPILEPROCESS_toCAst(this, childs[ast->ast->size-2]);
fprintf(czStdOut, " ");
TOKEN* token=this->tokens[ast->ast->tokensStart];
fprintf(czStdOut, "%s", token->value);
fprintf(czStdOut, "(");
CZ_COMPILEPROCESS_scopePush(this);
int pos=0;
while (pos<ast->ast->size-2) {
CZ_COMPILEPROCESS_toCAst(this, childs[pos]);
if (pos<ast->ast->size-3) {
fprintf(czStdOut, ", ");
;
};
pos+=1;
;
};
if (declr) {
fprintf(czStdOut, ");\n");
CZ_COMPILEPROCESS_scopePop(this);
;
return;
;
};
fprintf(czStdOut, ") {\n");
if (isMain) {
fprintf(czStdOut, "czStdOut = stdout;\n");
fprintf(czStdOut, "czStdIn = stdin;\n");
;
};
if (this->debug) {
fprintf(czStdOut, "printf(\"%s\\n\");\n", token->value);
;
};
CZ_COMPILEPROCESS_toCAst(this, childs[ast->ast->size-1]);
CZ_COMPILEPROCESS_scopePop(this);
fprintf(czStdOut, "}\n");
;
}
void CZ_COMPILEPROCESS_strctfunToCAst(COMPILEPROCESS* this, FunctionNode* ast, bool declr, STR* strctName) {
NODE** childs;
childs = (NODE**)ast->ast->children;
CZ_COMPILEPROCESS_toCAst(this, childs[ast->ast->size-2]);
fprintf(czStdOut, " ");
TOKEN* token=this->tokens[ast->ast->tokensStart];
fprintf(czStdOut, "CZ_%s_%s", strctName->ptr, token->value);
fprintf(czStdOut, "(");
CZ_COMPILEPROCESS_scopePush(this);
STR* thisStr=toSTR("this");
TYPE* thisType=malloc(sizeof(TYPE));
thisType->name=toSTR(strctName->ptr);
thisType->variant=6;
DEFINEDVARS* thisVar=malloc(sizeof(DEFINEDVARS));
thisVar->moved=false;
thisVar->name=thisStr;
thisVar->typ=thisType;
CZ_SCOPE_define(this->scope, thisVar);
fprintf(czStdOut, "%s* this", strctName->ptr);
if (ast->ast->size>2) {
fprintf(czStdOut, ", ");
;
};
int pos=0;
while (pos<ast->ast->size-2) {
CZ_COMPILEPROCESS_toCAst(this, childs[pos]);
if (pos<ast->ast->size-3) {
fprintf(czStdOut, ", ");
;
};
pos+=1;
;
};
if (declr) {
fprintf(czStdOut, ");\n");
CZ_COMPILEPROCESS_scopePop(this);
;
return;
;
};
fprintf(czStdOut, ") {\n");
if (this->debug) {
fprintf(czStdOut, "printf(\"CZ_%s_%s\\n\");\n", strctName->ptr, token->value);
;
};
CZ_COMPILEPROCESS_toCAst(this, childs[ast->ast->size-1]);
CZ_COMPILEPROCESS_scopePop(this);
fprintf(czStdOut, "}\n");
;
}
void CZ_COMPILEPROCESS_strctPrintf(COMPILEPROCESS* this, STRCTENTRY* strct, bool declr) {
if (declr) {
fprintf(czStdOut, "typedef struct _%s {\n", strct->name->ptr);
CZ_COMPILEPROCESS_scopePush(this);
for(int i = 0; i < strct->pSize; i++) {
CZ_TYPE_toC(strct->props[i]->type);
fprintf(czStdOut, " %s;\n", strct->props[i]->name->ptr);
;
};
CZ_COMPILEPROCESS_scopePop(this);
fprintf(czStdOut, "} ");
fprintf(czStdOut, "%s;\n", strct->name->ptr);
;
};
for(int j = 0; j < strct->fSize; j++) {
CZ_COMPILEPROCESS_strctfunToCAst(this, strct->funs[j]->ast, declr, strct->name);
;
};
;
}
void CZ_COMPILEPROCESS_printUnusedEnumsValue(COMPILEPROCESS* this) {
for(int i = 0; i < this->program->eSize; i++) {
ENUM* en=this->program->enums[i];
for(int j = 0; j < en->size; j++) {
if (en->values[j]->used==0) {
fprintf(czStdOut, "\033[33mWarning: Enum value %s in %s is not used\033[39m\n", en->values[j]->name->ptr, en->name->ptr);
;
};
;
};
;
};
;
}
#endif

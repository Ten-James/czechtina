#ifndef CZECHTINA_czechtina__compiler__functions
#define CZECHTINA_czechtina__compiler__functions
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__parser__ast.c"
#include "czechtina__compiler__types.c"
#include "czechtina__lexer__token.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _FunctionVariant FunctionVariant;
typedef struct _FunctionNode FunctionNode;
typedef struct _FunctionVariant {
TYPE** args;
int size;
} FunctionVariant;
typedef struct _FunctionNode {
char* funName;
TYPE* returnType;
NODE* ast;
bool isDepricated;
FunctionVariant** variants;
int variantsSize;
int variantsReserved;
} FunctionNode;
void CZ_FunctionNode_init(FunctionNode* this);
void CZ_FunctionNode_consume(FunctionNode* this, NODE* ast, char* funName, TOKEN** tokens);
void CZ_FunctionNode_init(FunctionNode* this) {
this->isDepricated=false;
this->variantsSize=0;
this->variantsReserved=1;
this->variants=malloc(sizeof(FunctionVariant*) *(1));
;
}
void CZ_FunctionNode_consume(FunctionNode* this, NODE* ast, char* funName, TOKEN** tokens) {
this->ast=ast;
this->funName=funName;
FunctionVariant* variant=malloc(sizeof(FunctionVariant));
variant->size=ast->size-2;
variant->args=malloc(sizeof(TYPE*) *(variant->size));
for(int i = 0; i < variant->size; i++) {
variant->args[i]=generateType(ast->children[i]->children[1], tokens);
;
};
this->variants[this->variantsSize]=variant;
;
}
#endif

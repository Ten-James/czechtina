#ifndef CZECHTINA_czechtina__compiler__strcts
#define CZECHTINA_czechtina__compiler__strcts
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__compiler__types.c"
#include "czechtina__compiler__functions.c"
#include "std.c"
#include "std__str.c"
#include "czechtina__parser__ast.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _STRCTPROP STRCTPROP;
typedef struct _STRCTFUN STRCTFUN;
typedef struct _STRCTENTRY STRCTENTRY;
typedef struct _STRCTPROP {
STR* name;
TYPE* type;
} STRCTPROP;
typedef struct _STRCTFUN {
STR* name;
FunctionNode* ast;
} STRCTFUN;
typedef struct _STRCTENTRY {
STR* name;
STRCTPROP** props;
int pSize;
int pCapacity;
STRCTFUN** funs;
int fSize;
int fCapacity;
} STRCTENTRY;
void CZ_STRCTENTRY_init(STRCTENTRY* this);
void CZ_STRCTENTRY_addProp(STRCTENTRY* this, STR* name, TYPE* type);
void CZ_STRCTENTRY_addFun(STRCTENTRY* this, STR* name, FunctionNode* ast);
bool CZ_STRCTENTRY_containsProp(STRCTENTRY* this, char* name);
TYPE* CZ_STRCTENTRY_getPropertyType(STRCTENTRY* this, char* name);
bool CZ_STRCTENTRY_containsFun(STRCTENTRY* this, char* name);
STRCTFUN* CZ_STRCTENTRY_getFunction(STRCTENTRY* this, char* name);
TYPE* CZ_STRCTENTRY_getFunctionReturnType(STRCTENTRY* this, char* name);
void CZ_STRCTENTRY_init(STRCTENTRY* this) {
this->props=malloc(sizeof(STRCTPROP*) *(10));
this->pSize=0;
this->pCapacity=10;
this->funs=malloc(sizeof(STRCTFUN*) *(10));
this->fSize=0;
this->fCapacity=10;
;
}
void CZ_STRCTENTRY_addProp(STRCTENTRY* this, STR* name, TYPE* type) {
if (this->pSize==this->pCapacity) {
this->pCapacity*=2;
this->props=realloc(this->props, this->pCapacity * sizeof(STRCTPROP));
;
};
this->props[this->pSize]=malloc(sizeof(STRCTPROP));
this->props[this->pSize]->name=name;
this->props[this->pSize]->type=type;
this->pSize+=1;
;
}
void CZ_STRCTENTRY_addFun(STRCTENTRY* this, STR* name, FunctionNode* ast) {
if (this->fSize==this->fCapacity) {
this->fCapacity*=2;
this->funs=realloc(this->funs, this->fCapacity * sizeof(STRCTFUN));
;
};
this->funs[this->fSize]=malloc(sizeof(STRCTFUN));
this->funs[this->fSize]->name=name;
this->funs[this->fSize]->ast=ast;
this->fSize+=1;
;
}
bool CZ_STRCTENTRY_containsProp(STRCTENTRY* this, char* name) {
for(int i = 0; i < this->pSize; i++) {
if (ptrEqual(this->props[i]->name->ptr, name)) {
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
TYPE* CZ_STRCTENTRY_getPropertyType(STRCTENTRY* this, char* name) {
for(int i = 0; i < this->pSize; i++) {
if (ptrEqual(this->props[i]->name->ptr, name)) {
;
return this->props[i]->type;
;
};
;
};
fprintf(stderr,"src/compiler/structs.cz:75:34\n");fprintf(stderr,"Property not found");exit(1);
;
}
bool CZ_STRCTENTRY_containsFun(STRCTENTRY* this, char* name) {
for(int i = 0; i < this->fSize; i++) {
if (ptrEqual(this->funs[i]->name->ptr, name)) {
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
STRCTFUN* CZ_STRCTENTRY_getFunction(STRCTENTRY* this, char* name) {
for(int i = 0; i < this->fSize; i++) {
if (ptrEqual(this->funs[i]->name->ptr, name)) {
;
return this->funs[i];
;
};
;
};
fprintf(stderr,"src/compiler/structs.cz:93:34\n");fprintf(stderr,"Function not found");exit(1);
;
}
TYPE* CZ_STRCTENTRY_getFunctionReturnType(STRCTENTRY* this, char* name) {
for(int i = 0; i < this->fSize; i++) {
if (ptrEqual(this->funs[i]->name->ptr, name)) {
;
return this->funs[i]->ast->returnType;
;
};
;
};
fprintf(stderr,"src/compiler/structs.cz:102:34\n");fprintf(stderr,"Function not found");exit(1);
;
}
#endif

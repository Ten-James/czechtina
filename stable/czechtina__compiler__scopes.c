#ifndef CZECHTINA_czechtina__compiler__scopes
#define CZECHTINA_czechtina__compiler__scopes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__compiler__types.c"
#include "czechtina__utils__levenshtein.c"
#include "std.c"
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _DEFINEDVARS DEFINEDVARS;
typedef struct _SCOPE SCOPE;
typedef struct _DEFINEDVARS {
STR* name;
TYPE* typ;
bool moved;
} DEFINEDVARS;
typedef struct _SCOPE {
DEFINEDVARS** vars;
int size;
int reserved;
SCOPE* parent;
} SCOPE;
void CZ_SCOPE_init(SCOPE* this);
void CZ_SCOPE_define(SCOPE* this, DEFINEDVARS* var);
bool CZ_SCOPE_isDefined(SCOPE* this, STR* name);
char* CZ_SCOPE_getClosestVariable(SCOPE* this, char* name, int cur);
TYPE* CZ_SCOPE_getType(SCOPE* this, STR* name);
DEFINEDVARS* CZ_SCOPE_get(SCOPE* this, char* name);
void CZ_SCOPE_moveMemFromThisScope(SCOPE* this, STR* name);
void CZ_SCOPE_init(SCOPE* this) {
this->vars=malloc(sizeof(DEFINEDVARS*) *(10));
this->size=0;
this->reserved=10;
this->parent=0;
;
}
void CZ_SCOPE_define(SCOPE* this, DEFINEDVARS* var) {
if (this->size==this->reserved) {
this->reserved*=2;
this->vars=realloc(this->vars, this->reserved * sizeof(DEFINEDVARS));
;
};
this->vars[this->size]=var;
this->size+=1;
;
}
bool CZ_SCOPE_isDefined(SCOPE* this, STR* name) {
for(int i = 0; i < this->size; i++) {
DEFINEDVARS* var=this->vars[i];
char* n=var->name->ptr;
if (ptrEqual(n, name->ptr)) {
;
return true;
;
};
;
};
if (this->parent!=0) {
;
return CZ_SCOPE_isDefined(this->parent, name);
;
};
;
return false;
;
}
char* CZ_SCOPE_getClosestVariable(SCOPE* this, char* name, int cur) {
char* ret=0;
for(int i = 0; i < this->size; i++) {
int val=levenshteinDistance(name, this->vars[i]->name->ptr);
if (val<cur) {
cur=val;
ret=this->vars[i]->name->ptr;
;
};
;
};
if (this->parent!=0) {
char* newRet=CZ_SCOPE_getClosestVariable(this->parent, name, cur);
if (newRet!=0) {
;
return newRet;
;
};
;
};
;
return ret;
;
}
TYPE* CZ_SCOPE_getType(SCOPE* this, STR* name) {
for(int i = 0; i < this->size; i++) {
DEFINEDVARS* var=this->vars[i];
char* n=var->name->ptr;
if (ptrEqual(n, name->ptr)) {
;
return var->typ;
;
};
;
};
if (this->parent!=0) {
;
return CZ_SCOPE_getType(this->parent, name);
;
};
fprintf(stderr,"src/compiler/scopes.cz:79:36\n");fprintf(stderr,"Variable not defined");exit(1);
;
}
DEFINEDVARS* CZ_SCOPE_get(SCOPE* this, char* name) {
for(int i = 0; i < this->size; i++) {
DEFINEDVARS* var=this->vars[i];
char* n=var->name->ptr;
if (ptrEqual(n, name)) {
;
return var;
;
};
;
};
if (this->parent!=0) {
;
return CZ_SCOPE_get(this->parent, name);
;
};
fprintf(stderr,"src/compiler/scopes.cz:93:36\n");fprintf(stderr,"Variable not defined");exit(1);
;
}
void CZ_SCOPE_moveMemFromThisScope(SCOPE* this, STR* name) {
for(int i = 0; i < this->size; i++) {
DEFINEDVARS* var=this->vars[i];
char* n=var->name->ptr;
if (ptrEqual(n, name->ptr)) {
var->moved=true;
;
};
;
};
;
}
#endif

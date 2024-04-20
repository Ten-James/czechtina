#ifndef CZECHTINA_czechtina__compiler__enums
#define CZECHTINA_czechtina__compiler__enums
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "std.c"
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _ENUMVALUE ENUMVALUE;
typedef struct _ENUM ENUM;
typedef struct _ENUMVALUE {
STR* name;
int value;
int used;
} ENUMVALUE;
typedef struct _ENUM {
STR* name;
ENUMVALUE** values;
int size;
int capacity;
} ENUM;
void CZ_ENUM_init(ENUM* this, char* name);
void CZ_ENUM_add(ENUM* this, char* name);
void CZ_ENUM_init(ENUM* this, char* name) {
this->name=toSTR(name);
this->values=malloc(sizeof(ENUMVALUE*) *(10));
this->size=0;
this->capacity=10;
;
}
void CZ_ENUM_add(ENUM* this, char* name) {
if (this->size==this->capacity) {
this->capacity*=2;
this->values=realloc(this->values, this->capacity * sizeof(ENUMVALUE));
;
};
ENUMVALUE* evalue=malloc(sizeof(ENUMVALUE));
evalue->name=toSTR(name);
evalue->value=this->size;
evalue->used=0;
this->values[this->size]=evalue;
this->size+=1;
;
}
#endif

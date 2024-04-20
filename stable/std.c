#ifndef CZECHTINA_std
#define CZECHTINA_std
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _ARR ARR;
void debugPrint(char* in);
typedef struct _ARR {
void* arr;
int size;
int reserved;
} ARR;
void CZ_ARR_destruct(ARR* this);
void CZ_ARR_realloc(ARR* this, int size);
void debugPrint(char* in) {
if (false) {
fprintf(czStdOut, "%s\n", in);
;
};
;
}
void CZ_ARR_destruct(ARR* this) {
free(this->arr);
;
}
void CZ_ARR_realloc(ARR* this, int size) {
this->reserved=this->reserved*2;
this->arr = realloc(this->arr, this->reserved * size);
;
}
#endif

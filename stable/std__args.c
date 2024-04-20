#ifndef CZECHTINA_std__args
#define CZECHTINA_std__args
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "std.c"
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _ARGS ARGS;
typedef struct _ARGS {
int argc;
char** argv;
} ARGS;
void CZ_ARGS_init(ARGS* this, int argc, char** argv);
bool CZ_ARGS_contains(ARGS* this, char* name);
char* CZ_ARGS_getName(ARGS* this, char* name);
char* CZ_ARGS_firstUnNamed(ARGS* this);
void CZ_ARGS_init(ARGS* this, int argc, char** argv) {
this->argc=argc;
this->argv=argv;
;
}
bool CZ_ARGS_contains(ARGS* this, char* name) {
for(int i = 0; i < this->argc; i++) {
if (ptrEqual(this->argv[i], name)) {
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
char* CZ_ARGS_getName(ARGS* this, char* name) {
for(int i = 0; i < this->argc-1; i++) {
if (ptrEqual(this->argv[i], name)) {
;
return this->argv[i+1];
;
};
;
};
;
return "";
;
}
char* CZ_ARGS_firstUnNamed(ARGS* this) {
for(int i = 1; i < this->argc; i++) {
if (this->argv[i][0]!='-') {
if (this->argv[i-1][0]!='-') {
;
return this->argv[i];
;
};
;
};
;
};
;
return "";
;
}
#endif

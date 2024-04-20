#ifndef CZECHTINA_czechtina__compiler__sourcemanager
#define CZECHTINA_czechtina__compiler__sourcemanager
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__compiler.c"
#include "std.c"
#include "std__str.c"
#include "std__paths.c"
#include "std__args.c"
#include "czechtina__parser__ast.c"
#include "czechtina__parser__programAST.c"
#include "czechtina__lexer__token.c"
#include "czechtina__optimalization__borrowchecker.c"
#include "czechtina__optimalization__typeChecker.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _SOURCEMANAGER SOURCEMANAGER;
typedef struct _SOURCEMANAGER {
COMPILEPROCESS** sources;
int size;
int reserved;
bool enableDeducer;
} SOURCEMANAGER;
void CZ_SOURCEMANAGER_init(SOURCEMANAGER* this, int size, bool enableDeducer);
void CZ_SOURCEMANAGER_addSource(SOURCEMANAGER* this, STR* path);
void CZ_SOURCEMANAGER_print(SOURCEMANAGER* this);
void CZ_SOURCEMANAGER_compile(SOURCEMANAGER* this, char* buildDir);
void CZ_SOURCEMANAGER_compileBinary(SOURCEMANAGER* this, char* bin, char* outputDir, ARGS* arg);
void CZ_SOURCEMANAGER_init(SOURCEMANAGER* this, int size, bool enableDeducer) {
this->sources=malloc(sizeof(COMPILEPROCESS*) *(size));
this->size=0;
this->reserved=size;
this->enableDeducer=enableDeducer;
;
}
void CZ_SOURCEMANAGER_addSource(SOURCEMANAGER* this, STR* path) {
if (this->size==this->reserved) {
fprintf(stderr,"src/compiler/sourcemanger.cz:28:45\n");fprintf(stderr,"No more space for sources");exit(1);
;
};
COMPILEPROCESS* source=malloc(sizeof(COMPILEPROCESS));
CZ_COMPILEPROCESS_init(source, path, this->enableDeducer);
this->sources[this->size]=source;
this->size+=1;
;
}
void CZ_SOURCEMANAGER_print(SOURCEMANAGER* this) {
for(int i = 0; i < this->size; i++) {
COMPILEPROCESS* source=this->sources[i];
source->sources=this->sources;
source->sSize=this->size;
;
};
int foundMain=0;
for(int i = 0; i < this->size; i++) {
COMPILEPROCESS* proccess=this->sources[i];
fprintf(czStdOut,"%s", proccess->filePath->ptr);
if (proccess->program!=0) {
PROGRAMAST* prg=proccess->program;
if (prg->main!=0) {
foundMain+=1;
;
};
if (prg->packageName==0) {
fprintf(czStdOut,"\tNo package name found"); fputs("\n",czStdOut);
;
}
else {
fprintf(czStdOut,"\tPackage: "); fprintf(czStdOut,"%s", prg->packageName->ptr); fputs("\n",czStdOut);
;
};
;
}
else {
fprintf(stderr,"src/compiler/sourcemanger.cz:60:50\n");fprintf(stderr,"No program found in source");exit(1);
;
};
;
};
fprintf(czStdOut,"foundMain: "); fprintf(czStdOut,"%d", foundMain); fputs("\n",czStdOut);
;
}
void CZ_SOURCEMANAGER_compile(SOURCEMANAGER* this, char* buildDir) {
if ((dirExist(buildDir))==false) {
createDir(buildDir);
;
};
for(int i = 0; i < this->size; i++) {
COMPILEPROCESS* source=this->sources[i];
source->sources=this->sources;
source->sSize=this->size;
;
};
for(int i = 0; i < this->size; i++) {
COMPILEPROCESS* proccess=this->sources[i];
TypeChecker* checker=malloc(sizeof(TypeChecker));
checker->process=proccess;
if (proccess->program!=0) {
PROGRAMAST* prg=proccess->program;
for(int j = 0; j < prg->sSize; j++) {
CZ_TypeChecker_testStructLeak(checker, prg->strcts[j]);
;
};
for(int j = 0; j < prg->fSize; j++) {
CZ_TypeChecker_testFunctionLeak(checker, prg->functions[j]);
;
};
if (prg->main!=0) {
CZ_TypeChecker_testFunctionLeak(checker, prg->main);
;
};
;
};
//somehow destruct: checker 
;
};
for(int i = 0; i < this->size; i++) {
COMPILEPROCESS* proccess=this->sources[i];
BorrowChecker* checker=malloc(sizeof(BorrowChecker));
checker->shouldPrint=false;
checker->process=proccess;
if (proccess->program!=0) {
PROGRAMAST* prg=proccess->program;
for(int j = 0; j < prg->sSize; j++) {
CZ_BorrowChecker_testStructLeak(checker, prg->strcts[j]);
;
};
for(int j = 0; j < prg->fSize; j++) {
CZ_BorrowChecker_testFunctionLeak(checker, prg->functions[j]);
;
};
if (prg->main!=0) {
CZ_BorrowChecker_testFunctionLeak(checker, prg->main);
;
};
;
};
//somehow destruct: checker 
;
};
for(int i = 0; i < this->size; i++) {
CZ_COMPILEPROCESS_compile(this->sources[i], buildDir);
;
};
fprintf(czStdOut,"All sources compiled"); fputs("\n",czStdOut);
for(int i = 0; i < this->size; i++) {
CZ_COMPILEPROCESS_printUnusedEnumsValue(this->sources[i]);
;
};
;
}
void CZ_SOURCEMANAGER_compileBinary(SOURCEMANAGER* this, char* bin, char* outputDir, ARGS* arg) {
STR* cmd=toSTR("cc -w ");
if (CZ_ARGS_contains(arg, "--debug-flags")) {
CZ_STR_appendPtr(cmd, "-ggdb -O0 ");
;
};
CZ_STR_appendPtr(cmd, "-o ");
CZ_STR_appendPtr(cmd, bin);
CZ_STR_appendPtr(cmd, " ");
CZ_STR_appendPtr(cmd, outputDir);
CZ_STR_appendPtr(cmd, "/main.c");
fprintf(czStdOut,"%s", cmd->ptr); fputs("\n",czStdOut);
system(cmd->ptr);
//somehow destruct: cmd 
;
}
#endif

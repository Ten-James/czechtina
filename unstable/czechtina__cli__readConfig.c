#ifndef CZECHTINA_czechtina__cli__readConfig
#define CZECHTINA_czechtina__cli__readConfig
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "std.c"
#include "std__str.c"
#include "std__files.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
void readConfig(char* name);
void readConfig(char* name) {
STR* cmd=toSTR(name);
CZ_STR_appendPtr(cmd, " ");
char* config=readFile(".buildczech");
if (config==0) {
//somehow destruct: config cmd 
;
return;
;
};
int pos=0;
while (config[pos]!=0) {
if (ptrStartsWith(config+pos, "output=")) {
pos+=7;
char* output=config+pos;
CZ_STR_appendPtr(cmd, " -o ");
while (output[0]!='\n'&&output[0]!=0) {
CZ_STR_appendChar(cmd, output[0]);
output+=1;
;
};
;
};
if (ptrStartsWith(config+pos, "src=")) {
pos+=4;
char* src=config+pos;
CZ_STR_appendPtr(cmd, " --src ");
while (src[0]!='\n'&&src[0]!=0) {
CZ_STR_appendChar(cmd, src[0]);
src+=1;
;
};
;
};
if (ptrStartsWith(config+pos, "outputDir=")) {
pos+=10;
char* outputDir=config+pos;
CZ_STR_appendPtr(cmd, " --output-dir ");
while (outputDir[0]!='\n'&&outputDir[0]!=0) {
CZ_STR_appendChar(cmd, outputDir[0]);
outputDir+=1;
;
};
;
};
if (ptrStartsWith(config+pos, "debugFlags")) {
pos+=11;
CZ_STR_appendPtr(cmd, " --debug-flags ");
;
};
if (ptrStartsWith(config+pos, "backup")) {
pos+=6;
CZ_STR_appendPtr(cmd, " --backup ");
;
};
while (config[pos]!='\n'&&config[pos]!=0) {
pos+=1;
;
};
if (config[pos]==0) {
;
break;
;
};
pos+=1;
;
};
system(cmd->ptr);
//somehow destruct: config cmd 
;
}
#endif

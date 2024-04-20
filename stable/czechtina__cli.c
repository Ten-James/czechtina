#ifndef CZECHTINA_czechtina__cli
#define CZECHTINA_czechtina__cli
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__cli__readConfig.c"
#include "std.c"
#include "std__str.c"
#include "std__files.c"
#include "std__args.c"
#include "czechtina__compiler__sourcemanager.c"
#include "czechtina__utils__printer.c"
#include "czechtina__utils__getFiles.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
int cli(ARGS* arg);
int cli(ARGS* arg) {
if (arg->argc<2) {
if (fileExists(".buildczech")) {
readConfig(arg->argv[0]);
//somehow destruct: arg 
;
return 0;
;
};
fprintf(czStdOut, "Usage: %s <file>\n", arg->argv[0]);
fprintf(stderr,"src/cli/cli.cz:19:33\n");fprintf(stderr,"invalid arguments");exit(1);
;
};
bool deducer=true;
if (CZ_ARGS_contains(arg, "-h")) {
printUsage(arg);
//somehow destruct: arg 
;
return 0;
;
};
if (CZ_ARGS_contains(arg, "-v")) {
char* version="0.9.1";
fprintf(czStdOut, "Czechtina v%s\n", version);
//somehow destruct: arg 
;
return 0;
;
};
if (CZ_ARGS_contains(arg, "--disable-deducer")) {
deducer=false;
;
};
char* outputDir="build";
char* output="main";
if (CZ_ARGS_contains(arg, "-o")) {
output=CZ_ARGS_getName(arg, "-o");
;
};
if (CZ_ARGS_contains(arg, "--output-dir")) {
outputDir=CZ_ARGS_getName(arg, "--output-dir");
;
};
if (CZ_ARGS_contains(arg, "--src")) {
STR* filePath=toSTR(CZ_ARGS_getName(arg, "--src"));
if (filePath->size<2) {
CZ_STR_appendPtr(filePath, ".");
;
};
fprintf(czStdOut, "Reading files from %s\n", filePath->ptr);
FILES* files=malloc(sizeof(FILES));
CZ_FILES_init(files);
CZ_FILES_readDir(files, filePath->ptr);
SOURCEMANAGER* manager=malloc(sizeof(SOURCEMANAGER));
CZ_SOURCEMANAGER_init(manager, files->size, deducer);
for(int i = 0; i < files->size; i++) {
CZ_SOURCEMANAGER_addSource(manager, files->arr[i]);
;
};
CZ_SOURCEMANAGER_compile(manager, outputDir);
if (CZ_ARGS_contains(arg, "--no-binary")) {
//somehow destruct: manager files filePath arg 
;
return 0;
;
};
if (CZ_ARGS_contains(arg, "--backup")) {
STR* cmd=toSTR("cp ");
CZ_STR_appendPtr(cmd, output);
CZ_STR_appendPtr(cmd, " ");
CZ_STR_appendPtr(cmd, output);
CZ_STR_appendPtr(cmd, ".bak");
fprintf(czStdOut,"%s", cmd->ptr); fputs("\n",czStdOut);
system(cmd->ptr);
//somehow destruct: cmd 
;
};
CZ_SOURCEMANAGER_compileBinary(manager, output, outputDir, arg);
//somehow destruct: manager files filePath arg 
;
return 0;
//somehow destruct: manager files filePath 
;
};
SOURCEMANAGER* manager=malloc(sizeof(SOURCEMANAGER));
CZ_SOURCEMANAGER_init(manager, 1, deducer);
STR* file=toSTR(CZ_ARGS_firstUnNamed(arg));
if (file->size<2) {
fprintf(stderr,"src/cli/cli.cz:94:28\n");fprintf(stderr,"invalid file");exit(1);
;
};
CZ_SOURCEMANAGER_addSource(manager, file);
CZ_SOURCEMANAGER_compile(manager, outputDir);
if (CZ_ARGS_contains(arg, "--no-binary")) {
//somehow destruct: file manager arg 
;
return 0;
;
};
if (CZ_ARGS_contains(arg, "--backup")) {
STR* cmd=toSTR("cp ");
CZ_STR_appendPtr(cmd, output);
CZ_STR_appendPtr(cmd, " ");
CZ_STR_appendPtr(cmd, output);
CZ_STR_appendPtr(cmd, ".bak");
fprintf(czStdOut,"%s", cmd->ptr); fputs("\n",czStdOut);
system(cmd->ptr);
//somehow destruct: cmd 
;
};
CZ_SOURCEMANAGER_compileBinary(manager, output, outputDir, arg);
//somehow destruct: file manager arg 
;
return 0;
//somehow destruct: file manager arg 
;
}
#endif

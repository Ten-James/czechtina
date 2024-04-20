#ifndef CZECHTINA_czechtina__utils__printer
#define CZECHTINA_czechtina__utils__printer
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "std.c"
#include "std__args.c"
#include "czechtina__lexer__token.c"
#include "czechtina__parser__ast.c"
#include "czechtina__parser__programAST.c"
#include "czechtina__compiler__functions.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
void printUsage(ARGS* arg);
void printTokens(TOKEN** arr, int from, int to);
void printError(int pos, TOKEN** tokens, char* text, char* message);
void printUsage(ARGS* arg) {
fprintf(czStdOut,"Usage:"); fputs("\n",czStdOut);
fprintf(czStdOut, "  %s [-v] [-h] [--disable-deducer] <filepath> [--src <path>] [-o <path>] [-backup]\n", arg->argv[0]);
fprintf(czStdOut,"Options:"); fputs("\n",czStdOut);
fprintf(czStdOut,"  -v, --version        Print version"); fputs("\n",czStdOut);
fprintf(czStdOut,"  -h, --help           Print this help"); fputs("\n",czStdOut);
fprintf(czStdOut,"  --disable-deducer    Disable type deducer"); fputs("\n",czStdOut);
fprintf(czStdOut,"  --src <path>         Set source path"); fputs("\n",czStdOut);
fprintf(czStdOut,"  -o <path>            Set binary output path"); fputs("\n",czStdOut);
fprintf(czStdOut,"  -backup              Automatic backup output"); fputs("\n",czStdOut);
;
}
void printTokens(TOKEN** arr, int from, int to) {
fprintf(czStdOut,"Tokens:"); fputs("\n",czStdOut);
for(int i = from; i < to; i++) {
TOKEN* token=arr[i];
fprintf(czStdOut, "%4d: ", i);
fprintf(czStdOut,"{ ");
printTokenType(token->type);
fprintf(czStdOut,", col:"); fprintf(czStdOut,"%d", token->col); fprintf(czStdOut,", line:"); fprintf(czStdOut,"%d", token->line);
if (token->type==29) {
fprintf(czStdOut, ", value: %d", token->nValue);
;
}
else if (token->type==30) {
fprintf(czStdOut, ", value: [%c]%d", token->nValue, token->nValue);
;
}
else if (token->type==27||token->type==28) {
fprintf(czStdOut, ", value: %s", token->value);
;
};
fprintf(czStdOut," }"); fputs("\n",czStdOut);
;
};
fprintf(czStdOut,"-------------------------------------------"); fputs("\n",czStdOut);
;
}
void printError(int pos, TOKEN** tokens, char* text, char* message) {
void* old;
old = czStdOut;
czStdOut = stderr;
TOKEN* token;
token=tokens[pos];
fprintf(czStdOut, "Error at line %d, col %d: %s\n", token->line, token->col, message);
int textpos=0;
for(int i = 0; i < token->line-2; i++) {
textpos++;
while (text[textpos]!='\n'&&text[textpos]!=0) {
textpos++;
;
};
;
};
textpos++;
while (text[textpos]!='\n'&&text[textpos]!=0) {
fprintf(czStdOut,"%c", text[textpos]);
textpos++;
;
};
fprintf(czStdOut,""); fputs("\n",czStdOut);
textpos++;
while (text[textpos]!='\n'&&text[textpos]!=0) {
fprintf(czStdOut,"%c", text[textpos]);
textpos++;
;
};
fprintf(czStdOut,""); fputs("\n",czStdOut);
for(int i = 0; i < token->col-1; i++) {
fprintf(czStdOut," ");
;
};
fprintf(czStdOut,"^"); fputs("\n",czStdOut);
textpos++;
while (text[textpos]!='\n'&&text[textpos]!=0) {
fprintf(czStdOut,"%c", text[textpos]);
textpos++;
;
};
fprintf(czStdOut,""); fputs("\n",czStdOut);
fprintf(czStdOut,"-------------------------------------------"); fputs("\n",czStdOut);
czStdOut = old;
;
}
#endif

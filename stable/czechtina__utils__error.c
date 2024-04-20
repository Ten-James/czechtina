#ifndef CZECHTINA_czechtina__utils__error
#define CZECHTINA_czechtina__utils__error
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__lexer__token.c"
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _Error Error;
typedef struct _Error {
STR* message;
STR* secMessage;
int pos;
TOKEN** tokens;
char* text;
char* filePath;
int level;
} Error;
void CZ_Error_init(Error* this);
void CZ_Error_destruct(Error* this);
void CZ_Error_setColor(Error* this);
void CZ_Error_print(Error* this, bool longPrint);
void CZ_Error_init(Error* this) {
this->message=toSTR("");
this->secMessage=toSTR("");
this->pos=0;
this->level=2;
;
}
void CZ_Error_destruct(Error* this) {
CZ_STR_destruct(this->message);
free(this->message);
CZ_STR_destruct(this->secMessage);
free(this->secMessage);
;
}
void CZ_Error_setColor(Error* this) {
if (this->level==0) {
fprintf(czStdOut,"\033[34m");
;
}
else if (this->level==1) {
fprintf(czStdOut,"\033[33m");
;
}
else if (this->level==2) {
fprintf(czStdOut,"\033[91m");
;
}
else if (this->level==3) {
fprintf(czStdOut,"\033[91m");
;
};
;
}
void CZ_Error_print(Error* this, bool longPrint) {
void* old;
old = czStdOut;
czStdOut = stderr;
TOKEN* token=this->tokens[this->pos];
char* text=this->text;
fprintf(czStdOut,"%s", this->filePath); fprintf(czStdOut,":"); fprintf(czStdOut,"%d", token->line); fprintf(czStdOut,":"); fprintf(czStdOut,"%d", token->col); fprintf(czStdOut,": ");
CZ_Error_setColor(this);
if (this->level==0) {
fprintf(czStdOut,"info: ");
;
}
else if (this->level==1) {
fprintf(czStdOut,"warning: ");
;
}
else if (this->level==2) {
fprintf(czStdOut,"error: ");
;
}
else if (this->level==3) {
fprintf(czStdOut,"fatal: ");
;
};
fprintf(czStdOut,"%s", this->message->ptr); fputs("\n",czStdOut);
fprintf(czStdOut,"\033[39m");
if (longPrint) {
fprintf(czStdOut,"-------------------------------------------"); fputs("\n",czStdOut);
int textpos=0;
for(int i = 0; i < token->line-2; i++) {
textpos++;
while (text[textpos]!='\n'&&text[textpos]!=0) {
textpos++;
;
};
;
};
fprintf(czStdOut, "%4d|", token->line-1);
textpos++;
while (text[textpos]!='\n'&&text[textpos]!=0) {
fprintf(czStdOut,"%c", text[textpos]);
textpos++;
;
};
fprintf(czStdOut,""); fputs("\n",czStdOut);
fprintf(czStdOut, "%4d|", token->line);
textpos++;
while (text[textpos]!='\n'&&text[textpos]!=0) {
fprintf(czStdOut,"%c", text[textpos]);
textpos++;
;
};
fprintf(czStdOut,""); fputs("\n",czStdOut);
fprintf(czStdOut, "    |");
for(int i = 0; i < token->col-1; i++) {
fprintf(czStdOut," ");
;
};
CZ_Error_setColor(this);
fprintf(czStdOut,"^"); fprintf(czStdOut,"%s", this->secMessage->ptr); fprintf(czStdOut,"\033[39m"); fputs("\n",czStdOut);
fprintf(czStdOut, "%4d|", token->line+1);
textpos++;
while (text[textpos]!='\n'&&text[textpos]!=0) {
fprintf(czStdOut,"%c", text[textpos]);
textpos++;
;
};
fprintf(czStdOut,""); fputs("\n",czStdOut);
fprintf(czStdOut,"-------------------------------------------"); fputs("\n",czStdOut);
;
};
czStdOut = old;
;
}
#endif

#ifndef CZECHTINA_czechtina__lexer
#define CZECHTINA_czechtina__lexer
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__lexer__token.c"
#include "std.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _LEXSTATE LEXSTATE;
ARR* lex(char* text);
void appendToken(ARR* arr, LEXSTATE* state, int type);
void appendTokenNumber(ARR* arr, LEXSTATE* state, int type, int number);
void appendTokenString(ARR* arr, LEXSTATE* state, int type, char* text);
typedef struct _LEXSTATE {
int pos;
int line;
int col;
char* text;
} LEXSTATE;
void CZ_LEXSTATE_move(LEXSTATE* this, int size);
void CZ_LEXSTATE_skipSpace(LEXSTATE* this);
void CZ_LEXSTATE_skipTillEndOfLine(LEXSTATE* this);
void CZ_LEXSTATE_skipTillEndOfBlockComment(LEXSTATE* this);
bool CZ_LEXSTATE_isCurrentWord(LEXSTATE* this, char* word);
ARR* lex(char* text) {
LEXSTATE* state=malloc(sizeof(LEXSTATE));
state->text=text;
state->pos=0;
state->line=1;
state->col=1;
TOKEN** tokens;
ARR* arr;
arr = (ARR *)malloc(sizeof(ARR));
tokens = (TOKEN **)((TOKEN **)malloc(255 * sizeof(TOKEN *)));
arr->arr = tokens;
arr->size=0;
arr->reserved=255;
while (text[state->pos]!=0) {
CZ_LEXSTATE_skipSpace(state);
if (text[state->pos]=='/'&&text[state->pos+1]=='/') {
CZ_LEXSTATE_skipTillEndOfLine(state);
;
continue;
;
};
if (text[state->pos]=='/'&&text[state->pos+1]=='*') {
CZ_LEXSTATE_skipTillEndOfBlockComment(state);
;
continue;
;
};
if (text[state->pos]=='+') {
if (text[state->pos+1]=='+') {
appendToken(arr, state, 1);
CZ_LEXSTATE_move(state, 1);
;
}
else if (text[state->pos+1]=='=') {
appendToken(arr, state, 53);
CZ_LEXSTATE_move(state, 1);
;
}
else {
appendToken(arr, state, 0);
;
};
;
}
else if (text[state->pos]=='-') {
if (text[state->pos+1]=='-') {
appendToken(arr, state, 3);
CZ_LEXSTATE_move(state, 1);
;
}
else if (text[state->pos+1]=='>') {
appendToken(arr, state, 51);
CZ_LEXSTATE_move(state, 1);
;
}
else if (text[state->pos+1]=='=') {
appendToken(arr, state, 54);
CZ_LEXSTATE_move(state, 1);
;
}
else {
appendToken(arr, state, 2);
;
};
;
}
else if (text[state->pos]=='*') {
if (text[state->pos+1]=='=') {
appendToken(arr, state, 55);
CZ_LEXSTATE_move(state, 1);
;
}
else {
appendToken(arr, state, 7);
;
};
;
}
else if (text[state->pos]=='/') {
if (text[state->pos+1]=='=') {
appendToken(arr, state, 56);
CZ_LEXSTATE_move(state, 1);
;
}
else {
appendToken(arr, state, 8);
;
};
;
}
else if (text[state->pos]=='%') {
if (text[state->pos+1]=='=') {
appendToken(arr, state, 57);
CZ_LEXSTATE_move(state, 1);
;
}
else {
appendToken(arr, state, 4);
;
};
;
}
else if (text[state->pos]=='?') {
appendToken(arr, state, 6);
;
}
else if (text[state->pos]=='=') {
if (text[state->pos+1]=='=') {
appendToken(arr, state, 47);
CZ_LEXSTATE_move(state, 1);
;
}
else {
appendToken(arr, state, 52);
;
};
;
}
else if (text[state->pos]=='!') {
if (text[state->pos+1]=='=') {
appendToken(arr, state, 48);
CZ_LEXSTATE_move(state, 1);
;
}
else {
appendToken(arr, state, 9);
;
};
;
}
else if (text[state->pos]=='(') {
appendToken(arr, state, 33);
;
}
else if (text[state->pos]==')') {
appendToken(arr, state, 34);
;
}
else if (text[state->pos]=='{') {
appendToken(arr, state, 35);
;
}
else if (text[state->pos]=='}') {
appendToken(arr, state, 36);
;
}
else if (text[state->pos]=='[') {
appendToken(arr, state, 41);
;
}
else if (text[state->pos]==']') {
appendToken(arr, state, 42);
;
}
else if (text[state->pos]==';') {
appendToken(arr, state, 40);
;
}
else if (text[state->pos]==',') {
appendToken(arr, state, 37);
;
}
else if (text[state->pos]=='.') {
appendToken(arr, state, 38);
;
}
else if (text[state->pos]==':') {
appendToken(arr, state, 39);
;
}
else if (text[state->pos]=='<') {
if (text[state->pos+1]=='=') {
appendToken(arr, state, 45);
CZ_LEXSTATE_move(state, 1);
;
}
else {
appendToken(arr, state, 43);
;
};
;
}
else if (text[state->pos]=='>') {
if (text[state->pos+1]=='=') {
appendToken(arr, state, 46);
CZ_LEXSTATE_move(state, 1);
;
}
else {
appendToken(arr, state, 44);
;
};
;
}
else if (text[state->pos]=='&') {
if (text[state->pos+1]=='&') {
appendToken(arr, state, 49);
CZ_LEXSTATE_move(state, 1);
;
}
else {
appendToken(arr, state, 5);
;
};
;
}
else if (text[state->pos]=='|'&&text[state->pos+1]=='|') {
appendToken(arr, state, 50);
CZ_LEXSTATE_move(state, 1);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "#depricated")) {
appendToken(arr, state, 64);
CZ_LEXSTATE_move(state, 10);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "if")) {
appendToken(arr, state, 10);
CZ_LEXSTATE_move(state, 1);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "else")) {
appendToken(arr, state, 11);
CZ_LEXSTATE_move(state, 3);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "while")) {
appendToken(arr, state, 12);
CZ_LEXSTATE_move(state, 4);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "for")) {
appendToken(arr, state, 13);
CZ_LEXSTATE_move(state, 2);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "return")) {
appendToken(arr, state, 14);
CZ_LEXSTATE_move(state, 5);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "break")) {
appendToken(arr, state, 15);
CZ_LEXSTATE_move(state, 4);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "continue")) {
appendToken(arr, state, 17);
CZ_LEXSTATE_move(state, 7);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "struct")) {
appendToken(arr, state, 19);
CZ_LEXSTATE_move(state, 5);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "typedef")) {
appendToken(arr, state, 18);
CZ_LEXSTATE_move(state, 6);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "enum")) {
appendToken(arr, state, 20);
CZ_LEXSTATE_move(state, 3);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "use")) {
appendToken(arr, state, 21);
CZ_LEXSTATE_move(state, 2);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "new")) {
appendToken(arr, state, 16);
CZ_LEXSTATE_move(state, 2);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "package")) {
appendToken(arr, state, 22);
CZ_LEXSTATE_move(state, 6);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "as")) {
appendToken(arr, state, 23);
CZ_LEXSTATE_move(state, 1);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "do")) {
appendToken(arr, state, 25);
CZ_LEXSTATE_move(state, 1);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "az")) {
appendToken(arr, state, 26);
CZ_LEXSTATE_move(state, 1);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "call")) {
appendToken(arr, state, 24);
CZ_LEXSTATE_move(state, 3);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "zavolej")) {
appendToken(arr, state, 24);
CZ_LEXSTATE_move(state, 6);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "true")) {
appendToken(arr, state, 31);
CZ_LEXSTATE_move(state, 3);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "false")) {
appendToken(arr, state, 32);
CZ_LEXSTATE_move(state, 4);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "int")) {
appendToken(arr, state, 58);
CZ_LEXSTATE_move(state, 2);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "char")) {
appendToken(arr, state, 59);
CZ_LEXSTATE_move(state, 3);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "void")) {
appendToken(arr, state, 60);
CZ_LEXSTATE_move(state, 3);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "bool")) {
appendToken(arr, state, 62);
CZ_LEXSTATE_move(state, 3);
;
}
else if (CZ_LEXSTATE_isCurrentWord(state, "pointer")) {
appendToken(arr, state, 63);
CZ_LEXSTATE_move(state, 6);
;
}
else if (text[state->pos]>='0'&&text[state->pos]<='9') {
int pos=state->pos;
int value=0;
while (text[pos]>='0'&&text[pos]<='9') {
value=value*10+text[pos]-'0';
pos++;
;
};
pos--;
appendTokenNumber(arr, state, 29, value);
CZ_LEXSTATE_move(state, pos-state->pos);
;
}
else if (('a'<=text[state->pos]&&text[state->pos]<='z')||('A'<=text[state->pos]&&text[state->pos]<='Z')) {
int pos=state->pos;
while (('a'<=text[pos]&&text[pos]<='z')||('A'<=text[pos]&&text[pos]<='Z')||('0'<=text[pos]&&text[pos]<='9')) {
pos++;
;
};
pos--;
char* identifierText=malloc(sizeof(char*) *(pos-state->pos+1));
for(int i = 0; i < pos-state->pos+1; i++) {
identifierText[i]=text[state->pos+i];
;
};
identifierText[pos-state->pos+1]=0;
appendTokenString(arr, state, 27, identifierText);
CZ_LEXSTATE_move(state, pos-state->pos);
;
}
else if (text[state->pos]==34) {
CZ_LEXSTATE_move(state, 1);
int pos=state->pos;
while (1) {
if (text[state->pos]==34) {
;
break;
;
};
if (text[state->pos]==0) {
fprintf(stderr,"src/lexer/lexer.cz:368:53\n");fprintf(stderr,"unexcepted string literal");exit(1);
;
};
if (text[state->pos]==92) {
CZ_LEXSTATE_move(state, 1);
;
};
CZ_LEXSTATE_move(state, 1);
;
};
char* identifierText=malloc(sizeof(char*) *(state->pos-pos+1));
for(int i = 0; i < state->pos-pos; i++) {
identifierText[i]=text[pos+i];
;
};
identifierText[state->pos-pos]=0;
appendTokenString(arr, state, 28, identifierText);
;
}
else if (text[state->pos]==39) {
CZ_LEXSTATE_move(state, 1);
int value=text[state->pos];
if (value==92) {
CZ_LEXSTATE_move(state, 1);
value=text[state->pos];
if (value=='n') {
value=10;
;
}
else if (value=='t') {
value=9;
;
}
else if (value=='r') {
value=13;
;
}
else if (value=='0') {
value=0;
;
}
else if (value==34) {
value=34;
;
}
else if (value==39) {
value=39;
;
}
else if (value==92) {
value=92;
;
}
else {
fprintf(czStdOut,"Unknown escape sequence: "); fprintf(czStdOut,"%d", value); fputs("\n",czStdOut);
;
};
;
};
if (text[state->pos+1]!=39) {
fprintf(stderr,"src/lexer/lexer.cz:401:58\n");fprintf(stderr,"Expected ' after character literal");exit(1);
;
};
CZ_LEXSTATE_move(state, 1);
appendTokenNumber(arr, state, 30, value);
;
}
else if (text[state->pos]==0) {
appendToken(arr, state, 65);
;
}
else {
fprintf(czStdOut,"Unknown token: "); fprintf(czStdOut,"%c", text[state->pos]); fprintf(czStdOut," at "); fprintf(czStdOut,"%d", state->line); fprintf(czStdOut,":"); fprintf(czStdOut,"%d", state->col); fputs("\n",czStdOut);
fprintf(czStdOut, "char value %d\n", text[state->pos]);
;
};
CZ_LEXSTATE_move(state, 1);
;
};
appendToken(arr, state, 65);
//somehow destruct: state 
;
return arr;
//somehow destruct: state 
;
}
void appendToken(ARR* arr, LEXSTATE* state, int type) {
TOKEN* token;
token = (TOKEN *)malloc(sizeof(TOKEN));
token->type=type;
token->line=state->line;
token->col=state->col;
token->size=1;
((void**)arr->arr)[arr->size] = token;
arr->size+=1;
if (arr->size==arr->reserved) {
CZ_ARR_realloc(arr, (sizeof(void*)));
;
};
;
}
void appendTokenNumber(ARR* arr, LEXSTATE* state, int type, int number) {
TOKEN* token;
token = (TOKEN *)malloc(sizeof(TOKEN));
token->type=type;
token->line=state->line;
token->col=state->col;
token->nValue=number;
((void**)arr->arr)[arr->size] = token;
arr->size+=1;
if (arr->size==arr->reserved) {
CZ_ARR_realloc(arr, (sizeof(void*)));
;
};
;
}
void appendTokenString(ARR* arr, LEXSTATE* state, int type, char* text) {
TOKEN* token;
token = (TOKEN *)malloc(sizeof(TOKEN));
token->type=type;
token->line=state->line;
token->col=state->col;
CZ_TOKEN_consume(token, text);
((void**)arr->arr)[arr->size] = token;
arr->size+=1;
if (arr->size==arr->reserved) {
CZ_ARR_realloc(arr, (sizeof(void*)));
;
};
;
}
void CZ_LEXSTATE_move(LEXSTATE* this, int size) {
char* text=this->text;
for(int i = 0; i < size; i++) {
if (text[this->pos]==0) {
;
return;
;
};
if (text[this->pos]=='\n') {
this->line+=1;
this->col=1;
;
}
else {
this->col+=1;
;
};
this->pos+=1;
;
};
;
}
void CZ_LEXSTATE_skipSpace(LEXSTATE* this) {
char* text=this->text;
while (text[this->pos]==' '||text[this->pos]=='\t'||text[this->pos]=='\n'||text[this->pos]=='\r') {
CZ_LEXSTATE_move(this, 1);
;
};
;
}
void CZ_LEXSTATE_skipTillEndOfLine(LEXSTATE* this) {
char* text=this->text;
while (text[this->pos]!=0&&text[this->pos]!='\n') {
CZ_LEXSTATE_move(this, 1);
;
};
;
}
void CZ_LEXSTATE_skipTillEndOfBlockComment(LEXSTATE* this) {
char* text=this->text;
while (text[this->pos]!=0) {
if (text[this->pos]=='*'&&text[this->pos+1]=='/') {
CZ_LEXSTATE_move(this, 2);
;
return;
;
};
CZ_LEXSTATE_move(this, 1);
;
};
;
}
bool CZ_LEXSTATE_isCurrentWord(LEXSTATE* this, char* word) {
char* text=this->text;
int i=0;
while (word[i]!=0) {
if (text[this->pos+i]==0) {
;
return false;
;
};
if (text[this->pos+i]!=word[i]) {
;
return false;
;
};
i++;
;
};
if (('a'<=text[this->pos+i]&&text[this->pos+i]<='z')||('A'<=text[this->pos+i]&&text[this->pos+i]<='Z')||('0'<=text[this->pos+i]&&text[this->pos+i]<='9')) {
;
return false;
;
};
;
return true;
;
}
#endif

#ifndef CZECHTINA_czechtina__lexer__token
#define CZECHTINA_czechtina__lexer__token
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _TOKEN TOKEN;
void printTokenType(int type);
typedef struct _TOKEN {
int type;
int nValue;
char* value;
int size;
int line;
int col;
} TOKEN;
void CZ_TOKEN_consume(TOKEN* this, char* text);
void CZ_TOKEN_destruct(TOKEN* this);
void printTokenType(int type) {
if (type==0) {
fprintf(czStdOut,"+");
;
}
else if (type==1) {
fprintf(czStdOut,"++");
;
}
else if (type==2) {
fprintf(czStdOut,"-");
;
}
else if (type==3) {
fprintf(czStdOut,"--");
;
}
else if (type==4) {
fprintf(czStdOut,"%%");
;
}
else if (type==5) {
fprintf(czStdOut,"&");
;
}
else if (type==6) {
fprintf(czStdOut,"?");
;
}
else if (type==7) {
fprintf(czStdOut,"*");
;
}
else if (type==8) {
fprintf(czStdOut,"/");
;
}
else if (type==9) {
fprintf(czStdOut,"!");
;
}
else if (type==10) {
fprintf(czStdOut,"if");
;
}
else if (type==11) {
fprintf(czStdOut,"else");
;
}
else if (type==12) {
fprintf(czStdOut,"while");
;
}
else if (type==13) {
fprintf(czStdOut,"for");
;
}
else if (type==14) {
fprintf(czStdOut,"return");
;
}
else if (type==15) {
fprintf(czStdOut,"break");
;
}
else if (type==17) {
fprintf(czStdOut,"continue");
;
}
else if (type==18) {
fprintf(czStdOut,"typedef");
;
}
else if (type==19) {
fprintf(czStdOut,"struct");
;
}
else if (type==20) {
fprintf(czStdOut,"enum");
;
}
else if (type==21) {
fprintf(czStdOut,"use");
;
}
else if (type==16) {
fprintf(czStdOut,"new");
;
}
else if (type==22) {
fprintf(czStdOut,"package");
;
}
else if (type==23) {
fprintf(czStdOut,"as");
;
}
else if (type==26) {
fprintf(czStdOut,"az");
;
}
else if (type==25) {
fprintf(czStdOut,"do");
;
}
else if (type==24) {
fprintf(czStdOut,"zavolej");
;
}
else if (type==27) {
fprintf(czStdOut,"Identifier");
;
}
else if (type==28) {
fprintf(czStdOut,"String");
;
}
else if (type==29) {
fprintf(czStdOut,"Number");
;
}
else if (type==30) {
fprintf(czStdOut,"Character");
;
}
else if (type==31) {
fprintf(czStdOut,"true");
;
}
else if (type==32) {
fprintf(czStdOut,"false");
;
}
else if (type==40) {
fprintf(czStdOut,";");
;
}
else if (type==33) {
fprintf(czStdOut,"(");
;
}
else if (type==34) {
fprintf(czStdOut,")");
;
}
else if (type==35) {
fprintf(czStdOut,"{");
;
}
else if (type==36) {
fprintf(czStdOut,"}");
;
}
else if (type==37) {
fprintf(czStdOut,",");
;
}
else if (type==38) {
fprintf(czStdOut,".");
;
}
else if (type==41) {
fprintf(czStdOut,"[");
;
}
else if (type==42) {
fprintf(czStdOut,"]");
;
}
else if (type==43) {
fprintf(czStdOut,"<");
;
}
else if (type==44) {
fprintf(czStdOut,">");
;
}
else if (type==45) {
fprintf(czStdOut,"<=");
;
}
else if (type==46) {
fprintf(czStdOut,">=");
;
}
else if (type==39) {
fprintf(czStdOut,":");
;
}
else if (type==47) {
fprintf(czStdOut,"==");
;
}
else if (type==48) {
fprintf(czStdOut,"!=");
;
}
else if (type==49) {
fprintf(czStdOut,"&&");
;
}
else if (type==50) {
fprintf(czStdOut,"||");
;
}
else if (type==51) {
fprintf(czStdOut,"->");
;
}
else if (type==52) {
fprintf(czStdOut,"=");
;
}
else if (type==53) {
fprintf(czStdOut,"+=");
;
}
else if (type==54) {
fprintf(czStdOut,"-=");
;
}
else if (type==55) {
fprintf(czStdOut,"*=");
;
}
else if (type==56) {
fprintf(czStdOut,"/=");
;
}
else if (type==57) {
fprintf(czStdOut,"%%=");
;
}
else if (type==58) {
fprintf(czStdOut,"int");
;
}
else if (type==59) {
fprintf(czStdOut,"char");
;
}
else if (type==60) {
fprintf(czStdOut,"void");
;
}
else if (type==61) {
fprintf(czStdOut,"float");
;
}
else if (type==62) {
fprintf(czStdOut,"bool");
;
}
else if (type==63) {
fprintf(czStdOut,"pointer");
;
}
else if (type==65) {
fprintf(czStdOut,"EOF");
;
}
else {
fprintf(stderr,"src/lexer/token.cz:176:34\n");fprintf(stderr,"Unknown token type");exit(1);
;
};
;
}
void CZ_TOKEN_consume(TOKEN* this, char* text) {
this->value=text;
;
}
void CZ_TOKEN_destruct(TOKEN* this) {
if (this->value!=0&&this->type!=29) {
free(this->value);
;
};
;
}
#endif

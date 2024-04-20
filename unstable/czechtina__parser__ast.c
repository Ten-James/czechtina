#ifndef CZECHTINA_czechtina__parser__ast
#define CZECHTINA_czechtina__parser__ast
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _NODE NODE;
void printAstOperandType(int type);
typedef struct _NODE {
int type;
int tokensStart;
int tokensEnd;
int operand;
NODE** children;
STR** strs;
int size;
int reserved;
} NODE;
void CZ_NODE_init(NODE* this);
void CZ_NODE_consumeBinary(NODE* this, NODE* left, NODE* right, int operand);
void CZ_NODE_consumeUnary(NODE* this, NODE* child, int operand);
void CZ_NODE_createArr(NODE* this);
void CZ_NODE_reallocStrs(NODE* this);
void CZ_NODE_add(NODE* this, NODE* node);
void CZ_NODE_addBeforeNode(NODE* this, NODE* beforeNode, NODE* node);
void printAstOperandType(int type) {
if (type==0) {
fprintf(czStdOut,"");
;
}
else if (type==1) {
fprintf(czStdOut,"!");
;
}
else if (type==2) {
fprintf(czStdOut,"*");
;
}
else if (type==3) {
fprintf(czStdOut,"memmove");
;
}
else if (type==5) {
fprintf(czStdOut,"()");
;
}
else if (type==6) {
fprintf(czStdOut,"");
;
}
else if (type==7) {
fprintf(czStdOut,"==");
;
}
else if (type==8) {
fprintf(czStdOut,"!=");
;
}
else if (type==9) {
fprintf(czStdOut,"<");
;
}
else if (type==10) {
fprintf(czStdOut,">");
;
}
else if (type==11) {
fprintf(czStdOut,"<=");
;
}
else if (type==12) {
fprintf(czStdOut,">=");
;
}
else if (type==13) {
fprintf(czStdOut,"+");
;
}
else if (type==14) {
fprintf(czStdOut,"++");
;
}
else if (type==15) {
fprintf(czStdOut,"-");
;
}
else if (type==16) {
fprintf(czStdOut,"--");
;
}
else if (type==17) {
fprintf(czStdOut,"*");
;
}
else if (type==18) {
fprintf(czStdOut,"/");
;
}
else if (type==19) {
fprintf(czStdOut,"%%");
;
}
else if (type==20) {
fprintf(czStdOut,"=");
;
}
else if (type==21) {
fprintf(czStdOut,"+=");
;
}
else if (type==22) {
fprintf(czStdOut,"!=");
;
}
else if (type==23) {
fprintf(czStdOut,"-=");
;
}
else if (type==24) {
fprintf(czStdOut,"%%=");
;
}
else if (type==25) {
fprintf(czStdOut,"/=");
;
}
else if (type==26) {
fprintf(czStdOut,"*=");
;
}
else if (type==27) {
fprintf(czStdOut,"&&");
;
}
else if (type==28) {
fprintf(czStdOut,"||");
;
}
else {
fprintf(czStdOut,"Unknown");
;
};
;
}
void CZ_NODE_init(NODE* this) {
this->children=0;
this->size=0;
this->reserved=0;
this->operand=0;
;
}
void CZ_NODE_consumeBinary(NODE* this, NODE* left, NODE* right, int operand) {
this->operand=operand;
this->size=2;
this->reserved=2;
this->children=malloc(sizeof(NODE*) *(2));
this->children[0]=left;
this->children[1]=right;
;
}
void CZ_NODE_consumeUnary(NODE* this, NODE* child, int operand) {
this->operand=operand;
this->size=1;
this->reserved=1;
this->children=malloc(sizeof(NODE*) *(1));
this->children[0]=child;
;
}
void CZ_NODE_createArr(NODE* this) {
this->size=0;
this->reserved=5;
this->children=malloc(sizeof(NODE*) *(5));
;
}
void CZ_NODE_reallocStrs(NODE* this) {
this->reserved=this->reserved*2;
this->strs=realloc(this->strs, sizeof(STR *) * this->reserved);
;
}
void CZ_NODE_add(NODE* this, NODE* node) {
if (this->size==this->reserved) {
this->reserved=this->reserved*2;
this->children=realloc(this->children, sizeof(NODE *) * this->reserved);
;
};
this->children[this->size]=node;
this->size=this->size+1;
;
}
void CZ_NODE_addBeforeNode(NODE* this, NODE* beforeNode, NODE* node) {
if (this->size==this->reserved) {
this->reserved=this->reserved*2;
this->children=realloc(this->children, sizeof(NODE *) * this->reserved);
;
};
int before=0;
for(int i = 0; i < this->size; i++) {
if (this->children[i]==beforeNode) {
before=i;
;
break;
;
};
;
};
this->size+=1;
int pos=this->size-1;
while (pos>before) {
this->children[pos] = this->children[pos - 1];
pos-=1;
;
};
this->children[before]=node;
;
}
#endif

#ifndef CZECHTINA_czechtina__compiler__types
#define CZECHTINA_czechtina__compiler__types
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__parser__ast.c"
#include "czechtina__lexer__token.c"
#include "czechtina__utils__printer.c"
#include "std.c"
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _TYPE TYPE;
TYPE* generateType(NODE* ast, TOKEN** tokens);
typedef struct _TYPE {
int variant;
STR* name;
TYPE* dereference;
bool isMemMove;
bool isOptional;
} TYPE;
void CZ_TYPE_printType(TYPE* this);
void CZ_TYPE_toC(TYPE* this);
STR* CZ_TYPE_toSTR(TYPE* this);
void CZ_TYPE_appendSTR(TYPE* this, STR* str);
TYPE* CZ_TYPE_copy(TYPE* this);
TYPE* generateType(NODE* ast, TOKEN** tokens) {
if (ast==0) {
fprintf(stderr,"src/compiler/types.cz:124:45\n");fprintf(stderr,"Invalid input to generateType");exit(1);
;
};
NODE** childs;
childs = (NODE**)ast->children;
if (ast->type==3) {
TOKEN* token=tokens[ast->tokensStart];
if (token->type==5) {
TYPE* typ=generateType(childs[0], tokens);
typ->isMemMove=true;
;
return typ;
;
};
if (token->type==6) {
TYPE* typ=generateType(childs[0], tokens);
typ->isOptional=true;
;
return typ;
;
};
TYPE* typ=malloc(sizeof(TYPE));
if (token->type==60) {
typ->variant=0;
;
}
else if (token->type==59) {
typ->variant=2;
;
}
else if (token->type==58) {
typ->variant=3;
;
}
else if (token->type==61) {
typ->variant=4;
;
}
else if (token->type==27) {
typ->variant=6;
typ->name=toSTR(token->value);
;
}
else if (token->type==63) {
typ->variant=5;
typ->dereference=generateType(childs[0], tokens);
;
}
else if (token->type==62) {
typ->variant=1;
;
}
else {
fprintf(czStdOut,"%d", token->type); fputs("\n",czStdOut);
fprintf(stderr,"src/compiler/types.cz:163:32\n");fprintf(stderr,"Unknown type");exit(1);
;
};
;
return typ;
;
};
fprintf(stderr,"src/compiler/types.cz:169:41\n");fprintf(stderr,"Invalid input to generateType");exit(1);
;
}
void CZ_TYPE_printType(TYPE* this) {
if (this==0) {
fprintf(stderr,"src/compiler/types.cz:17:41\n");fprintf(stderr,"invalid type to print");exit(1);
;
};
if (this->isMemMove) {
fprintf(czStdOut,"&");
;
};
if (this->isOptional) {
fprintf(czStdOut,"Optional<");
;
};
if (this->variant==0) {
fprintf(czStdOut,"void");
;
}
else if (this->variant==2) {
fprintf(czStdOut,"char");
;
}
else if (this->variant==3) {
fprintf(czStdOut,"int");
;
}
else if (this->variant==4) {
fprintf(czStdOut,"double");
;
}
else if (this->variant==1) {
fprintf(czStdOut,"bool");
;
}
else if (this->variant==5) {
fprintf(czStdOut,"Pointer<");
CZ_TYPE_printType(this->dereference);
fprintf(czStdOut,">");
;
}
else if (this->variant==6) {
fprintf(czStdOut,"Struct<");
fprintf(czStdOut,"%s", this->name->ptr);
fprintf(czStdOut,">");
;
};
if (this->isOptional) {
fprintf(czStdOut,">");
;
};
;
}
void CZ_TYPE_toC(TYPE* this) {
if (this==0) {
fprintf(stderr,"src/compiler/types.cz:52:41\n");fprintf(stderr,"invalid type to print");exit(1);
;
};
if (this->variant==0) {
fprintf(czStdOut,"void");
;
}
else if (this->variant==2) {
fprintf(czStdOut,"char");
;
}
else if (this->variant==3) {
fprintf(czStdOut,"int");
;
}
else if (this->variant==4) {
fprintf(czStdOut,"double");
;
}
else if (this->variant==1) {
fprintf(czStdOut,"bool");
;
}
else if (this->variant==5) {
CZ_TYPE_toC(this->dereference);
fprintf(czStdOut,"*");
;
}
else if (this->variant==6) {
fprintf(czStdOut,"%s", this->name->ptr);
fprintf(czStdOut,"*");
;
};
;
}
STR* CZ_TYPE_toSTR(TYPE* this) {
STR* str=malloc(sizeof(STR));
CZ_STR_alloc(str, 10);
CZ_TYPE_appendSTR(this, str);
;
return str;
;
}
void CZ_TYPE_appendSTR(TYPE* this, STR* str) {
if (this->isMemMove) {
CZ_STR_appendPtr(str, "&");
;
};
if (this->isOptional) {
CZ_STR_appendPtr(str, "?");
;
};
if (this->variant==0) {
CZ_STR_appendPtr(str, "void");
;
}
else if (this->variant==2) {
CZ_STR_appendPtr(str, "char");
;
}
else if (this->variant==3) {
CZ_STR_appendPtr(str, "int");
;
}
else if (this->variant==4) {
CZ_STR_appendPtr(str, "double");
;
}
else if (this->variant==1) {
CZ_STR_appendPtr(str, "bool");
;
}
else if (this->variant==5) {
CZ_STR_appendPtr(str, "Pointer<");
CZ_TYPE_appendSTR(this->dereference, str);
CZ_STR_appendPtr(str, ">");
;
}
else if (this->variant==6) {
CZ_STR_appendPtr(str, this->name->ptr);
;
};
;
}
TYPE* CZ_TYPE_copy(TYPE* this) {
TYPE* typ=malloc(sizeof(TYPE));
typ->variant=this->variant;
if (this->name!=0) {
typ->name=CZ_STR_copy(this->name);
;
}
else {
typ->name=0;
;
};
typ->isMemMove=this->isMemMove;
typ->isOptional=this->isOptional;
if (this->dereference!=0) {
typ->dereference=CZ_TYPE_copy(this->dereference);
;
};
;
return typ;
;
}
#endif

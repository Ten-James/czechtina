#ifndef CZECHTINA_czechtina__parser__programAST
#define CZECHTINA_czechtina__parser__programAST
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "czechtina__compiler__functions.c"
#include "czechtina__parser__ast.c"
#include "czechtina__compiler__enums.c"
#include "czechtina__compiler__strcts.c"
#include "czechtina__compiler__types.c"
#include "std__str.c"
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _PROGRAMAST PROGRAMAST;
typedef struct _PROGRAMAST {
STR* packageName;
STR** imports;
int iSize;
int iReserved;
ENUM** enums;
int eSize;
int eReserved;
STRCTENTRY** strcts;
int sSize;
int sReserved;
FunctionNode** functions;
int fSize;
int fReserved;
FunctionNode* main;
} PROGRAMAST;
void CZ_PROGRAMAST_init(PROGRAMAST* this);
void CZ_PROGRAMAST_consumeStruct(PROGRAMAST* this, STRCTENTRY* stru);
void CZ_PROGRAMAST_consumeEnum(PROGRAMAST* this, ENUM* en);
void CZ_PROGRAMAST_consumeImport(PROGRAMAST* this, STR* import);
void CZ_PROGRAMAST_consumeFunction(PROGRAMAST* this, FunctionNode* function);
void CZ_PROGRAMAST_consumeMain(PROGRAMAST* this, FunctionNode* main);
void CZ_PROGRAMAST_init(PROGRAMAST* this) {
this->imports=malloc(sizeof(STR*) *(10));
this->iSize=0;
this->iReserved=10;
this->functions=malloc(sizeof(FunctionNode*) *(5));
this->fSize=0;
this->fReserved=5;
this->enums=malloc(sizeof(ENUM*) *(5));
this->eSize=0;
this->eReserved=5;
this->strcts=malloc(sizeof(STRCTENTRY*) *(5));
this->sSize=0;
this->sReserved=5;
this->packageName=0;
this->main=0;
;
}
void CZ_PROGRAMAST_consumeStruct(PROGRAMAST* this, STRCTENTRY* stru) {
if (this->sSize==this->sReserved) {
this->sReserved*=2;
this->strcts=realloc(this->strcts, this->sReserved * sizeof(STRCTENTRY*));;
;
};
this->strcts[this->sSize]=stru;
this->sSize+=1;
;
}
void CZ_PROGRAMAST_consumeEnum(PROGRAMAST* this, ENUM* en) {
if (this->eSize==this->eReserved) {
this->eReserved*=2;
this->enums=realloc(this->enums, this->eReserved * sizeof(ENUM*));;
;
};
this->enums[this->eSize]=en;
this->eSize+=1;
;
}
void CZ_PROGRAMAST_consumeImport(PROGRAMAST* this, STR* import) {
if (this->iSize==this->iReserved) {
this->iReserved*=2;
this->imports=realloc(this->imports, this->iReserved * sizeof(STR*));;
;
};
this->imports[this->iSize]=import;
this->iSize+=1;
;
}
void CZ_PROGRAMAST_consumeFunction(PROGRAMAST* this, FunctionNode* function) {
if (this->fSize==this->fReserved) {
this->fReserved*=2;
this->functions=realloc(this->functions, this->fReserved * sizeof(FunctionNode*));;
;
};
this->functions[this->fSize]=function;
this->fSize+=1;
;
}
void CZ_PROGRAMAST_consumeMain(PROGRAMAST* this, FunctionNode* main) {
this->main=main;
;
}
#endif

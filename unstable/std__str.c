#ifndef CZECHTINA_std__str
#define CZECHTINA_std__str
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _STR STR;
STR* toSTR(char* ptr);
bool ptrEqual(char* a, char* b);
bool ptrStartsWith(char* haystack, char* needle);
typedef struct _STR {
char* ptr;
int size;
int reserved;
} STR;
void CZ_STR_init(STR* this);
void CZ_STR_destruct(STR* this);
STR* CZ_STR_copy(STR* this);
void CZ_STR_alloc(STR* this, int size);
void CZ_STR_realloc(STR* this);
void CZ_STR_appendChar(STR* this, char c);
void CZ_STR_appendPtr(STR* this, char* ptr);
void CZ_STR_appendInt(STR* this, int val);
bool CZ_STR_isEndingWith(STR* this, char* ptr);
STR* toSTR(char* ptr) {
STR* str=malloc(sizeof(STR));
CZ_STR_init(str);
int len=strlen(ptr);
CZ_STR_alloc(str, len+10);
CZ_STR_appendPtr(str, ptr);
;
return str;
;
}
bool ptrEqual(char* a, char* b) {
int i=0;
while (a[i]!=0&&b[i]!=0) {
if (a[i]!=b[i]) {
;
return false;
;
};
i+=1;
;
};
if (b[i]!=0||a[i]!=0) {
;
return false;
;
};
;
return true;
;
}
bool ptrStartsWith(char* haystack, char* needle) {
int i=0;
while (needle[i]!=0) {
if (haystack[i]!=needle[i]) {
;
return false;
;
};
i+=1;
;
};
;
return true;
;
}
void CZ_STR_init(STR* this) {
this->ptr=0;
this->size=0;
this->reserved=0;
;
}
void CZ_STR_destruct(STR* this) {
if (this->ptr!=0) {
free(this->ptr);
;
};
;
}
STR* CZ_STR_copy(STR* this) {
STR* str=malloc(sizeof(STR));
CZ_STR_init(str);
CZ_STR_alloc(str, this->size+10);
CZ_STR_appendPtr(str, this->ptr);
;
return str;
;
}
void CZ_STR_alloc(STR* this, int size) {
if (this->ptr!=0) {
free(this->ptr);
;
};
this->ptr=malloc(sizeof(char*) *(size));
this->size=0;
this->reserved=size;
;
}
void CZ_STR_realloc(STR* this) {
this->reserved=this->reserved*2+10;
this->ptr = realloc(this->ptr, this->reserved * sizeof(char));
;
}
void CZ_STR_appendChar(STR* this, char c) {
if (this->reserved<this->size+1) {
CZ_STR_realloc(this);
;
};
this->ptr[this->size]=c;
this->size+=1;
this->ptr[this->size]=0;
;
}
void CZ_STR_appendPtr(STR* this, char* ptr) {
int len=strlen(ptr);
while (this->reserved<this->size+len) {
CZ_STR_realloc(this);
;
};
int pos=0;
while (pos<len) {
this->ptr[this->size+pos]=ptr[pos];
pos+=1;
;
};
this->size+=pos;
this->ptr[this->size]=0;
;
}
void CZ_STR_appendInt(STR* this, int val) {
if (val==0) {
CZ_STR_appendChar(this, '0');
;
return;
;
};
if (val<0) {
CZ_STR_appendChar(this, '-');
val=0-val;
;
};
if (val>0) {
if (val/10>0) {
CZ_STR_appendInt(this, val/10);
;
};
CZ_STR_appendChar(this, (val % 10)+'0');
;
};
;
}
bool CZ_STR_isEndingWith(STR* this, char* ptr) {
int len=strlen(ptr);
if (this->size<len) {
;
return false;
;
};
int pos=0;
while (pos<len) {
if (this->ptr[this->size-len+pos]!=ptr[pos]) {
;
return false;
;
};
pos+=1;
;
};
;
return true;
;
}
#endif

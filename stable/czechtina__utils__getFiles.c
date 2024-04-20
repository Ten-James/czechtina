#ifndef CZECHTINA_czechtina__utils__getFiles
#define CZECHTINA_czechtina__utils__getFiles
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "std.c"
#include "std__str.c"
#include <dirent.h>
#include <sys/stat.h>
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _FILES FILES;
typedef struct _FILES {
STR** arr;
int size;
int reserved;
} FILES;
void CZ_FILES_init(FILES* this);
void CZ_FILES_append(FILES* this, STR* str);
void CZ_FILES_readDir(FILES* this, char* dir);
void CZ_FILES_init(FILES* this) {
this->arr=malloc(sizeof(STR*) *(5));
this->size=0;
this->reserved=5;
;
}
void CZ_FILES_append(FILES* this, STR* str) {
if (this->size==this->reserved) {
this->reserved*=2;
this->arr=realloc(this->arr, this->reserved * sizeof(STR));
;
};
this->arr[this->size]=str;
this->size+=1;
;
}
void CZ_FILES_readDir(FILES* this, char* dir) {

        struct dirent *entry;
        struct stat statbuf;

        DIR *directory = opendir(dir);

        if (directory == NULL) {
            return;
        }
        ;
while ((entry = readdir(directory)) != NULL) {
if (entry->d_name[0] == '.') {
continue;
;
};
STR* str=toSTR(dir);
CZ_STR_appendPtr(str, "/");
CZ_STR_appendPtr(str, entry->d_name);
if (stat(str->ptr, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
CZ_FILES_readDir(this, str->ptr);
continue;
;
};
if (CZ_STR_isEndingWith(str, ".cz")) {
CZ_FILES_append(this, str);
;
};
//somehow destruct: str 
;
};
;
}
#endif

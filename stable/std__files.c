#ifndef CZECHTINA_std__files
#define CZECHTINA_std__files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
typedef struct _FILEWRITER FILEWRITER;
char* readFile(char* path);
bool writeFile(char* path, char* content);
bool fileExists(char* path);
typedef struct _FILEWRITER {
char* path;
void* stream;
} FILEWRITER;
void CZ_FILEWRITER_init(FILEWRITER* this, char* path);
bool CZ_FILEWRITER_open(FILEWRITER* this);
bool CZ_FILEWRITER_write(FILEWRITER* this, char* content);
bool CZ_FILEWRITER_close(FILEWRITER* this);
void CZ_FILEWRITER_destruct(FILEWRITER* this);
char* readFile(char* path) {

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *fileContent = (char *)malloc((fileSize + 1) * sizeof(char));
    if (fileContent == NULL) {
        fprintf(stderr, "Memory allocation error");
        fclose(file);
        return NULL;
    }

    fread(fileContent, sizeof(char), fileSize, file);
    fileContent[fileSize] = 0;

    fclose(file);
    return fileContent;
    ;
;
}
bool writeFile(char* path, char* content) {

    FILE *file = fopen(path, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s", path);
        return false;
    }

    fprintf(file, "%s", content);
    fclose(file);
    return true;
    ;
;
}
bool fileExists(char* path) {

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return false;
    }
    fclose(file);
    return true;
    ;
;
}
void CZ_FILEWRITER_init(FILEWRITER* this, char* path) {
this->path=path;
this->stream=0;
CZ_FILEWRITER_open(this);
;
}
bool CZ_FILEWRITER_open(FILEWRITER* this) {

        this->stream = fopen(this->path, "w");
        if (this->stream == NULL) {
            fprintf(stderr, "Error opening file %s", this->path);
            return false;
        }
        return true;
        ;
;
}
bool CZ_FILEWRITER_write(FILEWRITER* this, char* content) {

        if (this->stream == NULL) {
            fprintf(stderr, "File not opened");
            return false;
        }
        fprintf(this->stream, "%s", content);
        return true;
        ;
;
}
bool CZ_FILEWRITER_close(FILEWRITER* this) {

        if (this->stream == NULL) {
            fprintf(stderr, "File not opened");
            return false;
        }
        fclose(this->stream);
        return true;
        ;
;
}
void CZ_FILEWRITER_destruct(FILEWRITER* this) {

        if (this->stream != NULL) {
            fclose(this->stream);
        }
        ;
;
}
#endif

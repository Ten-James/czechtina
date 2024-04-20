#ifndef CZECHTINA_std__paths
#define CZECHTINA_std__paths
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
bool dirExist(char* pathName);
bool createDir(char* pathName);
bool dirExist(char* pathName) {
 
        DIR *dir = opendir(pathName);
        if (dir) {
            closedir(dir);
            return true;
        } else {
            return false;
        }
    ;
;
}
bool createDir(char* pathName) {

        if (mkdir(pathName, 0777) == 0) {
            return true;
        } else {
            return false;
        }
    ;
;
}
#endif

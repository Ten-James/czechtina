#ifndef CZECHTINA_czechtina__utils__levenshtein
#define CZECHTINA_czechtina__utils__levenshtein
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
extern void* czStdOut;
extern void* czStdIn;
typedef void* FileStream;
int levenshteinDistance(char* s1, char* s2);
int levenshteinDistance(char* s1, char* s2) {

    #define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    if (s1len >254 || s2len > 254) return -1; // since we dont use VLA
    unsigned int column[255];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x - 1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y - 1] + 1, lastdiag + (s1[y-1] == s2[x - 1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return column[s1len];
;
;
}
#endif

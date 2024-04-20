#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "std__args.c"
#include "czechtina__cli.c"
void* czStdOut = 0;
void* czStdIn = 0;
typedef void* FileStream;
int main(int argc, char** argv) {
czStdOut = stdout;
ARGS* arg=malloc(sizeof(ARGS));
CZ_ARGS_init(arg, argc, argv);
;
return cli(arg);
;
}

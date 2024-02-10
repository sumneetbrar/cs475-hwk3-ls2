#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define MAX 2048
#define INDENT "    "

// Declare the functions
void printDir(char* path, int depth);
void printModDir(char *path, char *match, int depth);
void output(stack_t *s);
char* pTs(void* ptr);

#endif

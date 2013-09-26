#ifndef CLIB_H
#define CLIB_H
#include <stddef.h>

char *strncat(char *, const char *, size_t n);

int fprintf(int, const char *, ...);


/*
Implement a Non-standard itoa
return address of local static(thanks jserv)
*/
char *itoa(int number, int base);

#endif

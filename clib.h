#ifndef CLIB_H
#define CLIB_H
#include <stddef.h>

void *memcpy(void * restrict dest, const void * restrict src, size_t n);
#ifdef ENABLE_ASM_OPT
int strcmp(const char *a, const char *b) __attribute__ ((naked));
size_t strlen(const char *s) __attribute__ ((naked));
#else
int strcmp(const char *a, const char *b);
size_t strlen(const char *s);
#endif
int strncmp(const char *a, const char *b, size_t n);

char *strncat(char *, const char *, size_t n);

int fprintf(int, const char *, ...);


/*
Implement a Non-standard itoa
return address of local static(thanks jserv)
*/
char *itoa(int number, int base);

#endif

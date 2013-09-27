#ifndef SHELL_H
#define SHELL_H

#include "stm32f10x.h"
#include "RTOSConfig.h"
#include "syscall.h"
#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t n);
int strcmp(const char *a, const char *b) __attribute__ ((naked));
int strcmp(const char *a, const char *b);
size_t strlen(const char *s) __attribute__ ((naked));
size_t strlen(const char *s);
void puts(char *s);
void shell_puts(int fdout,char *s);


#endif


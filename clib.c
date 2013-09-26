#include "clib.h"
#include "stm32f10x.h"
#include "RTOSConfig.h"
#include <stdarg.h>
#include "syscall.h"


//#pragma GCC diagnostic ignored "-pedantic"

int strcmp(const char *a, const char *b)
{
#ifndef ENABLE_ASM_OPT
	while(*a && (*a==*b))
		++a, ++b;
	return *a-*b;
#else
	__asm(
        "strcmp_lop:                \n"
        "   ldrb    r2, [r0],#1     \n"
        "   ldrb    r3, [r1],#1     \n"
        "   cmp     r2, #1          \n"
        "   it      hi              \n"
        "   cmphi   r2, r3          \n"
        "   beq     strcmp_lop      \n"
		"	sub     r0, r2, r3  	\n"
        "   bx      lr              \n"
		:::);
#endif
}



size_t strlen(const char *s)
{
#ifndef ENABLE_ASM_OPT
	int count=0;
	while(*s++)
		++count;
	return count;
#else
	
	__asm(
	"	sub  r3, r0, #1			\n"
        "strlen_loop:               \n"
		"	ldrb r2, [r3, #1]!		\n"
		"	cmp  r2, #0				\n"
        "   bne  strlen_loop        \n"
		"	sub  r0, r3, r0			\n"
		"	bx   lr					\n"
		:::
	);
#endif
}

#ifndef ENABLE_ASM_OPT
void *memcpy(void * restrict dest, const void * restrict src, size_t n){
	int i;
	for(i=0; i<n; ++i)
		*((unsigned char *)dest+i)=*((unsigned char *)src+i);
	return dest;
}
#endif

char *strncat(char *str1, const char *str2, size_t n){
	int start=strlen(str1);
	char *p;
	for(p=&str1[start]; p-&str1[start]<n; ++p){
		*p=str2[p-&str1[start]];
		if(*p=='\0') break;
	}
}


/*%d %s %c supported*/
int fprintf(int fd, const char *format, ...){
	va_list v1;
	va_start(v1, format);

	int i=0;
	int argtmpi, tmp2;
	char *argtmpcp, argtmpc;
	char tmpbuf[20]={0};
	while(format[i]!='\0'){
		if(format[i]=='%'){
			switch(format[i+1]){
				case '%':
					write(fd, "%", 2); i+=2; continue;
				case 'd':
				case 'x':
					argtmpi=va_arg(v1, int);
					if(format[i+1]=='d')
						argtmpcp=itoa(argtmpi, 10);
					else
						argtmpcp=itoa(argtmpi, 16);
					write(fd, argtmpcp, strlen(argtmpcp)+1);
					i+=2; continue;
					
				case 's':
					argtmpcp=va_arg(v1, char *);
					write(fd, argtmpcp, strlen(argtmpcp)+1);
					i+=2; continue;
				case 'c':
					argtmpc=va_arg(v1, int);
					tmpbuf[0]=argtmpc; tmpbuf[1]='\0';
					write(fd, tmpbuf, 2);
					i+=2; continue;
				default:;
					/*ignore unknown specifiers*/
			}
		}else{
			tmpbuf[0]=format[i];tmpbuf[1]='\0';
			write(fd, tmpbuf, 2);
		}
		++i;
	}

	va_end(v1);
}

char *itoa(int number, int base){
	static char buffer[32]={0};
	if(number==0){
		//if number is zero, for loop below will not run
		buffer[30]='0';
		return &buffer[30];
	}
	/*negative part is not elegant, i will fix it later*/
	int negative=(number<0);
	if(negative) number=-number;
	int i=30;
	for(;number && i; --i, number/=base)
		buffer[i]= "0123456789ABCDEF" [number%base];
	if(negative){
		buffer[i]='-';
		--i;
	}
	return &buffer[i+1];
}

/*
void puts(const char *s)
{
	while (*s) {
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		 ;
		USART_SendData(USART2, *s);
		s++;
	}
}*/

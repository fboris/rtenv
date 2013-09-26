#include "clib.h"
#include "stm32f10x.h"
#include "RTOSConfig.h"
#include <stdarg.h>
#include "syscall.h"


//#pragma GCC diagnostic ignored "-pedantic"





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

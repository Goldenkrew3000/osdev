#ifndef _UART_H
#define _UART_H

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

#define TO_HEX_DIGIT(n) ('0' + (n) + ((n) < 10 ? 0 : 'a' - '0' - 10))

void ns16550a_init(void);
uint8_t uart_get(void);
int kputchar(int);
int kputs(const char *);
int toupper(int);
void kvprintf(const char *, va_list);
void kprintf(const char *, ...);

#endif

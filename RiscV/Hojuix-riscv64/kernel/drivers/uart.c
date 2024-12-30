// NS16550A UART Driver
// Hojuix 2024

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <limits.h>
#include <drivers/uart.h>

#define UART_ADDR 0x10000000

void ns16550a_init() {
    volatile uint8_t* uart_ptr = (uint8_t*)UART_ADDR;

    // Set word length to 8
    const uint8_t LCR = 0b11;
    uart_ptr[3] = LCR;
    
    // Enable FIFO
    uart_ptr[2] = 0b1;

    // Enable receiver buffer interrupts
    uart_ptr[1] = 0b1;

    // NOTE: For real UART, we need to set the baudrate
    // Example at https://github.com/DonaldKellett/marvelos/blob/main/src/uart/uart.c
}

static void uart_put(uint8_t character) {
    *(uint8_t*)UART_ADDR = character;
}

uint8_t uart_get(void) {
    return *(uint8_t*)UART_ADDR;
}

int kputchar(int character) {
    uart_put((uint8_t)character);
    return character;
}

static void kprint(const char *str) {
    while (*str) {
        kputchar((int)*str);
        ++str;
    }
}

int kputs(const char *str) {
    kprint(str);
    kputchar((int)'\n');
    return 0;
}

int toupper(int c) {
    return 'a' <= c && c <= 'z' ? c + 'A' - 'a' : c;
}

// NOTE: Copied. Limited version of vprintf()
void kvprintf(const char *format, va_list arg) {
    while (*format) {
        if (*format == '%') {
        ++format;
        if (!*format)
        return;
        switch (*format) {
        case 'd':
        case 'i':
        {
        int n = va_arg(arg, int);
        if (n == INT_MIN) {
            kprint("-2147483648");
            break;
        }
        if (n < 0) {
            kputchar('-');
            n = ~n + 1;
        }
        char lsh = '0' + n % 10;
        n /= 10;
        char buf[9];
        char *p_buf = buf;
        while (n) {
            *p_buf++ = '0' + n % 10;
            n /= 10;
        }
        while (p_buf != buf)
            kputchar(*--p_buf);
        kputchar(lsh);
        }
        break;
        case 'u':
        {
        unsigned n = va_arg(arg, unsigned);
        char lsh = '0' + n % 10;
        n /= 10;
        char buf[9];
        char *p_buf = buf;
        while (n) {
            *p_buf++ = '0' + n % 10;
            n /= 10;
        }
        while (p_buf != buf)
            kputchar(*--p_buf);
        kputchar(lsh);
        }
        break;
        case 'o':
        {
        unsigned n = va_arg(arg, unsigned);
        char lsh = '0' + n % 8;
        n /= 8;
        char buf[10];
        char *p_buf = buf;
        while (n) {
            *p_buf++ = '0' + n % 8;
            n /= 8;
        }
        while (p_buf != buf)
            kputchar(*--p_buf);
        kputchar(lsh);
        }
        break;
        case 'x':
        {
        unsigned n = va_arg(arg, unsigned);
        char lsh = TO_HEX_DIGIT(n % 16);
        n /= 16;
        char buf[7];
        char *p_buf = buf;
        while (n) {
            *p_buf++ = TO_HEX_DIGIT(n % 16);
            n /= 16;
        }
        while (p_buf != buf)
            kputchar(*--p_buf);
        kputchar(lsh);
        }
        break;
        case 'X':
        {
        unsigned n = va_arg(arg, unsigned);
        char lsh = TO_HEX_DIGIT(n % 16);
        n /= 16;
        char buf[7];
        char *p_buf = buf;
        while (n) {
            *p_buf++ = TO_HEX_DIGIT(n % 16);
            n /= 16;
        }
        while (p_buf != buf)
            kputchar(toupper(*--p_buf));
        kputchar(toupper(lsh));
        }
        break;
        case 'c':
        kputchar(va_arg(arg, int));
        break;
        case 's':
        kprint(va_arg(arg, char *));
        break;
        case 'p':
        {
        kprint("0x");
        size_t ptr = va_arg(arg, size_t);
        char lsh = TO_HEX_DIGIT(ptr % 16);
        ptr /= 16;
        char buf[15];
        char *p_buf = buf;
        while (ptr) {
            *p_buf++ = TO_HEX_DIGIT(ptr % 16);
            ptr /= 16;
        }
        while (p_buf != buf)
            kputchar(*--p_buf);
        kputchar(lsh);
        }
        break;
        case '%':
        kputchar('%');
        break;
        default:
        kputchar('%');
        kputchar(*format);
        }
        } else
        kputchar(*format);
        ++format;
    }
}

void kprintf(const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    kvprintf(format, arg);
    va_end(arg);
}

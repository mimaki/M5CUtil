// #pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <inttypes.h>

//
// mtstdio.cpp
//
int m5putc(int c, FILE *stream);
int m5putchar(int c);
int m5puterrchar(int c);
int m5fputs(const char *s, FILE *stream);
int m5puts(const char *s);
// int m5fprintf(FILE *stream, const char *format, ...);
int m5printf(const char *format, ...);

//
// m5util.cpp
//
void m5delay(uint32_t ms);

//
// m5i2c.cpp
//
void m5i2c_init(void);
int m5i2c_write(uint8_t addr, const uint8_t *data, size_t len, uint8_t stop);
size_t m5i2c_read(uint8_t addr, uint8_t *data, size_t len, uint8_t stop);


//
// m2gpio.cpp
//
void m5gpio_init(void);
void m5gpio_write(int pin, int level);


#ifdef __cplusplus
}
#endif

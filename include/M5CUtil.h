#pragma once
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

#ifdef __cplusplus
}
#endif

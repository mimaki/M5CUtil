#include "M5CUtil.h"
#include "M5Unified.h"
#include <malloc.h>

int m5putc(int c, FILE *stream)
{
  putc(c, stream);
  fflush(stream);
  M5.Lcd.printf("%c", c);
  M5.update();
  return c;
}

int m5putchar(int c)
{
  return m5putc(c, stdout);
}

int m5puterrchar(int c)
{
  return m5putc(c, stderr);
}

int m5fputs(const char *s, FILE *stream)
{
  if (s == NULL) return 0;
  size_t len = strlen(s);
  for (size_t i = 0; i < len; i++) {
    m5putc(s[i], stream);
  }
  return len;
}

int m5puts(const char *s)
{
  int len = m5fputs(s, stdout);
  m5putc('\n', stdout);
  return len + 1; // +1 for newline
}

#define SPRINTF_BUFFER_SIZE 256
// int m5fprintf(FILE *stream, const char *format, ...)
// {
//   int ret = 0;
//   va_list args;
//   va_start(args, format);
//   // int ret = vsprintf(NULL, format, args);
//   char *buf = (char *)malloc(SPRINTF_BUFFER_SIZE);
//   if (buf) {
//     vsnprintf(buf, SPRINTF_BUFFER_SIZE, format, args);
//     m5fputs(buf, stream);
//     free(buf);
//   }
//   va_end(args);
//   return ret;
// }

int m5printf(const char *format, ...)
{
  int ret = 0;
  va_list args;
  va_start(args, format);
  char *buf = (char *)malloc(SPRINTF_BUFFER_SIZE);
  if (buf) {
    vsnprintf(buf, SPRINTF_BUFFER_SIZE, format, args);
    m5fputs(buf, stdout);
    free(buf);
  }
  va_end(args);
  return ret;
}

void m5delay(uint32_t ms)
{
  vTaskDelay(pdMS_TO_TICKS(ms));
}

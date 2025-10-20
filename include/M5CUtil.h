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


//
// lcd.cpp
//
void m5lcd_init(void);
void m5lcd_set_brightness(uint8_t brightness);
void m5lcd_draw_pixel(int16_t x, int16_t y, uint16_t color);
void m5lcd_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void m5lcd_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void m5lcd_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void m5lcd_fill_screen(uint16_t color);
void m5lcd_clear(void);
void m5lcd_circle(int16_t x, int16_t y, int16_t r, uint16_t color);
void m5lcd_fill_circle(int16_t x, int16_t y, int16_t r, uint16_t color);
void m5lcd_draw_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);
void m5lcd_fill_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);
void m5lcd_draw_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void m5lcd_fill_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void m5lcd_draw_bitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *data);
void m5lcd_draw_char(int16_t x, int16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
void m5lcd_set_cursor(int16_t x, int16_t y);
void m5lcd_set_text_color(uint16_t fc);
void m5lcd_set_text_size(uint8_t s);
void m5lcd_set_text_wrap(uint8_t wx, uint8_t wy);

//
// m5accel
//
void m5accel_read(float *x, float *y, float *z);

//
// usb_serial
//
void usb_serial_init(void);
void usb_serial_send_byte(uint8_t b);
void usb_serial_send_bytes(uint8_t *b, size_t len);
int16_t usb_serial_receive_byte(void);
size_t usb_serial_receive_bytes(uint8_t *b, size_t len);

// mrb file list
struct tag_dir_entry {
  struct tag_dir_entry *next;
  char *name;
};
typedef struct tag_dir_entry dir_entry;

dir_entry *list_mrb_files(const char *path);
void free_dir_list(dir_entry *head);

#ifdef __cplusplus
}
#endif

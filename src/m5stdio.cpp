#include "M5Unified.h"
#include "M5GFX.h"
#include "M5CUtil.h"
#include <malloc.h>

#ifdef __cplusplus
extern "C" {
#endif

// #include "hal/uart_ll.h"
// #include "soc/uart_periph.h"

// #include "tinyusb.h"
// #include "tusb_cdc_acm.h"
// #include "tusb.h"
// #include "tusb_cdc_acm.h"
// #include "driver/usb_serial_jtag.h"

// #define UART_TXFIFO_MAX_SIZE 127  // UART0のTX FIFOの最大サイズ

// static inline void m5uart_send_byte(uint8_t c)
// {
//   // UART0 の FIFO に直接書き込み
//   while (uart_ll_get_txfifo_len(&UART0) >= UART_TXFIFO_MAX_SIZE - 1) {
//       ; // FIFOが空くまで待つ（最大127バイト）
//   }
//   uart_ll_write_txfifo(&UART0, &c, 1);
//   // while (!(UART0.status & UART_TXFIFO_EMPTY)); // wait until TX FIFO empty
//   // UART0.fifo = c;

//   // 送信完了待ち（オプション）
//   while (uart_ll_get_txfifo_len(&UART0) > 0) ;
// } 

// int m5usb_init(void)
// {
//   // TinyUSB初期化
//   // tusb_init();
//   // CDC-ACM初期化
//   // tud_cdc_acm_init();

//   // // USBシリアルJTAG初期化
//   // usb_serial_jtag_driver_install();

//   // ドライバを初期化
//   const usb_serial_jtag_driver_config_t usb_serial_config = {
//     .tx_buffer_size = 64,  // 書き込みバッファサイズ
//     .rx_buffer_size = 64   // 読み込みバッファサイズ
//   };
//   ESP_ERROR_CHECK(usb_serial_jtag_driver_install((usb_serial_jtag_driver_config_t*)&usb_serial_config));

//   return 0;
// }

int m5putc(int c, FILE *stream)
{
  putc(c, stream);
  fflush(stream);
  // uart_write_bytes(UART_NUM_0, (const char *)&c, 1);
  // uart_wait_tx_done(UART_NUM_0, pdMS_TO_TICKS(20));
  // m5uart_send_byte((uint8_t)c);
  // tud_cdc_write_char(c);
  // tud_cdc_write_flush();
  // usb_serial_jtag_write_bytes(&c, 1, 0);
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

int m5getc(FILE *stream)
{
  int c = -1;
  while (1) {
    c = fgetc(stream);
    if (c != EOF) break;
    M5.update();
    vTaskDelay(1);
  }
  if (c >= ' ') {
    m5putchar(c); // Echo back
  }
  else if (c == '\n' || c == '\r') {
    c = '\n';
    m5putchar(c); // Echo back
  }
  // else if (c == '\r') {
  //   m5putchar('\n'); // Echo back as newline
  //   c = '\n';
  // }
  return c;
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

// m5util.cppにいれたい
void m5delay(uint32_t ms)
{
  vTaskDelay(pdMS_TO_TICKS(ms));
}

// m5i2c.cppにいれたい
#include "driver/i2c.h"

#define I2C_MASTER_NUM        I2C_NUM_0 // I2C ポート番号
#define I2C_MASTER_SDA_IO     2         // SDA ピン
#define I2C_MASTER_SCL_IO     1         // SCL ピン
#define I2C_MASTER_FREQ_HZ    400000    // 400kHz
#define I2C_MASTER_TIMEOUT_MS 1000

void m5i2c_init(void)
{
  // static const i2c_config_t conf = {
  //   .mode = I2C_MODE_MASTER,
  //   .sda_io_num = I2C_MASTER_SDA_IO,
  //   .scl_io_num = I2C_MASTER_SCL_IO,
  //   .sda_pullup_en = GPIO_PULLUP_ENABLE,
  //   .scl_pullup_en = GPIO_PULLUP_ENABLE,
  //   .master = {
  //       .clk_speed = I2C_MASTER_FREQ_HZ,
  //   },
  //   .clk_flags = 0
  // };
  // ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
  // ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));

  // Groveポート I2C初期化 (Port0, SDA=2, SCL=1)
  bool result = M5.Ex_I2C.begin(I2C_MASTER_NUM, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);

  // m5printf("I2C initialized: %s\n", result ? "OK" : "Failed");
}

int m5i2c_write(uint8_t addr, const uint8_t *data, size_t len, uint8_t stop)
{
  // esp_err_t ret = i2c_master_write_to_device(I2C_MASTER_NUM, addr, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  // M5.Ex_I2C.beginTransmission(addr);
  M5.Ex_I2C.start(addr, false, I2C_MASTER_FREQ_HZ);
  bool result = M5.Ex_I2C.write(data, len);
  // int ret = M5.Ex_I2C.endTransmission(stop ? 1 : 0);
  if (stop) {
    M5.Ex_I2C.stop();
  }
  // m5printf("m5i2c_write: addr=0x%02x, len=%d, ret=%d\n", addr, len, result ? 0 : -1);
  return result ? 0 : -1;
}

size_t m5i2c_read(uint8_t addr, uint8_t *data, size_t len, uint8_t stop)
{
  // esp_err_t ret = i2c_master_read_from_device(I2C_MASTER_NUM, addr, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  // M5.Ex_I2C.requestFrom((int)addr, len);
  M5.Ex_I2C.start(addr, true, I2C_MASTER_FREQ_HZ);
  // size_t i = 0;
  // for (i=0; i<len; i++) {
  //   data[i] = (uint8_t)M5.Ex_I2C.read();
  // }
  int result = M5.Ex_I2C.read(data, len);
  M5.Ex_I2C.stop();
  // if (stop) M5.Ex_I2C.endTransmission(1);
  // int ret = result ? ESP_OK : ESP_FAIL;
  // m5printf("m5i2c_read: addr=0x%02x, len=%d, result=%d\n", addr, len, result);
  return result ? len : 0;
}

// m3gpio.cppにいれたい
#include "driver/gpio.h"

#define PIN_OUT1 5
#define PIN_OUT2 6
#define PIN_OUT3 7

void m5gpio_init(void)
{
  // 出力ピン設定
  gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << PIN_OUT1) | (1ULL << PIN_OUT2) | (1ULL << PIN_OUT3),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
  };
  gpio_config(&io_conf);
}

void m5gpio_write(int pin, int level)
{
  if (pin < 0 || pin > 2) return;
  int gpio_num = PIN_OUT1 + pin;
  gpio_set_level((gpio_num_t)gpio_num, level ? 1 : 0);
}

// lcd.cppに入れたい
M5GFX Lcd;

void m5lcd_init(void)
{
  Lcd.begin();
  Lcd.setRotation(1);
  Lcd.fillScreen(BLACK);
  Lcd.setTextColor(WHITE);
  Lcd.setTextSize(1);
  Lcd.setCursor(0, 0);
}

void m5lcd_set_brightness(uint8_t brightness)
{
  Lcd.setBrightness(brightness);
}

void m5lcd_draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  Lcd.drawPixel(x, y, color);
}

void m5lcd_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  Lcd.drawLine(x1, y1, x2, y2, color);
}

void m5lcd_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  Lcd.drawRect(x, y, w, h, color);
}

void m5lcd_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  Lcd.fillRect(x, y, w, h, color);
}

void m5lcd_fill_screen(uint16_t color)
{
  Lcd.fillScreen(color);
}

void m5lcd_clear(void)
{
  Lcd.fillScreen(BLACK);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
}

void m5lcd_circle(int16_t x, int16_t y, int16_t r, uint16_t color)
{
  Lcd.drawCircle(x, y, r, color);
}

void m5lcd_fill_circle(int16_t x, int16_t y, int16_t r, uint16_t color)
{
  Lcd.fillCircle(x, y, r, color);
}

void m5lcd_draw_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color)
{
  Lcd.drawTriangle(x1, y1, x2, y2, x3, y3, color);
}

void m5lcd_fill_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color)
{
  Lcd.fillTriangle(x1, y1, x2, y2, x3, y3, color);
}

void m5lcd_draw_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  Lcd.drawRoundRect(x, y, w, h, r, color);
}

void m5lcd_fill_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  Lcd.fillRoundRect(x, y, w, h, r, color);
}

void m5lcd_draw_bitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *data)
{
  Lcd.drawBitmap(x, y, w, h, data);
}

void m5lcd_draw_char(int16_t x, int16_t y, char c, uint16_t fc, uint16_t bc, uint8_t sz)
{
  Lcd.drawChar(x, y, c, bc, fc, sz);
}

void m5lcd_set_cursor(int16_t x, int16_t y)
{
  M5.Lcd.setCursor(x, y);
}

void m5lcd_set_text_color(uint16_t fc)
{
  M5.Lcd.setTextColor(fc);
}

void m5lcd_set_text_size(uint8_t s)
{
  M5.Lcd.setTextSize(s);
}

void m5lcd_set_text_wrap(uint8_t wx, uint8_t wy)
{
  M5.Lcd.setTextWrap((bool)wx, (bool)wy);
}

/// 加速度センサー
void m5accel_read(float *x, float *y, float *z)
{
    M5.Imu.update();
    auto imu_data = M5.Imu.getImuData();
    *x = imu_data.accel.x;
    *y = imu_data.accel.y;
    *z = imu_data.accel.z;
}

/// usbserial.cppに入れたい
#include "driver/usb_serial_jtag.h"

#define USB_SERIAL_TX_BUFFER_SIZE 256
#define USB_SERIAL_RX_BUFFER_SIZE 256

// Initialize USB Serial
void usb_serial_init(void)
{
  const usb_serial_jtag_driver_config_t cfg = {
    .tx_buffer_size = USB_SERIAL_TX_BUFFER_SIZE,
    .rx_buffer_size = USB_SERIAL_RX_BUFFER_SIZE
  };
  ESP_ERROR_CHECK(usb_serial_jtag_driver_install((usb_serial_jtag_driver_config_t*)&cfg));
}

// Send a byte via USB Serial
void usb_serial_send_byte(uint8_t b)
{
  usb_serial_jtag_write_bytes(&b, 1, 0);
}

// Send multiple bytes via USB Serial
void usb_serial_send_bytes(uint8_t *b, size_t len)
{
  usb_serial_jtag_write_bytes(b, len, 0);
}

// Receive a byte via USB Serial, return -1 if no data
int16_t usb_serial_receive_byte(void)
{
  uint8_t b;
  size_t r = usb_serial_jtag_read_bytes(&b, 1, 0);
  return r == 1 ? b : -1;
}

// Receive bytes via USB Serial
size_t usb_serial_receive_bytes(uint8_t *b, size_t len)
{
  return usb_serial_jtag_read_bytes(b, len, 0);
}


/// dir.cppに入れたい
#include <dirent.h>

dir_entry *create_dir_entry(const char *name)
{
  dir_entry *entry = (dir_entry *)malloc(sizeof(dir_entry));
  if (entry) {
    entry->name = (char *)malloc(strlen(name) + 1);
    if (entry->name) {
      strcpy(entry->name, name);
    }
    entry->next = NULL;
  }
  return entry;
}

// List .mrb files in the specified directory
dir_entry *list_mrb_files(const char *path)
{
  dir_entry *head = NULL;
  dir_entry *tail = NULL;

  DIR *dir = opendir(path);
  if (dir == NULL) {
    m5printf("Failed to open directory: %s\n", path);
    return NULL;
  }
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    char *dname = entry->d_name;
    if (entry->d_type == DT_REG &&  // 通常ファイル
        !strchr(dname, '~') &&      // ロングネーム削除
        (strstr(dname, ".MRB") || strstr(dname, ".mrb"))) { // .MRBまたは.mrb拡張子
      dir_entry *file = create_dir_entry(entry->d_name);
      if (head == NULL) head = file;
      if (tail) {
        tail->next = file;
        tail = file;
      }
      else {
        tail = file;
      }
      // m5printf("Found mrb file: %s%s (%s)\n", path, entry->d_name, file->name);
    }
  }
  closedir(dir);
  return head;
}

// Free the directory entry list
void free_dir_list(dir_entry *head)
{
  dir_entry *current = head;
  while (current) {
    dir_entry *next = current->next;
    if (current->name) free(current->name);
    free(current);
    current = next;
  }
}

#ifdef __cplusplus
}
#endif

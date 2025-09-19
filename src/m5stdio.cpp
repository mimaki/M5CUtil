#include "M5CUtil.h"
#include "M5Unified.h"
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
  bool result = M5.In_I2C.begin(I2C_MASTER_NUM, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);

  m5printf("I2C initialized: %s\n", result ? "OK" : "Failed");
}

int m5i2c_write(uint8_t addr, const uint8_t *data, size_t len, uint8_t stop)
{
  // esp_err_t ret = i2c_master_write_to_device(I2C_MASTER_NUM, addr, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  // M5.In_I2C.beginTransmission(addr);
  M5.In_I2C.start(addr, false, I2C_MASTER_FREQ_HZ);
  bool result = M5.In_I2C.write(data, len);
  // int ret = M5.In_I2C.endTransmission(stop ? 1 : 0);
  if (stop) {
    M5.In_I2C.stop();
  }
m5printf("m5i2c_write: addr=0x%02x, len=%d, ret=%d\n", addr, len, result ? 0 : -1);
  return result ? 0 : -1;
}

size_t m5i2c_read(uint8_t addr, uint8_t *data, size_t len, uint8_t stop)
{
  // esp_err_t ret = i2c_master_read_from_device(I2C_MASTER_NUM, addr, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
  // M5.In_I2C.requestFrom((int)addr, len);
  M5.In_I2C.start(addr, true, I2C_MASTER_FREQ_HZ);
  // size_t i = 0;
  // for (i=0; i<len; i++) {
  //   data[i] = (uint8_t)M5.In_I2C.read();
  // }
  int result = M5.In_I2C.read(data, len);
  M5.In_I2C.stop();
  // if (stop) M5.In_I2C.endTransmission(1);
  // int ret = result ? ESP_OK : ESP_FAIL;
m5printf("m5i2c_read: addr=0x%02x, len=%d, result=%d\n", addr, len, result);
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



#ifdef __cplusplus
}
#endif

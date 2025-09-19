// #include "M5CUtil.h"
// #include <M5Unified.h>
// #include "driver/i2c.h"

// // #ifdef __cplusplus
// // extern "C" {
// // #endif

// #define I2C_MASTER_NUM        I2C_NUM_0 // I2C ポート番号
// #define I2C_MASTER_SDA_IO     1         // SDA ピン
// #define I2C_MASTER_SCL_IO     2         // SCL ピン
// #define I2C_MASTER_FREQ_HZ    400000    // 400kHz
// #define I2C_MASTER_TIMEOUT_MS 1000

// void m5i2c_init(void)
// {
//   static const i2c_config_t conf = {
//     .mode = I2C_MODE_MASTER,
//     .sda_io_num = I2C_MASTER_SDA_IO,
//     .scl_io_num = I2C_MASTER_SCL_IO,
//     .sda_pullup_en = GPIO_PULLUP_ENABLE,
//     .scl_pullup_en = GPIO_PULLUP_ENABLE,
//     .master = {
//         .clk_speed = I2C_MASTER_FREQ_HZ,
//     },
//     .clk_flags = 0
//   };
//   ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
//   ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));
// }

// int m5i2c_write(uint8_t addr, const uint8_t *data, size_t len, uint8_t stop)
// {
//   esp_err_t ret = i2c_master_write_to_device(I2C_MASTER_NUM, addr, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
//   return ret;
// }

// size_t m5i2c_read(uint8_t addr, uint8_t *data, size_t len, uint8_t stop)
// {
//   esp_err_t ret = i2c_master_read_from_device(I2C_MASTER_NUM, addr, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
//   return (ret == ESP_OK) ? len : 0;
// }

// // #ifdef __cplusplus
// // } // extern "C"
// // #endif

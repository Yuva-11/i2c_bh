
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5
#define BH1750_ADDR 0x23
#define POWER_ON 0x01
#define RESET 0x07
#define CONT_H_RES_MODE 0x10

void bh1750_init() {
    uint8_t buf[1];
    buf[0] = POWER_ON;
    i2c_write_blocking(I2C_PORT, BH1750_ADDR, buf, 1, false);
    sleep_ms(10);
    
    buf[0] = RESET;
    i2c_write_blocking(I2C_PORT, BH1750_ADDR, buf, 1, false);
    sleep_ms(10);
    
    buf[0] = CONT_H_RES_MODE;
    i2c_write_blocking(I2C_PORT, BH1750_ADDR, buf, 1, false);
    sleep_ms(180); // Measurement time
}

uint16_t bh1750_read_light() {
    uint8_t buf[2];
    i2c_read_blocking(I2C_PORT, BH1750_ADDR, buf, 2, false);
    return ((buf[0] << 8) | buf[1]) / 1.2; // Convert to lux
}

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    bh1750_init();

    while (1) {
        uint16_t lux = bh1750_read_light();
        printf("Light intensity: %d lux\n", lux);
        sleep_ms(1000);
    }
}

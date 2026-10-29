/**
 * dev_st7789v2.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdlib.h>

#include "board_common.h"
#include "drv_spi.h"
#include "dev_st7789v2.h"

/* RGB565 Color */
#define LCD_COLOR_WHITE      0xFFFF
#define LCD_COLOR_BLACK      0x0000
#define LCD_COLOR_RED        0xF800
#define LCD_COLOR_GREEN      0x07E0
#define LCD_COLOR_BLUE       0x001F

#define LCD_DC(value)         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (value == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define LCD_RST(value)        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (value == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define LCD_BACKLIGHT(value)  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (value == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define LCD_CS(value)         HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, (value == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET)

static const char *TAG = "dev_st7789v2";

static uint16_t s_lcd_width = 0;
static uint16_t s_lcd_height = 0;

static void priv_lcd_write_cmd(uint8_t cmd)
{
    LCD_DC(0);

    LCD_CS(0);
    drv_spi_write_buf(&cmd, 1);
    LCD_CS(1);
}

static void priv_lcd_write_byte(uint8_t byte)
{
    LCD_DC(1);

    LCD_CS(0);
    drv_spi_write_buf(&byte, 1);
    LCD_CS(1);
}

static void priv_lcd_write_buf(const uint8_t *buf, size_t buf_len)
{
    LCD_DC(1);

    LCD_CS(0);
    drv_spi_write_buf(buf, buf_len);
    LCD_CS(1);
}

static void priv_lcd_set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint8_t buf[4] = {0};

    priv_lcd_write_cmd(0x2A);
    buf[0] = x >> 8;
    buf[1] = x;
    buf[2] = width >> 8;
    buf[3] = width;
    priv_lcd_write_buf(buf, BOARD_ARRAY_SIZE(buf));

    priv_lcd_write_cmd(0x2B);
    buf[0] = y >> 8;
    buf[1] = y;
    buf[2] = height >> 8;
    buf[3] = height;
    priv_lcd_write_buf(buf, BOARD_ARRAY_SIZE(buf));

    priv_lcd_write_cmd(0x2C);
}

static void priv_lcd_clear(uint16_t color)
{
    uint8_t *buf = malloc(s_lcd_width * 2);

    priv_lcd_set_window(0, 0, s_lcd_width - 1, s_lcd_height - 1);

    if (buf != NULL) {
        for (uint16_t x = 0; x < s_lcd_width; x++) {
            buf[2 * x] = color >> 8;
            buf[2 * x + 1] = color;
        }

        for (uint16_t y = 0; y < s_lcd_height; y++) {
            priv_lcd_write_buf(buf, s_lcd_width * 2);
        }

        free(buf);
        buf = NULL;
    } else {
        uint8_t data[2] = {0};
        data[0] = color >> 8;
        data[1] = color;

        for (uint16_t x = 0; x < s_lcd_width; x++) {
            for (uint16_t y = 0; y < s_lcd_height; y++) {
                priv_lcd_write_buf(data, BOARD_ARRAY_SIZE(data));
            }
        }
    }
}

static void priv_io_init(void)
{
    GPIO_InitTypeDef gpio_init;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**
     * PB4 -> LCD_DC
     * PB6 -> LCD_RST
     * PB7 -> LCD_BACKLIGHT
     */
    gpio_init.Pin   = GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOB, &gpio_init);

    LCD_DC(0);
    LCD_RST(0);
    LCD_BACKLIGHT(0);

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /* PD7 -> LCD_CS */
    gpio_init.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOD, &gpio_init);

    /* CS pin: Low enable, High disable */
    LCD_CS(1);
}

int dev_st7789v2_flush(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *buf, size_t buf_len)
{
    if ((x < 0) || (y < 0) || (width <= 0) || (height <= 0) || (buf == NULL)) {
        BOARD_LOGE(TAG, "invalid param");
        return -1;
    }

    if ((x >= s_lcd_width) || (y >= s_lcd_height) || ((x + width) > s_lcd_width) || ((y + height) > s_lcd_height)) {
        BOARD_LOGE(TAG, "out of range");
        return -1;
    }

    priv_lcd_set_window(x, y, x + width - 1, y + height - 1);

    priv_lcd_write_buf(buf, buf_len);

    return 0;
}

int dev_st7789v2_init(uint16_t width, uint16_t height)
{
    if ((width <= 0) || (height <= 0)) {
        BOARD_LOGE(TAG, "invalid param");
        return -1;
    }

    s_lcd_width = width;
    s_lcd_height = height;

    drv_spi_init();
    priv_io_init();

    LCD_RST(0);
    board_delay_ms_os(10); /* Reset pulse duration: T_RW >= 10us */
    LCD_RST(1);
    board_delay_ms_os(120); /* Reset cancel: 5ms <= T_RT <= 120ms */

    /**
     * Sleep Out
     * 1. It will be necessary to wait 5msec before sending any new commands to a display module following this command to allow time for the supply voltages and clock circuits to stabilize
     * 2. It will be necessary to wait 120msec after sending sleep out command (when in sleep in mode) before sending an sleep in command
     */
    priv_lcd_write_cmd(0x11);
    board_delay_ms_os(10);

    /* Memory Data Access Control */
    priv_lcd_write_cmd(0x36);
    priv_lcd_write_byte(0x00);

    /* RGB565 */
    priv_lcd_write_cmd(0x3A);
    priv_lcd_write_byte(0x65);

    /* Porch Setting */
    priv_lcd_write_cmd(0xB2);
    priv_lcd_write_byte(0x0C);
    priv_lcd_write_byte(0x0C);
    priv_lcd_write_byte(0x00);
    priv_lcd_write_byte(0x33);
    priv_lcd_write_byte(0x33);

    /* Gate Control */
    priv_lcd_write_cmd(0xB7);
    priv_lcd_write_byte(0x72);

    /* VCOM Setting */
    priv_lcd_write_cmd(0xBB);
    priv_lcd_write_byte(0x3D);

    /* LCM Control */
    priv_lcd_write_cmd(0xC0);
    priv_lcd_write_byte(0x2C);

    /* VDV and VRH Command Enable */
    priv_lcd_write_cmd(0xC2);
    priv_lcd_write_byte(0x01);

    /* VRH Set */
    priv_lcd_write_cmd(0xC3);
    priv_lcd_write_byte(0x19);

    /* VDV Set */
    priv_lcd_write_cmd(0xC4);
    priv_lcd_write_byte(0x20);

    /* Frame Rate Control in Normal Mode */
    priv_lcd_write_cmd(0xC6);
    priv_lcd_write_byte(0x0F);

    /* Power Control 1 */
    priv_lcd_write_cmd(0xD0);
    priv_lcd_write_byte(0xA4);
    priv_lcd_write_byte(0xA1);

    /* Positive Voltage Gamma Control */
    priv_lcd_write_cmd(0xE0);
    priv_lcd_write_byte(0xD0);
    priv_lcd_write_byte(0x04);
    priv_lcd_write_byte(0x0D);
    priv_lcd_write_byte(0x11);
    priv_lcd_write_byte(0x13);
    priv_lcd_write_byte(0x2B);
    priv_lcd_write_byte(0x3F);
    priv_lcd_write_byte(0x54);
    priv_lcd_write_byte(0x4C);
    priv_lcd_write_byte(0x18);
    priv_lcd_write_byte(0x0D);
    priv_lcd_write_byte(0x0B);
    priv_lcd_write_byte(0x1F);
    priv_lcd_write_byte(0x23);

    /* Negative Voltage Gamma Control */
    priv_lcd_write_cmd(0xE1);
    priv_lcd_write_byte(0xD0);
    priv_lcd_write_byte(0x04);
    priv_lcd_write_byte(0x0C);
    priv_lcd_write_byte(0x11);
    priv_lcd_write_byte(0x13);
    priv_lcd_write_byte(0x2C);
    priv_lcd_write_byte(0x3F);
    priv_lcd_write_byte(0x44);
    priv_lcd_write_byte(0x51);
    priv_lcd_write_byte(0x2F);
    priv_lcd_write_byte(0x1F);
    priv_lcd_write_byte(0x1F);
    priv_lcd_write_byte(0x20);
    priv_lcd_write_byte(0x23);

    /* Display Inversion On */
    priv_lcd_write_cmd(0x21);

    /* Display On */
    priv_lcd_write_cmd(0x29);

    priv_lcd_clear(LCD_COLOR_BLACK);

    LCD_BACKLIGHT(1);

    return 0;
}

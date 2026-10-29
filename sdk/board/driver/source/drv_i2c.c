/**
 * drv_i2c.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdbool.h>

#include "board_common.h"
#include "drv_i2c.h"

static const char *TAG = "drv_i2c";

static I2C_HandleTypeDef s_i2c3_handler = {0};

static bool s_i2c3_init = false;

void I2C3_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&s_i2c3_handler);
}

void I2C3_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&s_i2c3_handler);
}

/**
 * 调用关系:
 * HAL_I2C_Init
 *   -> HAL_I2C_MspInit
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef status = HAL_OK;

    GPIO_InitTypeDef gpio_init = {0};
    RCC_PeriphCLKInitTypeDef periph_clk_init = {0};

    if (hi2c->Instance == I2C3) {
        periph_clk_init.PeriphClockSelection = RCC_PERIPHCLK_I2C3;
        periph_clk_init.I2c3ClockSelection   = RCC_I2C3CLKSOURCE_PCLK1;

        status = HAL_RCCEx_PeriphCLKConfig(&periph_clk_init);
        if (status != HAL_OK) {
            BOARD_LOGE(TAG, "HAL_RCCEx_PeriphCLKConfig failed: %d", status);
        }

        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**
         * PC0 -> I2C3_SCL
         * PC1 -> I2C3_SDA
         *
         * 两个引脚均接有 4.7K 外部上拉电阻 (100 KHz)
         */
        gpio_init.Pin       = GPIO_PIN_0 | GPIO_PIN_1;
        gpio_init.Mode      = GPIO_MODE_AF_OD;
        gpio_init.Pull      = GPIO_NOPULL;
        gpio_init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init.Alternate = GPIO_AF4_I2C3;

        HAL_GPIO_Init(GPIOC, &gpio_init);

        __HAL_RCC_I2C3_CLK_ENABLE();

        HAL_NVIC_SetPriority(I2C3_EV_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1, 0);
        HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);

        HAL_NVIC_SetPriority(I2C3_ER_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1, 0);
        HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
    }
}

/**
 * 调用关系:
 * HAL_I2C_DeInit
 *   -> HAL_I2C_MspDeInit
 */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C3) {
        __HAL_RCC_I2C3_CLK_DISABLE();

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1);

        HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
        HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
    }
}

int drv_i2c_init(void)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (s_i2c3_init) {
        return 0;
    }

    /**
     * Timing = 0x10909CEC
     * 1. I2C Clock Source = PCLK1 = 80 MHz
     * 2. I2C Speed = 100 KHz
     */
    s_i2c3_handler.Instance              = I2C3;
    s_i2c3_handler.Init.Timing           = 0x10909CEC;
    s_i2c3_handler.Init.OwnAddress1      = 0;
    s_i2c3_handler.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    s_i2c3_handler.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    s_i2c3_handler.Init.OwnAddress2      = 0;
    s_i2c3_handler.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    s_i2c3_handler.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    s_i2c3_handler.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    status = HAL_I2C_Init(&s_i2c3_handler);
    if (status != HAL_OK) {
        BOARD_LOGE(TAG, "HAL_I2C_Init failed: %d", status);
        return -1;
    }

    status = HAL_I2CEx_ConfigAnalogFilter(&s_i2c3_handler, I2C_ANALOGFILTER_ENABLE);
    if (status != HAL_OK) {
        BOARD_LOGE(TAG, "HAL_I2CEx_ConfigAnalogFilter failed: %d", status);
    }

    status = HAL_I2CEx_ConfigDigitalFilter(&s_i2c3_handler, 0);
    if (status != HAL_OK) {
        BOARD_LOGE(TAG, "HAL_I2CEx_ConfigDigitalFilter failed: %d", status);
    }

    s_i2c3_init = true;

    return 0;
}

int drv_i2c_reg_write_byte(uint16_t dev_addr, uint16_t reg_addr, const uint8_t *byte)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (!s_i2c3_init) {
        BOARD_LOGE(TAG, "i2c not init");
        return -1;
    }

    status = HAL_I2C_Mem_Write(&s_i2c3_handler, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)byte, 1, 100);
    if (status != HAL_OK) {
        BOARD_LOGE(TAG, "HAL_I2C_Mem_Write_IT failed: %d", status);
        return -1;
    }

    return 0;
}

int drv_i2c_reg_read_byte(uint16_t dev_addr, uint16_t reg_addr, uint8_t *byte)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (!s_i2c3_init) {
        BOARD_LOGE(TAG, "i2c not init");
        return -1;
    }

    status = HAL_I2C_Mem_Read(&s_i2c3_handler, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, byte, 1, 100);
    if (status != HAL_OK) {
        BOARD_LOGE(TAG, "HAL_I2C_Mem_Read_IT failed: %d", status);
        return -1;
    }

    return 0;
}

int drv_i2c_reg_write_buf(uint16_t dev_addr, uint16_t reg_addr, const uint8_t *buf, int len)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (!s_i2c3_init) {
        BOARD_LOGE(TAG, "i2c not init");
        return -1;
    }

    status = HAL_I2C_Mem_Write(&s_i2c3_handler, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buf, len, 100);
    if (status != HAL_OK) {
        BOARD_LOGE(TAG, "HAL_I2C_Mem_Write_IT failed: %d", status);
        return -1;
    }

    return 0;
}

int drv_i2c_reg_read_buf(uint16_t dev_addr, uint16_t reg_addr, uint8_t *buf, int len)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (!s_i2c3_init) {
        BOARD_LOGE(TAG, "i2c not init");
        return -1;
    }

    status = HAL_I2C_Mem_Read(&s_i2c3_handler, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, buf, len, 100);
    if (status != HAL_OK) {
        BOARD_LOGE(TAG, "HAL_I2C_Mem_Read_IT failed: %d", status);
        return -1;
    }

    return 0;
}

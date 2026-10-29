/**
 * drv_spi.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdbool.h>

#include "board_common.h"
#include "drv_spi.h"

static const char *TAG = "drv_spi";

static SPI_HandleTypeDef s_spi3_handler = {0};

static bool s_spi3_init = false;

void SPI3_IRQHandler(void)
{
    HAL_SPI_IRQHandler(&s_spi3_handler);
}

/**
 * 调用关系:
 * HAL_SPI_Init
 *   -> HAL_SPI_MspInit
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    HAL_StatusTypeDef status = HAL_OK;

    GPIO_InitTypeDef gpio_init = {0};

    if (hspi->Instance == SPI3) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**
         * PB3 -> SPI3_SCK
         * PB5 -> SPI3_MOSI
         */
        gpio_init.Pin       = GPIO_PIN_3 | GPIO_PIN_5;
        gpio_init.Mode      = GPIO_MODE_AF_PP;
        gpio_init.Pull      = GPIO_PULLUP;
        gpio_init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init.Alternate = GPIO_AF6_SPI3;

        HAL_GPIO_Init(GPIOB, &gpio_init);

        __HAL_RCC_SPI3_CLK_ENABLE();

        // TODO 添加 DMA 和中断的支持

        HAL_NVIC_SetPriority(SPI3_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1, 0);
        HAL_NVIC_EnableIRQ(SPI3_IRQn);
    }
}

/**
 * 调用关系:
 * HAL_SPI_DeInit
 *   -> HAL_SPI_MspDeInit
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
    if (hspi->Instance == SPI3) {
        __HAL_RCC_SPI3_CLK_DISABLE();

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_5);

        HAL_NVIC_DisableIRQ(SPI3_IRQn);
    }
}

int drv_spi_init(void)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (s_spi3_init) {
        return 0;
    }

    s_spi3_handler.Instance               = SPI3;
    s_spi3_handler.Init.Mode              = SPI_MODE_MASTER;
    /**
     * 1-line (half-duplex): MISO / MOSI 使用同一个引脚
     * 2-line (full-duplex): MISO / MOSI 使用不同的引脚
     */
    s_spi3_handler.Init.Direction         = SPI_DIRECTION_2LINES;
    /* 数据长度: 8 bit */
    s_spi3_handler.Init.DataSize          = SPI_DATASIZE_8BIT;
    /**
     * CPOL = 0: SCK 空闲时处于低电平
     * CPOL = 1: SCK 空闲时处于高电平
     */
    s_spi3_handler.Init.CLKPolarity       = SPI_POLARITY_LOW;
    /**
     * CPHA = 0: 在 SCK 第一个边沿采样
     * CPHA = 1: 在 SCK 第二个边沿采样
     */
    s_spi3_handler.Init.CLKPhase          = SPI_PHASE_1EDGE;
    /* 片选引脚由软件控制 (手动拉低 / 拉高 GPIO) */
    s_spi3_handler.Init.NSS               = SPI_NSS_SOFT;
    /**
     * SPI3 挂在 ABP1 上, 时钟源是 PCLK1
     * SCK CLK = PCLK1 / BaudRatePrescaler (80MHz / 2 = 40MHz)
     */
    s_spi3_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    /* MSB */
    s_spi3_handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    /**
     * 使能: 帧格式是 TI
     * 禁用: 帧格式是 Motorola
     */
    s_spi3_handler.Init.TIMode            = SPI_TIMODE_DISABLE;
    /* 禁用硬件 CRC 校验功能 */
    s_spi3_handler.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    /* CRC 多项式设为 7 (x⁸ + x² + x + 1) */
    s_spi3_handler.Init.CRCPolynomial     = 7;
    /* CRC 长度与数据帧长度一致 */
    s_spi3_handler.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    /**
     * 使能: 在数据传输结束后产生一个 NSS 高脉冲
     * 禁用: 在数据传输结束后不产生一个 NSS 高脉冲
     */
    s_spi3_handler.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;

    status = HAL_SPI_Init(&s_spi3_handler);
    if (status != HAL_OK) {
        BOARD_LOGE(TAG, "HAL_SPI_Init failed: %d", status);
        return -1;
    }

    s_spi3_init = true;

    return 0;
}

int drv_spi_write_buf(const uint8_t *buf, size_t buf_len)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (!s_spi3_init) {
        BOARD_LOGE(TAG, "spi not init");
        return -1;
    }

    if ((buf == NULL) || (buf_len == 0)) {
        BOARD_LOGE(TAG, "invalid param");
        return -1;
    }

    status = HAL_SPI_Transmit(&s_spi3_handler, buf, buf_len, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        BOARD_LOGE(TAG, "HAL_SPI_Transmit failed: %d", status);
        return -1;
    }

    return 0;
}

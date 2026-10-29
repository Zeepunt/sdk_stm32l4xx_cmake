/**
 * dev_st7789v2.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __DEV_ST7789V2_H__
#define __DEV_ST7789V2_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Flush a rectangular area to the ST7789V2 controller.
 * @param x Left X coordinate
 * @param y Top Y coordinate
 * @param width Area width in pixels
 * @param height Area height in pixels
 * @param buf Pixel data buffer
 * @param buf_len Pixel data buffer length
 * @return
 *   0: success
 *   -1: failed
 */
int dev_st7789v2_flush(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *buf, size_t buf_len);

/**
 * @brief Initialize the ST7789V2 Controller
 * @param res_x LCD resolution X, uint: pixel
 * @param res_y LCD resolution Y, uint: pixel
 * @return
 *   0: success
 *   -1: failed
 */
int dev_st7789v2_init(uint16_t res_x, uint16_t res_y);

#ifdef __cplusplus
}
#endif

#endif /* __DEV_ST7789V2_H__ */

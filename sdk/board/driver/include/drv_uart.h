/**
 * drv_uart.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Write data to UART
 * @param buf: data
 * @param len: data length
 * @return
 *   0: success
 *   -1: fail
 * @note blocking mode
 */
int drv_uart_write(const char *buf, int len);

/**
 * @brief Initialize the UART
 * @return
 *   0: success
 *   -1: fail
 */
int drv_uart_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_UART_H__ */

/**
 * drv_spi.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief send spi data
 * @param buf data buffer
 * @param buf_len data buffer length
 * @return
 *   0: success
 *   -1: failed
 * @note blocking mode
 */
int drv_spi_write_buf(const uint8_t *buf, size_t buf_len);

/**
 * @brief Initialize SPI bus
 * @return
 *   0: success
 *   -1: failed
 */
int drv_spi_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_SPI_H__ */

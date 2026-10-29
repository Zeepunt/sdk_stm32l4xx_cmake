/**
 * drv_i2c.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __DRV_I2C_H__
#define __DRV_I2C_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief write i2c register (1 byte)
 * @param dev_addr i2c device address
 * @param reg_addr i2c register address
 * @param byte i2c register value
 * @return
 *   0: success
 *   -1: failed
 * @note blocking mode
 */
int drv_i2c_reg_write_byte(uint16_t dev_addr, uint16_t reg_addr, const uint8_t *byte);

/**
 * @brief read i2c register (1 byte)
 * @param dev_addr i2c device address
 * @param reg_addr i2c register address
 * @param byte i2c register value
 * @return
 *   0: success
 *   -1: failed
 * @note blocking mode
 */
int drv_i2c_reg_read_byte(uint16_t dev_addr, uint16_t reg_addr, uint8_t *byte);

/**
 * @brief write i2c register (buf)
 * @param dev_addr i2c device address
 * @param reg_addr i2c register address
 * @param buf i2c register value
 * @param len i2c register value length
 * @return
 *   0: success
 *   -1: failed
 * @note blocking mode
 */
int drv_i2c_reg_write_buf(uint16_t dev_addr, uint16_t reg_addr, const uint8_t *buf, int len);

/**
 * @brief read i2c register (buf)
 * @param dev_addr i2c device address
 * @param reg_addr i2c register address
 * @param buf i2c register value
 * @param len i2c register value length
 * @return
 *   0: success
 *   -1: failed
 * @note blocking mode
 */
int drv_i2c_reg_read_buf(uint16_t dev_addr, uint16_t reg_addr, uint8_t *buf, int len);

/**
 * @brief Initialize I2C bus
 * @return
 *   0: success
 *   -1: failed
 */
int drv_i2c_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_I2C_H__ */

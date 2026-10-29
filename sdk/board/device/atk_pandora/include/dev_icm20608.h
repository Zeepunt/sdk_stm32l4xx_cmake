/**
 * dev_icm20608.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __DEV_ICM20608_H__
#define __DEV_ICM20608_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the gyroscope data
 * @param x x axis data
 * @param y y axis data
 * @param z z axis data
 * @return
 *   0: success
 *   -1: failed
 */
int dev_icm20608_gyroscope_get(int16_t *x, int16_t *y, int16_t *z);

/**
 * @brief Get the accelerometer data
 * @param x x axis data
 * @param y y axis data
 * @param z z axis data
 * @return
 *   0: success
 *   -1: failed
 */
int dev_icm20608_accelerometer_get(int16_t *x, int16_t *y, int16_t *z);

/**
 * @brief Initialize the ICM-20608 Sensor
 * @return
 *   0: success
 *   -1: failed
 */
int dev_icm20608_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __DEV_ICM20608_H__ */

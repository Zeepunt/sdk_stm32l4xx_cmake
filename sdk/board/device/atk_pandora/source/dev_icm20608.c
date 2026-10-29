/**
 * dev_icm20608.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include "board_common.h"
#include "drv_i2c.h"
#include "dev_icm20608.h"

/**
 * AD0 接 GND  : 7-bit address = 0x68
 * AD0 接 3.3V : 7-bit address = 0x69
 */
#define ICM20608_ADDR    0x68

typedef enum {
    REG_ACCEL_OFFS       = 0x06,

    REG_GYRO_CONFIG      = 0x1B,
    REG_ACCEL_CONFIG     = 0x1C,
    REG_ACCEL_CONFIG2    = 0x1D,

    REG_SMPLRT_DIV       = 0x19,
    REG_CONFIG           = 0X1A,

    REG_ACCEL_XOUT_H     = 0x3B,
    REG_ACCEL_XOUT_L     = 0x3C,
    REG_ACCEL_YOUT_H     = 0x3D,
    REG_ACCEL_YOUT_L     = 0x3E,
    REG_ACCEL_ZOUT_H     = 0x3F,
    REG_ACCEL_ZOUT_L     = 0x40,
    RET_TEMP_OUT_H       = 0x41,
    RET_TEMP_OUT_L       = 0x42,
    REG_GYRO_XOUT_H      = 0x43,
    REG_GYRO_XOUT_L      = 0x44,
    REG_GYRO_YOUT_H      = 0x45,
    REG_GYRO_YOUT_L      = 0x46,
    REG_GYRO_ZOUT_H      = 0x47,
    REG_GYRO_ZOUT_L      = 0x48,

    REG_PWR_MGMT_1       = 0x6B,
    REG_PWR_MGMT_2       = 0x6C,

    REG_WHO_AM_I         = 0x75,
} icm20608g_reg_t;

static const char *TAG = "dev_icm20608";

static void priv_reg_write_byte(uint8_t reg, uint8_t byte)
{
    (void)drv_i2c_reg_write_byte(ICM20608_ADDR << 1, reg, (const uint8_t *)&byte);
}

static void priv_reg_read_byte(uint8_t reg, uint8_t *byte)
{
    (void)drv_i2c_reg_read_byte(ICM20608_ADDR << 1, reg, byte);
}

static void priv_reg_read_buf(uint8_t reg, uint8_t *buf, int len)
{
    (void)drv_i2c_reg_read_buf(ICM20608_ADDR << 1, reg, buf, len);
}

static void priv_gyro_fsr_set(uint8_t fsr)
{
    /**
     * gyro = Gyroscope
     * fsr  = Full Scale Range
     */

    if (fsr > 3) {
        fsr = 3;
    }

    /**
     * bit[4:3] - FS_SEL[1:0]
     *   Gyro Full Scale Select
     *   00 = ±250dps
     *   01 = ±500dps
     *   10 = ±1000dps
     *   11 = ±2000dps
     */
    priv_reg_write_byte(REG_GYRO_CONFIG, fsr << 3);
}

static void priv_accel_fsr_set(uint8_t fsr)
{
    /**
     * accel = Accelerometer
     * fsr   = Full Scale Range
     */

    if (fsr > 3) {
        fsr = 3;
    }

    /**
     * bit[4:3] - ACCEL_FS_SEL[1:0]
     *   Accel Full Scale Select
     *   00 = ±2g
     *   01 = ±4g
     *   10 = ±8g
     *   11 = ±16g
     */
    priv_reg_write_byte(REG_ACCEL_CONFIG, fsr << 3);
}

static void priv_lpf_set(uint16_t hz)
{
    /* lpf = Low Pass Filter */

    uint8_t dlpf_cfg = 0;

    /**
     * DLPF_CFG  Gyroscope 3-dB BW   Temperature Sensor 3-dB BW
     *    1           176 Hz                  188 Hz
     *    2            92 Hz                   98 Hz
     *    3            41 Hz                   42 Hz
     *    4            20 Hz                   20 Hz
     *    5            10 Hz                   10 Hz
     *    6             5 Hz                    5 Hz
     *    7          3281 Hz                 4000 Hz
     */
    if (hz >= 176) {
        dlpf_cfg = 1;
    } else if (hz >= 92) {
        dlpf_cfg = 2;
    } else if (hz >= 41) {
        dlpf_cfg = 3;
    } else if (hz >= 20) {
        dlpf_cfg = 4;
    } else if (hz >= 10) {
        dlpf_cfg = 5;
    } else {
        dlpf_cfg = 6;
    }

    /**
     * bit[6] - FIFO_MODE
     *   1: when the FIFO is full, additional writes will not be written to FIFO
     *   0: when the FIFO is full, additional writes will be written to the FIFO, replacing the oldest data
     *
     * bit[5:3] - EXT_SYNC_SET[2:0]
     *   Enables the FSYNC pin data to be sampled
     *
     * bit[2:0] - DLPF_CFG[2:0]
     *   For the DLPF to be used, FCHOICE_B[1:0] is 2’b00
     */
    priv_reg_write_byte(REG_CONFIG, dlpf_cfg);
}

static void priv_accel_lpf_set(uint16_t hz)
{
    uint8_t a_dlpf_cfg = 0;

    /**
     * A_DLPF_CFG  Accelerometer 3-dB BW
     *    1             218.1 Hz
     *    2              99.0 Hz
     *    3              44.8 Hz
     *    4              21.2 Hz
     *    5              10.2 Hz
     *    6               5.1 Hz
     *    7             420.0 Hz
     */
    if (hz >= 218) {
        a_dlpf_cfg = 1;
    } else if (hz >= 99) {
        a_dlpf_cfg = 2;
    } else if (hz >= 45) {
        a_dlpf_cfg = 3;
    } else if (hz >= 21) {
        a_dlpf_cfg = 4;
    } else if (hz >= 10) {
        a_dlpf_cfg = 5;
    } else {
        a_dlpf_cfg = 6;
    }

    /**
     * bit[2:0] - A_DLPF_CFG
     *   Accelerometer low pass filter setting
     */
    priv_reg_write_byte(REG_ACCEL_CONFIG2, a_dlpf_cfg);
}

static void priv_sample_rate_set(uint16_t hz)
{
    if (hz > 1000) {
        hz = 1000;
    }

    if (hz < 4) {
        hz = 4;
    }

    priv_lpf_set(hz / 2);
    priv_accel_lpf_set(hz / 2);

    uint8_t div = (1000 / hz) - 1;
    /**
     * bit[7:0] - SMPLRT_DIV[7:0]
     *   Divides the internal sample rate to generate the sample rate that controls sensor data output rate, FIFO sample rate. 
     *   SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV), INTERNAL_SAMPLE_RATE = 1kHz
     *
     * NOTE: This register is only effective when FCHOICE_B register bits are 2’b00, and (0 < DLPF_CFG < 7)
     */
    priv_reg_write_byte(REG_SMPLRT_DIV, div);
}

int dev_icm20608_gyroscope_get(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t buf[6] = {0};

    priv_reg_read_buf(REG_GYRO_XOUT_H, buf, 6);

    *x = (int16_t)(buf[0] << 8 | buf[1]);
    *y = (int16_t)(buf[2] << 8 | buf[3]);
    *z = (int16_t)(buf[4] << 8 | buf[5]);

    return 0;
}

int dev_icm20608_accelerometer_get(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t buf[6] = {0};

    priv_reg_read_buf(REG_ACCEL_XOUT_H, buf, 6);

    *x = (int16_t)(buf[0] << 8 | buf[1]);
    *y = (int16_t)(buf[2] << 8 | buf[3]);
    *z = (int16_t)(buf[4] << 8 | buf[5]);

    return 0;
}

int dev_icm20608_init(void)
{
    int ret = 0;

    uint8_t version = 0;

    ret = drv_i2c_init();
    if (ret != 0) {
        return -1;
    }

    /**
     * bit[7] - DEVICE_RESET
     *   1: Reset the internal registers and restores the default settings.
     *   The bit automatically clears to 0 once the reset is done.
     */
    priv_reg_write_byte(REG_PWR_MGMT_1, 0x80);
    board_delay_ms_os(100);

    /**
     * bit[6] - SLEEP
     *   1: the chip is set to sleep mode
     *   The default value is 1, the chip comes up in Sleep mode
     *
     * bit[2:0] - CLKSEL[2:0]
     *   0: Internal 20MHz oscillator
     *   1: Auto selects the best available clock source – PLL if ready, else use the Internal oscillator
     */
    priv_reg_write_byte(REG_PWR_MGMT_1, 0x01);
    board_delay_ms_os(35);

    /**
     * bit[5] - STBY_XA
     *   0: X accelerometer is on
     *   1: X accelerometer is disabled
     *
     * bit[4] - STBY_YA
     *   0: Y accelerometer is on
     *   1: Y accelerometer is disabled
     *
     * bit[3] - STBY_ZA
     *   0: Z accelerometer is on
     *   1: Z accelerometer is disabled
     *
     * bit[2] - STBY_XG
     *   0: X gyro is on
     *   1: X gyro is disabled
     *
     * bit[1] - STBY_YG
     *   0: Y gyro is on
     *   1: Y gyro is disabled
     *
     * bit[0] - STBY_ZG
     *   0: Z gyro is on
     *   1: Z gyro is disabled
     */
    priv_reg_write_byte(REG_PWR_MGMT_2, 0x00);

    /**
     * 0xAE - ICM-20608-D
     * 0xAF - ICM-20608-G
     */
    priv_reg_read_byte(REG_WHO_AM_I, &version);
    BOARD_LOGD(TAG, "version: 0x%02X", version);
    if ((version != 0xAE) && (version != 0xAF)) {
        BOARD_LOGE(TAG, "unknown device ID");
        return -1;
    }

    priv_gyro_fsr_set(3);
    priv_accel_fsr_set(0);
    priv_sample_rate_set(26);

    return 0;
}

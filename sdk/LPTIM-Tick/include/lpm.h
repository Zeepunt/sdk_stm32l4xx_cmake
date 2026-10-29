/**
 * lpm.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */

#ifndef __LPM_H__
#define __LPM_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void lpm_pre_sleep_processing(uint32_t time);

void lpm_post_sleep_processing(uint32_t time);

void lpm_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __LPM_H__ */

/**
 * lpm.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include "stm32l4xx.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_pwr.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lpm.h"

static volatile uint32_t s_rcc_cr = 0;
static volatile uint32_t s_rcc_cfgr = 0;
static volatile uint32_t s_rcc_pllcfgr = 0;

void lpm_init(void)
{
#if (configUSE_TICKLESS_IDLE == 2)
    /* 设置从 STOP 模式唤醒后的系统时钟 */
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
#endif
}

void lpm_pre_sleep_processing(uint32_t time)
{
    HAL_SuspendTick();

    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP2);

    s_rcc_cr = READ_REG(RCC->CR);
    s_rcc_cfgr = READ_REG(RCC->CFGR);
    s_rcc_pllcfgr = READ_REG(RCC->PLLCFGR);

    LL_LPM_EnableDeepSleep();
}

void lpm_post_sleep_processing(uint32_t time)
{
    LL_LPM_EnableSleep();

    WRITE_REG(RCC->PLLCFGR, s_rcc_pllcfgr);
    WRITE_REG(RCC->CR, s_rcc_cr);
    WRITE_REG(RCC->CFGR, s_rcc_cfgr);

    HAL_ResumeTick();
}

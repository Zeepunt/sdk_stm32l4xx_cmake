/**
 * startup_stm32l475xx.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include <stdint.h>

#include "stm32l475xx.h"

typedef void (*VECTOR_TABLE_Type)(void);

/* 定义在 cmsis_armclang_m.h 或 cmsis_gcc_m.h 头文件中 */
extern uint32_t __INITIAL_SP;
extern __NO_RETURN void __PROGRAM_START(void);

__NO_RETURN void Reset_Handler(void);
void Default_Handler(void);

/* 弱声明, 默认中断处理函数都是 Default_Handler */
void NMI_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));

void WWDG_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void PVD_PVM_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void TAMP_STAMP_IRQHandler              (void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_WKUP_IRQHandler                (void) __attribute__ ((weak, alias("Default_Handler")));
void FLASH_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void RCC_IRQHandler                     (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI0_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI1_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI2_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI3_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI4_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_TX_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_RX0_IRQHandler                (void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler                (void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler                (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_BRK_TIM15_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_UP_TIM16_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM17_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM2_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM3_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM4_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI1_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI2_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void USART1_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void USART2_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void USART3_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler               (void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler               (void) __attribute__ ((weak, alias("Default_Handler")));
void DFSDM1_FLT3_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_BRK_IRQHandler                (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_UP_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_TRG_COM_IRQHandler            (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void ADC3_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void FMC_IRQHandler                     (void) __attribute__ ((weak, alias("Default_Handler")));
void SDMMC1_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM5_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI3_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void UART4_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void UART5_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler                (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM7_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel1_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel2_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel3_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel4_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel5_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DFSDM1_FLT0_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler")));
void DFSDM1_FLT1_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler")));
void DFSDM1_FLT2_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler")));
void COMP_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void LPTIM1_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void LPTIM2_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_FS_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel6_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel7_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler")));
void LPUART1_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void QUADSPI_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void SAI1_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void SAI2_IRQHandler                    (void) __attribute__ ((weak, alias("Default_Handler")));
void SWPMI1_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void TSC_IRQHandler                     (void) __attribute__ ((weak, alias("Default_Handler")));
void RNG_IRQHandler                     (void) __attribute__ ((weak, alias("Default_Handler")));
void FPU_IRQHandler                     (void) __attribute__ ((weak, alias("Default_Handler")));

/* 中断向量表 */
const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
    (VECTOR_TABLE_Type)(&__INITIAL_SP),       /* Initial Stack Pointer */
    Reset_Handler,                            /* Reset Handler */
    NMI_Handler,                              /* NMI Handler           (-14) */
    HardFault_Handler,                        /* Hard Fault Handler    (-13) */
    MemManage_Handler,                        /* MPU Fault Handler     (-12) */
    BusFault_Handler,                         /* Bus Fault Handler     (-11) */
    UsageFault_Handler,                       /* Usage Fault Handler   (-10) */
    0,                                        /* Reserved */
    0,                                        /* Reserved */
    0,                                        /* Reserved */
    0,                                        /* Reserved */
    SVC_Handler,                              /* SVC Handler           (-5) */
    DebugMon_Handler,                         /* Debug Monitor Handler (-4) */
    0,                                        /* Reserved */
    PendSV_Handler,                           /* PendSV Handler        (-2) */
    SysTick_Handler,                          /* SysTick Handler       (-1) */

    /* 外部中断 */
    WWDG_IRQHandler,                          /* Window WatchDog */
    PVD_PVM_IRQHandler,                       /* PVD/PVM1/PVM2/PVM3/PVM4 through EXTI Line detection */
    TAMP_STAMP_IRQHandler,                    /* Tamper and TimeStamps through the EXTI line */
    RTC_WKUP_IRQHandler,                      /* RTC Wakeup through the EXTI line */
    FLASH_IRQHandler,                         /* FLASH */
    RCC_IRQHandler,                           /* RCC */
    EXTI0_IRQHandler,                         /* EXTI Line0 */
    EXTI1_IRQHandler,                         /* EXTI Line1 */
    EXTI2_IRQHandler,                         /* EXTI Line2 */
    EXTI3_IRQHandler,                         /* EXTI Line3 */
    EXTI4_IRQHandler,                         /* EXTI Line4 */
    DMA1_Channel1_IRQHandler,                 /* DMA1 Channel 1 */
    DMA1_Channel2_IRQHandler,                 /* DMA1 Channel 2 */
    DMA1_Channel3_IRQHandler,                 /* DMA1 Channel 3 */
    DMA1_Channel4_IRQHandler,                 /* DMA1 Channel 4 */
    DMA1_Channel5_IRQHandler,                 /* DMA1 Channel 5 */
    DMA1_Channel6_IRQHandler,                 /* DMA1 Channel 6 */
    DMA1_Channel7_IRQHandler,                 /* DMA1 Channel 7 */
    ADC1_2_IRQHandler,                        /* ADC1, ADC2 */
    CAN1_TX_IRQHandler,                       /* CAN1 TX */
    CAN1_RX0_IRQHandler,                      /* CAN1 RX0 */
    CAN1_RX1_IRQHandler,                      /* CAN1 RX1 */
    CAN1_SCE_IRQHandler,                      /* CAN1 SCE */
    EXTI9_5_IRQHandler,                       /* External Line[9:5]s */
    TIM1_BRK_TIM15_IRQHandler,                /* TIM1 Break and TIM15 */
    TIM1_UP_TIM16_IRQHandler,                 /* TIM1 Update and TIM16 */
    TIM1_TRG_COM_TIM17_IRQHandler,            /* TIM1 Trigger and Commutation and TIM17 */
    TIM1_CC_IRQHandler,                       /* TIM1 Capture Compare */
    TIM2_IRQHandler,                          /* TIM2 */
    TIM3_IRQHandler,                          /* TIM3 */
    TIM4_IRQHandler,                          /* TIM4 */
    I2C1_EV_IRQHandler,                       /* I2C1 Event */
    I2C1_ER_IRQHandler,                       /* I2C1 Error */
    I2C2_EV_IRQHandler,                       /* I2C2 Event */
    I2C2_ER_IRQHandler,                       /* I2C2 Error */
    SPI1_IRQHandler,                          /* SPI1 */
    SPI2_IRQHandler,                          /* SPI2 */
    USART1_IRQHandler,                        /* USART1 */
    USART2_IRQHandler,                        /* USART2 */
    USART3_IRQHandler,                        /* USART3 */
    EXTI15_10_IRQHandler,                     /* External Line[15:10] */
    RTC_Alarm_IRQHandler,                     /* RTC Alarm (A and B) through EXTI Line */
    DFSDM1_FLT3_IRQHandler,                   /* DFSDM1 Filter 3 global Interrupt */
    TIM8_BRK_IRQHandler,                      /* TIM8 Break Interrupt */
    TIM8_UP_IRQHandler,                       /* TIM8 Update Interrupt */
    TIM8_TRG_COM_IRQHandler,                  /* TIM8 Trigger and Commutation Interrupt */
    TIM8_CC_IRQHandler,                       /* TIM8 Capture Compare Interrupt */
    ADC3_IRQHandler,                          /* ADC3 global Interrupt */
    FMC_IRQHandler,                           /* FMC */
    SDMMC1_IRQHandler,                        /* SDMMC1 */
    TIM5_IRQHandler,                          /* TIM5 */
    SPI3_IRQHandler,                          /* SPI3 */
    UART4_IRQHandler,                         /* UART4 */
    UART5_IRQHandler,                         /* UART5 */
    TIM6_DAC_IRQHandler,                      /* TIM6 and DAC1&2 underrun errors */
    TIM7_IRQHandler,                          /* TIM7 */
    DMA2_Channel1_IRQHandler,                 /* DMA2 Channel 1 */
    DMA2_Channel2_IRQHandler,                 /* DMA2 Channel 2 */
    DMA2_Channel3_IRQHandler,                 /* DMA2 Channel 3 */
    DMA2_Channel4_IRQHandler,                 /* DMA2 Channel 4 */
    DMA2_Channel5_IRQHandler,                 /* DMA2 Channel 5 */
    DFSDM1_FLT0_IRQHandler,                   /* DFSDM1 Filter 0 global Interrupt */
    DFSDM1_FLT1_IRQHandler,                   /* DFSDM1 Filter 1 global Interrupt */
    DFSDM1_FLT2_IRQHandler,                   /* DFSDM1 Filter 2 global Interrupt */
    COMP_IRQHandler,                          /* COMP Interrupt */
    LPTIM1_IRQHandler,                        /* LP TIM1 interrupt */
    LPTIM2_IRQHandler,                        /* LP TIM2 interrupt */
    OTG_FS_IRQHandler,                        /* USB OTG FS */
    DMA2_Channel6_IRQHandler,                 /* DMA2 Channel 6 */
    DMA2_Channel7_IRQHandler,                 /* DMA2 Channel 7 */
    LPUART1_IRQHandler,                       /* LP UART1 interrupt */
    QUADSPI_IRQHandler,                       /* Quad SPI global interrupt */
    I2C3_EV_IRQHandler,                       /* I2C3 event */
    I2C3_ER_IRQHandler,                       /* I2C3 error */
    SAI1_IRQHandler,                          /* Serial Audio Interface 1 global interrupt */
    SAI2_IRQHandler,                          /* Serial Audio Interface 2 global interrupt */
    SWPMI1_IRQHandler,                        /* Serial Wire Interface 1 global interrupt */
    TSC_IRQHandler,                           /* Touch Sense Controller global interrupt */
    0,                                        /* Reserved */
    0,                                        /* Reserved */
    RNG_IRQHandler,                           /* RNG global interrupt */
    FPU_IRQHandler,                           /* FPU */
};

__NO_RETURN void Reset_Handler(void)
{
    SystemInit();

    /* Enter PreMain (C library entry point) */
    __PROGRAM_START();
}

void Default_Handler(void)
{
    while(1);
}

/**
 * board_backtrace.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#ifndef __BOARD_BACKTRACE_H__
#define __BOARD_BACKTRACE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BACKTRACE_ARCH_CM3 = 0,
    BACKTRACE_ARCH_CM4 = 0,
} backtrace_arch_type_t;

#define BACKTRACE_ARCH_TYPE    BACKTRACE_ARCH_CM4

#if (BACKTRACE_ARCH_TYPE == BACKTRACE_ARCH_CM4)
void board_backtrace_cm4_trigger_HardFault(void);
void board_backtrace_cm4_trigger_MemManage(void);
void board_backtrace_cm4_trigger_BusFault(void);
void board_backtrace_cm4_trigger_UsageFault(void);
void board_backtrace_cm4_init(void);

#define BOARD_BACKTRACE_INIT()    board_backtrace_cm4_init()
#else
#define BOARD_BACKTRACE_INIT()
#endif

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_BACKTRACE_H__ */

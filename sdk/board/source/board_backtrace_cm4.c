/**
 * board_backtrace_cm4.c
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2026 Zeepunt
 */
#include "stm32l4xx.h"
#include "stm32l4xx_ll_cortex.h"

#include "board_common.h"
#include "board_backtrace.h"

typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;
} bt_stack_frame_t;

typedef struct {
    uint32_t exc_return;

    uint32_t ipsr;

    uint32_t msp;
    uint32_t psp;

    uint32_t cfsr;
    uint32_t hfsr;
    uint32_t shcsr;
    uint32_t icsr;

    uint32_t mmfar;
    uint32_t bfar;

    bt_stack_frame_t frame;
} bt_record_t;

static const char *TAG = "board_backtrace";

static volatile bt_record_t s_bt_record = {0};

#define BT_FAULT_HANDLER(name)                 \
    __attribute__((naked, used)) void name(void)   \
    {                                              \
        __asm volatile (                           \
            "mov r1, lr                \n"         \
            "tst r1, #4                \n"         \
            "ite eq                    \n"         \
            "mrseq r0, msp             \n"         \
            "mrsne r0, psp             \n"         \
            "b  priv_backtrace_handler \n"         \
        );                                         \
    }

BT_FAULT_HANDLER(HardFault_Handler)
BT_FAULT_HANDLER(MemManage_Handler)
BT_FAULT_HANDLER(BusFault_Handler)
BT_FAULT_HANDLER(UsageFault_Handler)

__attribute__((used)) static void priv_backtrace_handler(uint32_t *stack, uint32_t exc_return)
{
    s_bt_record.exc_return = exc_return;

    s_bt_record.ipsr = __get_IPSR();
    s_bt_record.msp  = __get_MSP();
    s_bt_record.psp  = __get_PSP();

    s_bt_record.cfsr  = SCB->CFSR;
    s_bt_record.hfsr  = SCB->HFSR;
    s_bt_record.shcsr = SCB->SHCSR;
    s_bt_record.icsr  = SCB->ICSR;
    s_bt_record.mmfar = SCB->MMFAR;
    s_bt_record.bfar  = SCB->BFAR;

    /* 硬件压栈帧的顺序固定为: r0, r1, r2, r3, r12, lr, pc, xpsr */
    s_bt_record.frame.r0   = stack[0];
    s_bt_record.frame.r1   = stack[1];
    s_bt_record.frame.r2   = stack[2];
    s_bt_record.frame.r3   = stack[3];
    s_bt_record.frame.r12  = stack[4];
    s_bt_record.frame.lr   = stack[5];
    s_bt_record.frame.pc   = stack[6];
    s_bt_record.frame.xpsr = stack[7];

    switch (s_bt_record.ipsr) {
        case 3: /* HardFault */
            BOARD_LOGW(TAG, "HardFault, HFSR : 0x%08x", s_bt_record.hfsr);
            break;

        case 4: /* MemManage */
            BOARD_LOGW(TAG, "MemManage");
            break;

        case 5: /* BusFault */
            BOARD_LOGW(TAG, "BusFault");
            break;

        case 6: /* UsageFault */
            BOARD_LOGW(TAG, "UsageFault");
            break;

        default:
            BOARD_LOGW(TAG, "unknown exception: %u", s_bt_record.ipsr);
            break;
    }

    /**
     * SCB->CFSR
     *  31          16 15       8 7      0
     * +--------------+----------+--------+
     * |     UFSR     |   BFSR   |  MFSR  |
     * +--------------+----------+--------+
     */
    (s_bt_record.cfsr & SCB_CFSR_IACCVIOL_Msk)    ? BOARD_LOGW(TAG, "MemManage IACCVIOL")    : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_DACCVIOL_Msk)    ? BOARD_LOGW(TAG, "MemManage DACCVIOL")    : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_MUNSTKERR_Msk)   ? BOARD_LOGW(TAG, "MemManage MUNSTKERR")   : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_MSTKERR_Msk)     ? BOARD_LOGW(TAG, "MemManage MSTKERR")     : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_MLSPERR_Msk)     ? BOARD_LOGW(TAG, "MemManage MLSPERR")     : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_MMARVALID_Msk)   ? BOARD_LOGW(TAG, "MemManage MMARVALID")   : (void)0;

    if (s_bt_record.cfsr & SCB_CFSR_MMARVALID_Msk) {
        BOARD_LOGW(TAG, "MemManage Address: 0x%08x", s_bt_record.mmfar);
    }

    (s_bt_record.cfsr & SCB_CFSR_IBUSERR_Msk)     ? BOARD_LOGW(TAG, "BusFault IBUSERR")     : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_PRECISERR_Msk)   ? BOARD_LOGW(TAG, "BusFault PRECISERR")   : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_IMPRECISERR_Msk) ? BOARD_LOGW(TAG, "BusFault IMPRECISERR") : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_UNSTKERR_Msk)    ? BOARD_LOGW(TAG, "BusFault UNSTKERR")    : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_STKERR_Msk)      ? BOARD_LOGW(TAG, "BusFault STKERR")      : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_LSPERR_Msk)      ? BOARD_LOGW(TAG, "BusFault LSPERR")      : (void)0;

    if (s_bt_record.cfsr & SCB_CFSR_BFARVALID_Msk) {
        BOARD_LOGW(TAG, "BusFault Address: 0x%08x", s_bt_record.bfar);
    }

    (s_bt_record.cfsr & SCB_CFSR_UNDEFINSTR_Msk)  ? BOARD_LOGW(TAG, "UsageFault UNDEFINSTR")  : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_INVSTATE_Msk)    ? BOARD_LOGW(TAG, "UsageFault INVSTATE")    : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_INVPC_Msk)       ? BOARD_LOGW(TAG, "UsageFault INVPC")       : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_NOCP_Msk)        ? BOARD_LOGW(TAG, "UsageFault NOCP")        : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_UNALIGNED_Msk)   ? BOARD_LOGW(TAG, "UsageFault UNALIGNED")   : (void)0;
    (s_bt_record.cfsr & SCB_CFSR_DIVBYZERO_Msk)   ? BOARD_LOGW(TAG, "UsageFault DIVBYZERO")   : (void)0;

    BOARD_LOGW(TAG, "MSP : 0x%08x, PSP: 0x%08x, EXC_RETURN: 0x%08x", s_bt_record.msp, s_bt_record.psp, s_bt_record.exc_return);
    BOARD_LOGW(TAG, "PC  : 0x%08x", s_bt_record.frame.pc);
    BOARD_LOGW(TAG, "LR  : 0x%08x", s_bt_record.frame.lr);
    BOARD_LOGW(TAG, "xPSR: 0x%08x", s_bt_record.frame.xpsr);
    BOARD_LOGW(TAG, "R0  : 0x%08x", s_bt_record.frame.r0);
    BOARD_LOGW(TAG, "R1  : 0x%08x", s_bt_record.frame.r1);
    BOARD_LOGW(TAG, "R2  : 0x%08x", s_bt_record.frame.r2);
    BOARD_LOGW(TAG, "R3  : 0x%08x", s_bt_record.frame.r3);
    BOARD_LOGW(TAG, "R12 : 0x%08x", s_bt_record.frame.r12);

    __BKPT(0);

    while (1) {
    }
}

void board_backtrace_cm4_trigger_HardFault(void)
{
    /* 通过关闭 UsageFault 异常来实现异常升级, 从而触发 HardFault */
    LL_HANDLER_DisableFault(LL_HANDLER_FAULT_USG);

    /* 执行未定义指令, 触发的是 UNDEFINSTR */
    __asm volatile (".word 0xFFFFFFFF");
}

void board_backtrace_cm4_trigger_MemManage(void)
{   
#if 1
    /**
     * bit0 = 1 表示合法 Thumb 目标
     *
     * 访问非法地址, 触发的是 IACCVIOL (取指失败), 此时 MMFAR 无效
     */
    __asm volatile (
        "ldr r0, =0xdeadbeef \n"
        "bx  r0              \n"
    );
#else
    /**
     * 用 MPU region 0 把 0x20001000 起 256 字节设为 no access
     *
     * 再写入该区域, 触发的是 DACCVIOL, 此时 MMFAR 有效
     */
    const uint32_t base = 0x20001000UL;

    /* 关闭 MPU, 避免配置过程中的中间状态触发意外 fault */
    MPU->CTRL = 0;

    /* 配置 region 0 */
    MPU->RNR = 0;
    MPU->RBAR = base;
    MPU->RASR = (1UL << 28) | /* XN     : 禁止执行 */
                (0UL << 24) | /* AP     : no access (特权/非特权都禁) */
                (7UL << 1)  | /* SIZE   : 2^(7+1) = 256 字节 */
                (1UL << 0);   /* ENABLE */

    /* 使能 MemManage 异常 + MPU */
    MPU->CTRL  = MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;
    __DSB();
    __ISB();

    /* 写 no-access 区域 */
    *(volatile uint32_t *)base = 0xDEADBEEF;
#endif
}

void board_backtrace_cm4_trigger_BusFault(void)
{
    /* 0x4FFF0000 在多数 Cortex-M4 的存储器映射中属于未映射区域 */
#if 1
    /* 写操作默认走写缓冲, 触发的是 IMPRECISERR (非精确数据总线错误), 此时 BFAR 无效 */
    *(volatile uint32_t *)0x4FFF0000UL = 0x12345678UL;
#else
    /**
     * 先将 ACTLR.DISDEFWBUF 置 1 表示禁用写缓冲
     *
     * 此时触发的是 PRECISERR (精确数据总线错误), 此时 BFAR 有效
     */
    SCnSCB->ACTLR |= SCnSCB_ACTLR_DISDEFWBUF_Msk;
    __DSB();
    __ISB();

    *(volatile uint32_t *)0x4FFF0000UL = 0x12345678UL;
#endif
}

void board_backtrace_cm4_trigger_UsageFault(void)
{
    /**
     * NULL 一般都是由 (void *)0 表示
     * bit0 = 0 表示 ARM 目标
     *
     * Cortex-M 只支持 Thumb, 触发的是 INVSTATE (非法状态)
     */
    void (*func)(void) = NULL;
    func();
}

void board_backtrace_cm4_init(void)
{
    LL_HANDLER_EnableFault(LL_HANDLER_FAULT_MEM);
    LL_HANDLER_EnableFault(LL_HANDLER_FAULT_BUS);
    LL_HANDLER_EnableFault(LL_HANDLER_FAULT_USG);

    BOARD_LOGI(TAG, "cortex-m4 backtrace init");
}

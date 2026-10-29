[代码仓库](https://github.com/jefftenney/LPTIM-Tick)

Commit：a63ca68062b81d0f05b83e56297e0d67a94f14e8



对于 FreeRTOS 来说：

1. `configUSE_TICKLESS_IDLE` 需要设为 2。
1. `vPortSetupTimerInterrupt` 和 `vPortSuppressTicksAndSleep` 函数由 `lptimTick.c` 内部的同名函数接管。
1. 不使用 `xPortSysTickHandler` 函数，tick 计数由 `lptimTick.c` 的 `LPTIM_IRQHandler` 函数接管。
1. `HAL_IncTick` 的调用还是要自己处理。
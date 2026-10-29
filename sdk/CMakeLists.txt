#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

if(CONFIG_SDK_CMSIS)
    add_subdirectory(CMSIS)
endif()

if(CONFIG_SDK_CMSIS_DEVICE)
    add_subdirectory(cmsis-device-l4)
endif()

if(CONFIG_SDK_STM32L4XX_HAL_DRIVER)
    add_subdirectory(stm32l4xx-hal-driver)
endif()

if(CONFIG_SDK_BOARD)
    add_subdirectory(board)
endif()

if(CONFIG_SDK_FREERTOS)
    add_subdirectory(FreeRTOS)
endif()

if(CONFIG_SDK_LPTIM_TICK)
    add_subdirectory(LPTIM-Tick)
endif()

if(CONFIG_SDK_SEGGER_RTT)
    add_subdirectory(RTT)
endif()

if(CONFIG_SDK_LVGL)
    add_subdirectory(lvgl)
endif()
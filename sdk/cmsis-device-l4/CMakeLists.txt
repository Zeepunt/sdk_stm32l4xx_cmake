#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(cmsis_device_incs
    "${CMAKE_CURRENT_SOURCE_DIR}/Include"
)

set(cmsis_device_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/system_stm32l4xx.c"
)

if(CONFIG_CHIP_STM32L475XX)
    list(APPEND cmsis_device_srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/port/startup_stm32l475xx.c"
    )
else()
    message(FATAL_ERROR "No startup file selected.")
endif()

component_register(
    SRCS
        ${cmsis_device_srcs}
    INCLUDE_DIRS
        ${cmsis_device_incs}
    REQUIRES
        "CMSIS"
)

if(CONFIG_STM32_CHIP)
    target_compile_definitions("${COMPONENT_LIB}"
        PUBLIC
            "${CONFIG_STM32_CHIP}"
    )
else()
    message(FATAL_ERROR "Unknown STM32 Chip")
endif()
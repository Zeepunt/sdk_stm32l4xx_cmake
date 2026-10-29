#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(lpm_incs
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(lpm_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/source/lpm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/lptimTick.c"
)

component_register(
    SRCS
        ${lpm_srcs}
    INCLUDE_DIRS
        ${lpm_incs}
    REQUIRES
        "stm32l4xx-hal-driver"
        "FreeRTOS"
)
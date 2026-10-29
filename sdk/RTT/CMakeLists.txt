#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(rtt_incs
    "${CMAKE_CURRENT_SOURCE_DIR}/RTT"
)

set(rtt_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/RTT/SEGGER_RTT_ASM_ARMv7M.S"
    "${CMAKE_CURRENT_SOURCE_DIR}/RTT/SEGGER_RTT_printf.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/RTT/SEGGER_RTT.c"
)

if("${TOOLCHAIN}" STREQUAL "gcc")
    list(APPEND rtt_srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/Syscalls/SEGGER_RTT_Syscalls_GCC.c"
    )
elseif("${TOOLCHAIN}" STREQUAL "armclang")
    list(APPEND rtt_srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/Syscalls/SEGGER_RTT_Syscalls_KEIL.c"
    )
else()
    message(FATAL_ERROR "Unknown TOOLCHAIN='${TOOLCHAIN}'")
endif()

component_register(
    SRCS
        ${rtt_srcs}
    INCLUDE_DIRS
        ${rtt_incs}
)

if(RTT_CONFIG_DIR)
    target_include_directories("${COMPONENT_LIB}"
        PUBLIC
            "${RTT_CONFIG_DIR}"
    )
else()
    message(FATAL_ERROR "Unknown Segger RTT Config")
endif()
#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(freertos_incs
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(freertos_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/source/croutine.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/event_groups.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/list.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/queue.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/stream_buffer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/tasks.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/timers.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/portable/MemMang/heap_4.c"
)

# Cortex-M4 without FPU 使用的是 ARM_CM3
# Cortex-M4 with FPU 使用的是 ARM_CM4F
if("${TOOLCHAIN}" STREQUAL "gcc")
    list(APPEND freertos_incs
        "${CMAKE_CURRENT_SOURCE_DIR}/portable/GCC/ARM_CM4F"
    )

    list(APPEND freertos_srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/portable/GCC/ARM_CM4F/port.c"
    )
elseif("${TOOLCHAIN}" STREQUAL "armclang")
    list(APPEND freertos_incs
        "${CMAKE_CURRENT_SOURCE_DIR}/portable/GCC/ARM_CM4F"
    )

    list(APPEND freertos_srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/portable/GCC/ARM_CM4F/port.c"
    )
else()
    message(FATAL_ERROR "Unknown TOOLCHAIN='${TOOLCHAIN}'")
endif()

component_register(
    SRCS
        ${freertos_srcs}
    INCLUDE_DIRS
        ${freertos_incs}
    REQUIRES
        "CMSIS"
)

if(FREERTOS_CONFIG_DIR)
    target_include_directories("${COMPONENT_LIB}"
        PUBLIC
            "${FREERTOS_CONFIG_DIR}"
    )
else()
    message(FATAL_ERROR "Unknown FreeRTOS Config")
endif()
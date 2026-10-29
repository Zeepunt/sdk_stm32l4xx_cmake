#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

set(LVGL_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/lvgl")

# lv_conf.h 所在的目录
if(LVGL_CONFIG_DIR)
    set(LV_BUILD_CONF_DIR "${LVGL_CONFIG_DIR}" CACHE PATH "Directory containing lv_conf.h for LVGL")
endif()

# ON: 使用 Kconfig 配置, OFF: 使用 lv_conf.h 配置
set(LV_BUILD_USE_KCONFIG OFF CACHE BOOL "Use Kconfig" FORCE)

set(LV_BUILD_TESTS OFF CACHE BOOL "Build the unit tests in the tests directory" FORCE)

if(NOT LV_BUILD_USE_KCONFIG)
    set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "Build demos" FORCE)
    set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
    set(CONFIG_LV_USE_THORVG OFF CACHE BOOL "Enable ThorVG" FORCE)
    set(CONFIG_LV_USE_THORVG_INTERNAL OFF CACHE BOOL "Use the internal version of ThorVG" FORCE)
endif()

include("${LVGL_ROOT_DIR}/env_support/cmake/main.cmake")

if(CONFIG_SDK_FREERTOS)
target_link_libraries(lvgl PUBLIC "FreeRTOS")
endif()
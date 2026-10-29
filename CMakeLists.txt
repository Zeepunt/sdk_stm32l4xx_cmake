#
# CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

cmake_minimum_required(VERSION 3.22)

# 将 cmake 目录添加到 CMAKE_MODULE_PATH 中
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# 加载 kconfig.cmake
include(kconfig)

# 加载 component.cmake
include(component)

add_subdirectory(sdk)
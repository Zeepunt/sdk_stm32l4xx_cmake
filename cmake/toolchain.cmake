#
# toolchain.cmake
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

# 可配置选项 (默认使用 GCC)
set(TOOLCHAIN "gcc" CACHE STRING "Compiler: gcc, armclang")
set_property(CACHE TOOLCHAIN PROPERTY STRINGS gcc armclang)

set(TOOLCHAIN_BIN_DIR "" CACHE PATH "Compiler bin directory")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(ARCH_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# 根据平台自动获取后缀 (Windows 为 .exe，Linux / macOS 为空)
set(TOOLCHAIN_EXE_SUFFIX "${CMAKE_HOST_EXECUTABLE_SUFFIX}")

if("${TOOLCHAIN}" STREQUAL "gcc")
    if(TOOLCHAIN_BIN_DIR)
        set(toolchain_prefix "${TOOLCHAIN_BIN_DIR}/arm-none-eabi-")
    else()
        set(toolchain_prefix "arm-none-eabi-")
    endif()

    set(CMAKE_C_COMPILER   "${toolchain_prefix}gcc${TOOLCHAIN_EXE_SUFFIX}")
    set(CMAKE_CXX_COMPILER "${toolchain_prefix}g++${TOOLCHAIN_EXE_SUFFIX}")
    set(CMAKE_ASM_COMPILER "${toolchain_prefix}gcc${TOOLCHAIN_EXE_SUFFIX}")
    set(CMAKE_OBJCOPY      "${toolchain_prefix}objcopy${TOOLCHAIN_EXE_SUFFIX}")
    set(CMAKE_OBJDUMP      "${toolchain_prefix}objdump${TOOLCHAIN_EXE_SUFFIX}")
    set(CMAKE_SIZE         "${toolchain_prefix}size${TOOLCHAIN_EXE_SUFFIX}")

    # 公共编译标志
    set(CMAKE_C_FLAGS_INIT   "${ARCH_FLAGS} -fdata-sections -ffunction-sections")
    set(CMAKE_ASM_FLAGS_INIT "${ARCH_FLAGS} -x assembler-with-cpp -Wa,-mimplicit-it=thumb")

    # 按构建类型区分的编译标志
    # Debug
    # -O0       : 编译器优化
    # -g3       : 生成调试信息的等级
    # -gdwarf-4 : 指定调试信息采用 DWARF 4 格式
    set(CMAKE_C_FLAGS_DEBUG_INIT     "-O0 -g3 -gdwarf-4")
    set(CMAKE_ASM_FLAGS_DEBUG_INIT   "-O0 -g3 -gdwarf-4")
    # Release
    set(CMAKE_C_FLAGS_RELEASE_INIT   "-O2 -g0")
    set(CMAKE_ASM_FLAGS_RELEASE_INIT "-O2 -g0")

    # 链接标志
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT   "-Wl,-O0")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE_INIT "-Wl,-O2")
elseif("${TOOLCHAIN}" STREQUAL "armclang")
    if(TOOLCHAIN_BIN_DIR)
        set(toolchain_prefix "${TOOLCHAIN_BIN_DIR}/")
    else()
        set(toolchain_prefix "")
    endif()

    set(CMAKE_C_COMPILER   "${toolchain_prefix}armclang${TOOLCHAIN_EXE_SUFFIX}")
    set(CMAKE_CXX_COMPILER "${toolchain_prefix}armclang${TOOLCHAIN_EXE_SUFFIX}")
    set(CMAKE_ASM_COMPILER "${toolchain_prefix}armclang${TOOLCHAIN_EXE_SUFFIX}")
    set(CMAKE_OBJCOPY      "${toolchain_prefix}fromelf${TOOLCHAIN_EXE_SUFFIX}")
    set(CMAKE_OBJDUMP      "${toolchain_prefix}fromelf${TOOLCHAIN_EXE_SUFFIX}")
    set(CMAKE_SIZE         "${toolchain_prefix}fromelf${TOOLCHAIN_EXE_SUFFIX}")

    # 公共编译标志
    set(CMAKE_C_FLAGS_INIT   "--target=arm-arm-none-eabi ${ARCH_FLAGS} -fdata-sections -ffunction-sections -funsigned-char -fshort-enums -fshort-wchar -fno-rtti")
    set(CMAKE_ASM_FLAGS_INIT "--target=arm-arm-none-eabi ${ARCH_FLAGS}")

    # 按构建类型区分的编译标志
    # Debug
    # -O1       : 编译器优化
    # -gdwarf-4 : 指定调试信息采用 DWARF 4 格式
    set(CMAKE_C_FLAGS_DEBUG_INIT     "-O1 -gdwarf-4")
    set(CMAKE_ASM_FLAGS_DEBUG_INIT   "-gdwarf-4")
    # Release
    set(CMAKE_C_FLAGS_RELEASE_INIT   "-O2")
    set(CMAKE_ASM_FLAGS_RELEASE_INIT "-O2")

    # 链接标志
    set(CMAKE_EXE_LINKER_FLAGS_INIT "--cpu=Cortex-M4.fp")
else()
    message(FATAL_ERROR "Unknown TOOLCHAIN='${TOOLCHAIN}'. Supported: gcc, armclang")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# cmake 探测编译器时会尝试试编译
# STATIC_LIBRARY 表示 试编译时不要链接成可执行文件, 只编译成静态库
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# NEVER: 不去 CMAKE_FIND_ROOT_PATH 下寻找
# ONLY : 只去 CMAKE_FIND_ROOT_PATH 下寻找
# BOTH : 在 CMAKE_FIND_ROOT_PATH 和宿主机默认路径 下寻找
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
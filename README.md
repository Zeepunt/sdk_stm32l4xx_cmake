

# 环境搭建

## Windows

Windows 10 22H2，x86-64。



当前使用的环境如下：

编译器

- arm-none-eabi-gcc：[Arm GNU Toolchain 15.3.Rel1](https://gitlab.arm.com/tooling/gnu-toolchains-for-arm/-/tree/releases/15.3.rel1)

  > `arm-gnu-toolchain-15.3.rel1-mingw-w64-x86_64-arm-none-eabi.zip`

- armclang：Arm Compiler for Embedded 6.19

  > 安装 Keil 5.38.0 时自带

依赖包

- CMake：[4.3.3](https://cmake.org/files/v4.3/)

  > `cmake-4.3.3-windows-x86_64.msi`

- Python：[3.12.13](https://www.python.org/downloads/release/python-31213/)

- kconfiglib

  ```shell
  $ pip install kconfiglib
  ```

  > 当前安装的版本是 14.1.0（[Github 仓库](https://github.com/ulfalizer/Kconfiglib)）

## MacOS



# 烧录工具

烧录的方式：

1. 使用官方的 `STM32CubeProgrammer`。

2. 使用开源的 `probe-rs`。

3. 其他。

## probe-rs

[Github 仓库](https://github.com/probe-rs/probe-rs)

- 安装

  Windows

  ```powershell
  > irm https://github.com/probe-rs/probe-rs/releases/latest/download/probe-rs-tools-installer.ps1 | iex
  ```

  Linux / MacOS

  ```shell
  $ curl --proto '=https' --tlsv1.2 -LsSf https://github.com/probe-rs/probe-rs/releases/latest/download/probe-rs-tools-installer.sh | sh
  ```

- 查看版本

  ```shell
  $ probe-rs --version
  probe-rs 0.32.0 (git commit: 48f5e4d)
  ```

- 查看支持的芯片型号

  Windows

  ```powershell
  > probe-rs chip list | Select-String -Pattern "stm32l4"
  ```

  Linux / MacOS

  ```shell
  $ probe-rs chip list | grep -i "stm32l4"
  ```

- 查看已连接的调试器

  这里获取的是调试器的 `<VID>:<PID>`。

  ```shell
  $ probe-rs list
  The following debug probes were found:
  [0]: ATK-FS-WINUSB-CMSIS-DAP -- 1a86:ff01-0:ATK 20230720 (CMSIS-DAP)
  
  $ probe-rs list
  The following debug probes were found:
  [0]: STLink V2-1 -- 0483:374b:066EFF574948786687254831 (ST-LINK)
  ```

- 烧录固件

  ```shell
  # 只烧录不运行
  $ probe-rs download --probe <VID>:<PID> --chip <芯片型号> <烧录文件>
  
  # 烧录并复位运行
  $ probe-rs run --probe <VID>:<PID> --chip <芯片型号> <烧录文件>
  
  # 示例
  $ probe-rs run --probe 0483:374b --chip STM32L475VETx ./build/hello_world.elf
  ```
  
  > 烧录文件
  >
  > - `.bin` 文件：常用于量产烧录、Bootloader 和 OTA 固件升级（烧录时需要明确目标地址）。
  > - `.hex` 文件：常用于开发调试阶段的固件烧录，也可以用于量产烧录（文件本身包含地址信息）。
  > - `.elf` 文件：主要用于调试，包含程序映像以及可选的符号、源码行号和变量等调试信息。
  
- 使用 Segger RTT 获取输出信息

  ```shell
  # 连接到一个已经在运行的设备并查看 RTT 日志
  $ probe-rs attach --probe <VID>:<PID> --chip <芯片型号> --rtt-up-channels 0 <烧录文件>
  
  # 烧录固件并查看 RTT 日志
  $ probe-rs run --probe <VID>:<PID> --chip <芯片型号> --rtt-up-channels 0 <烧录文件>
  
  # 示例
  $ probe-rs run --probe 0483:374b --chip STM32L475VETx --rtt-up-channels 0 ./build/hello_world.elf
  ```


# 使用

## CMake 配置阶段

1. 需要通过 `CMAKE_TOOLCHAIN_FILE` 指定使用的是 `cmake/toolchain.cmake` 文件。

   - Windows

     ```powershell
     > $env:SDK_STM32L4XX_PATH="D:\sdk\sdk_stm32l4xx"
     
     -DCMAKE_TOOLCHAIN_FILE="$env:SDK_STM32L4XX_PATH\cmake\toolchain.cmake"
     ```

   - Linux / MacOS

     ```shell
     $ export SDK_STM32L4XX_PATH="$HOME/sdk/sdk_stm32l4xx"
     
     -DCMAKE_TOOLCHAIN_FILE="$SDK_STM32L4XX_PATH/cmake/toolchain.cmake"
     ```

2. 编译工具链默认是使用系统环境 `PATH` 中提供的，但也可以通过 `TOOLCHAIN_BIN_DIR` 来指定其路径。

   - Windows

     ```powershell
     -DTOOLCHAIN_BIN_DIR="D:\toolchain\arm-gnu-toolchain-15.3.rel1\bin"
     ```
     
   - Linux / MacOS

     ```shell
     -DTOOLCHAIN_BIN_DIR="$HOME/toolchain/arm-gnu-toolchain-15.3.rel1/bin"
     ```

3. 编译工具链默认是使用 GCC，但也可以通过 `TOOLCHAIN` 来指定使用哪个。

   当前支持的工具链：

   - `-DTOOLCHAIN=gcc`
   - `-DTOOLCHAIN=armclang`

示例：

- Windows

  ```powershell
  > $env:SDK_STM32L4XX_PATH="D:\sdk\sdk_stm32l4xx"
  
  # gcc
  > cmake -B build `
          -G Ninja `
          -DCMAKE_TOOLCHAIN_FILE="$env:SDK_STM32L4XX_PATH\cmake\toolchain.cmake" `
          -DTOOLCHAIN=gcc `
          -DTOOLCHAIN_BIN_DIR="D:\toolchain\arm-gnu-toolchain-15.3.rel1\bin"
  
  # armclang
  > cmake -B build `
          -G Ninja `
          -DCMAKE_TOOLCHAIN_FILE="$env:SDK_STM32L4XX_PATH\cmake\toolchain.cmake" `
          -DTOOLCHAIN=armclang `
          -DTOOLCHAIN_BIN_DIR="D:\Program Files\Keil_v5\ARM\ARMCLANG\bin"
  
  > cmake --build build
  ```

## CMake 编译阶段

project 目录下具体某个项目的 CMakeLists.txt 文件开头需要加上：

```cmake
# 检测 SDK 路径
if(NOT DEFINED ENV{SDK_STM32L4XX_PATH})
    message(FATAL_ERROR "SDK directory not found")
endif()

if(NOT IS_DIRECTORY "$ENV{SDK_STM32L4XX_PATH}")
    message(FATAL_ERROR "SDK directory invalid: $ENV{SDK_STM32L4XX_PATH}")
endif()

# 项目名称
project(...)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# 放在 project 后面
add_subdirectory(
    "$ENV{SDK_STM32L4XX_PATH}"
    "${CMAKE_BINARY_DIR}/sdk"
)
```

## Kconfig 文件

```kconfig
rsource "$(SDK_STM32L4XX_PATH)/Kconfig"
```




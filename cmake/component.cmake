#
# component.cmake
#
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2026 Zeepunt

# 为当前文件设置一个全局唯一的包含标记
include_guard(GLOBAL)

#[[
Register a component as an OBJECT library and export COMPONENT_LIB.

.. command:: component_register

    .. code-block:: cmake

        component_register([SRCS <file>...]
                           [INCLUDE_DIRS <dir>...]
                           [PRIV_INCLUDE_DIRS <dir>...]
                           [LIBS <lib>...]
                           [REQUIRES <component>...])

    SRCS[in,opt] : List of source files for the component.

    INCLUDE_DIRS[in,opt] : List of public include directories for the created component library.

    PRIV_INCLUDE_DIRS[in,opt] : List of private include directories for the newly created component library.

    LIBS[in,opt] : List of external libraries.

    REQUIRES[in,opt] : List of publicly required components based on usage requirements.
#]]
function(component_register)
    set(options "")
    set(one_value "")
    set(multi_value SRCS INCLUDE_DIRS PRIV_INCLUDE_DIRS LIBS REQUIRES)

    cmake_parse_arguments(ARGS
        "${options}"
        "${one_value}"
        "${multi_value}"
        ${ARGN}
    )

    # 使用当前源码目录的目录名作为组件名与目标名
    get_filename_component(COMPONENT_NAME
        "${CMAKE_CURRENT_SOURCE_DIR}"
        NAME
    )

    if(TARGET "${COMPONENT_NAME}")
        message(FATAL_ERROR "Component '${COMPONENT_NAME}' already exists.")
    endif()

    if(NOT DEFINED KCONFIG_INCLUDE_DIR)
        message(WARNING "KCONFIG_INCLUDE_DIR is not defined.")
    endif()

    if(ARGS_SRCS)
        # 有源文件时创建对象库
        add_library("${COMPONENT_NAME}" OBJECT
            ${ARGS_SRCS}
        )

        target_include_directories("${COMPONENT_NAME}"
            PUBLIC
                "${KCONFIG_INCLUDE_DIR}"
                ${ARGS_INCLUDE_DIRS}
            PRIVATE
                ${ARGS_PRIV_INCLUDE_DIRS}
        )
    else()
        # 无源文件时创建接口库
        add_library("${COMPONENT_NAME}" INTERFACE)

        # 接口库使用 INTERFACE 作用域
        target_include_directories("${COMPONENT_NAME}"
            INTERFACE
                "${KCONFIG_INCLUDE_DIR}"
                ${ARGS_INCLUDE_DIRS}
        )
    endif()

    if(ARGS_LIBS)
        target_link_libraries("${COMPONENT_NAME}"
            INTERFACE
                ${ARGS_LIBS}
        )
    endif()

    if(ARGS_REQUIRES)
        target_link_libraries("${COMPONENT_NAME}"
            PUBLIC
                ${ARGS_REQUIRES}
        )
    endif()

    # 导出目标名给调用方
    set(COMPONENT_LIB "${COMPONENT_NAME}" PARENT_SCOPE)
endfunction()
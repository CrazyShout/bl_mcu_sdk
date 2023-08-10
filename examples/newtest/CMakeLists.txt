# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})
message(AUTHOR_WARNING "这是SDK根目录*** for debug" ${BL_SDK_BASE})
sdk_add_include_directories(.)

# sdk_add_include_directories(/home/xyj/work/wasm-micro-runtime/core/shared/platform/include)
# sdk_add_include_directories(/home/xyj/work/bl_mcu_sdk/components/os/freertos/include)
# sdk_add_include_directories(${BL_SDK_BASE}/components/os/freertos/posix/include)

include_directories(${BL_SDK_BASE}/components/os/freertos/include)
include_directories(${BL_SDK_BASE}/components/os/freertos/posix/include)
include_directories(${BL_SDK_BASE}/components/os/freertos/portable/GCC/RISC-V/c906)
# 以下拷贝自BL的rtos组件下的CMakeLists 不同的核心使用的include不同
# sdk_add_include_directories(portable/GCC/RISC-V/common)
# sdk_add_include_directories(portable/GCC/RISC-V/common/chip_specific_extensions/RV32I_CLINT_no_extensions)
sdk_set_main_file(main.c)
project(newtest)


enable_language(ASM)

set (WAMR_BUILD_PLATFORM "newtest")

# Build as X86_32 by default, change to "AARCH64[sub]", "ARM[sub]", "THUMB[sub]", "MIPS" or "XTENSA"
# if we want to support arm, thumb, mips or xtensa
if (NOT DEFINED WAMR_BUILD_TARGET)
  set (WAMR_BUILD_TARGET "RISCV64")
endif ()

if (NOT DEFINED WAMR_BUILD_INTERP)
  # Enable Interpreter by default
  set (WAMR_BUILD_INTERP 1)
endif ()

if (NOT DEFINED WAMR_BUILD_AOT)
  # Enable AOT by default.
  set (WAMR_BUILD_AOT 0)
endif ()

if (NOT DEFINED WAMR_BUILD_LIBC_BUILTIN)
  # Enable libc builtin support by default
  set (WAMR_BUILD_LIBC_BUILTIN 0)
endif ()

if (NOT DEFINED WAMR_BUILD_LIBC_WASI)
  # Disable libc wasi support by default
  set (WAMR_BUILD_LIBC_WASI 0)
endif ()

if (WAMR_BUILD_TARGET STREQUAL "RISCV64_LP64" OR WAMR_BUILD_TARGET STREQUAL "RISCV32_ILP32")
  set (WAMR_BUILD_FAST_INTERP 1) # 这个设置为1启动快速解释器模式，速度两倍，但是会消耗两倍的内存，目前启用会报错，暂时关闭
endif ()

# # Override the global heap usage
# if (NOT DEFINED WAMR_BUILD_GLOBAL_HEAP_POOL)
#   set (WAMR_BUILD_GLOBAL_HEAP_POOL 1)
# endif ()

# # Override the global heap size for small devices
# if (NOT DEFINED WAMR_BUILD_GLOBAL_HEAP_SIZE)
#   set (WAMR_BUILD_GLOBAL_HEAP_SIZE 131072) # 128 KB
# endif ()



# set (WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../wasm-micro-runtime/)
if (NOT DEFINED WAMR_ROOT_DIR)
  # this assumption is true if this file is copied to WAMR_ROOT
  set (WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wamr)
  message(AUTHOR_WARNING "这是wamr根目录" ${WAMR_ROOT_DIR})
endif ()

# wamr官方移植需求
include (${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(vmlib ${WAMR_RUNTIME_LIB_SOURCE})


target_sources(app PRIVATE 
              ${WAMR_RUNTIME_LIB_SOURCE}
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/platform/freertos/freertos_malloc.c
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/platform/freertos/freertos_platform.c
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/platform/freertos/freertos_thread.c
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/platform/freertos/freertos_time.c
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/mem-alloc/mem_alloc.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/mem-alloc/ems/ems_kfc.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/mem-alloc/ems/ems_alloc.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/mem-alloc/ems/ems_hmu.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_assert.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_common.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_hashmap.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_list.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_log.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_queue.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_vector.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/runtime_timer.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/libraries/libc-builtin/libc_builtin_wrapper.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_application.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_runtime_common.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_native.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_exec_env.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_memory.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_c_api.c 
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/arch/invokeNative_riscv.S
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/interpreter/wasm_interp_classic.c
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/interpreter/wasm_loader.c
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/interpreter/wasm_runtime.c
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/aot/aot_loader.c
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/aot/arch/aot_reloc_riscv.c
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/aot/aot_runtime.c
              ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/aot/aot_intrinsic.c
              iwasm.c
              )

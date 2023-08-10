# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})
message(AUTHOR_WARNING "这是SDK根目录*** for debug" ${BL_SDK_BASE})
sdk_add_include_directories(.)

# sdk_add_include_directories(${BL_SDK_BASE}/components/os/freertos/include)
# sdk_add_include_directories(${BL_SDK_BASE}/components/os/freertos/portable/GCC/RISC-V/c906)
# sdk_add_include_directories(${BL_SDK_BASE}/components/os/freertos/posix/include)
# sdk_add_include_directories(${BL_SDK_BASE}/components/os/freertos/portable/GCC/RISC-V/c906)
# sdk_add_include_directories(${BL_SDK_BASE}/components/os/freertos/portable/GCC/RISC-V/c906/chip_specific_extensions/Thead_E906FD_RV32IMAFDC)

# sdk_add_include_directories(/home/xyj/work/wasm-micro-runtime/core/shared/platform/include)
# sdk_add_include_directories(/home/xyj/work/bl_mcu_sdk/components/os/freertos/include)
# sdk_add_include_directories(${BL_SDK_BASE}/components/os/freertos/posix/include)
# set(CMAKE_INCLUDE_PATH /usr/include) 

# include_directories(${BL_SDK_BASE}/components/os/freertos/include)
# include_directories(${BL_SDK_BASE}/components/os/freertos/posix/include)
# include_directories(${BL_SDK_BASE}/components/os/freertos/portable/GCC/RISC-V/c906)
# 以下拷贝自BL的rtos组件下的CMakeLists 不同的核心使用的include不同
# sdk_add_include_directories(portable/GCC/RISC-V/common)
# sdk_add_include_directories(portable/GCC/RISC-V/common/chip_specific_extensions/RV32I_CLINT_no_extensions)


sdk_set_main_file(main.c)

# project(iwasm)


enable_language(ASM)

set (WAMR_BUILD_PLATFORM "freertos")

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
  set (WAMR_BUILD_AOT 1)
endif ()

if (NOT DEFINED WAMR_BUILD_LIBC_BUILTIN)
  # Enable libc builtin support by default
  set (WAMR_BUILD_LIBC_BUILTIN 1)
endif ()

if (NOT DEFINED WAMR_BUILD_LIBC_WASI)
  # Disable libc wasi support by default
  set (WAMR_BUILD_LIBC_WASI 1)
endif ()

if (WAMR_BUILD_TARGET STREQUAL "RISCV64_LP64" OR WAMR_BUILD_TARGET STREQUAL "RISCV32_ILP32")
  set (WAMR_BUILD_FAST_INTERP 0) # 这个设置为1启动快速解释器模式，速度两倍，但是会消耗两倍的内存，目前启用会报错，暂时关闭
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
# if (NOT DEFINED WAMR_ROOT_DIR)
#   # this assumption is true if this file is copied to WAMR_ROOT
#   set (WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wamr)
#   message(AUTHOR_WARNING "这是wamr根目录" ${WAMR_ROOT_DIR})
# endif ()

if (NOT DEFINED WAMR_ROOT_DIR)
# this assumption is true if this file is copied to WAMR_ROOT
set (WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wamr)
# set (WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../wasm-micro-runtime/)
message(AUTHOR_WARNING "这是wamr根目录" ${WAMR_ROOT_DIR})
endif ()

include (${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
sdk_generate_library(vmlib)# 生成静态库，加入SDK_LIBS全局属性，
# sdk_add_include_directories(${BL_SDK_BASE}/components/os/freertos/include)
# sdk_add_include_directories(${BL_SDK_BASE}/components/os/freertos/posix/include)
# sdk_add_include_directories(${RUNTIME_LIB_HEADER_LIST})
sdk_library_add_sources(${WAMR_RUNTIME_LIB_SOURCE})
target_link_libraries(vmlib PRIVATE libc mm freertos utils lhal std)
# target_compile_options(vmlib INTERFACE -fno-builtin)
sdk_add_link_libraries(vmlib)# vmlib链接到使用sdk_intf_lib的目标中，sdk_intf_lib为接口库起到传递作用，target_link_libraries(sdk_intf_lib INTERFACE vmlib)：vmlib不会链接到sdk_intf_lib，而是链接到使用sdk_intf_lib的目标

target_sources(app PRIVATE iwasm.c)

message(AUTHOR_WARNING "这是wamr头文件目录" ${RUNTIME_LIB_HEADER_LIST})
message(AUTHOR_WARNING "参与编译的wamr源文件：" ${WAMR_RUNTIME_LIB_SOURCE})
project(iwasm)# 这个宏展开后是对库进行链接，所以放在之后

# wamr官方移植需求
# include (${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
# add_library(vmlib ${WAMR_RUNTIME_LIB_SOURCE})
# set(source_files main.c)
# add_executable(iwasm main.c iwasm.c)
# target_link_libraries (${CMAKE_CURRENT_SOURCE_DIR}/build/build_out/iwasm_bl808_d0.elf vmlib)


# target_sources(app PRIVATE 
#               # ${WAMR_RUNTIME_LIB_SOURCE}
# #               ${BL_SDK_BASE}/components/os/freertos/croutine.c
# #               ${BL_SDK_BASE}/components/os/freertos/event_groups.c
# #               ${BL_SDK_BASE}/components/os/freertos/list.c
# #               ${BL_SDK_BASE}/components/os/freertos/queue.c
# #               ${BL_SDK_BASE}/components/os/freertos/tasks.c
# #               ${BL_SDK_BASE}/components/os/freertos/timers.c
# #               ${BL_SDK_BASE}/components/os/freertos/freertos_port.c
# #               ${BL_SDK_BASE}/components/os/freertos/portable/MemMang/heap_3.c

# #               ${BL_SDK_BASE}/components/os/freertos/portable/GCC/RISC-V/c906/port.c
# #               ${BL_SDK_BASE}/components/os/freertos/portable/GCC/RISC-V/c906/portASM.S


# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_clock.c
# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_mqueue.c
# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_pthread_barrier.c
# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_pthread_cond.c
# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_pthread_mutex.c
# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_pthread.c
# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_sched.c
# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_semaphore.c
# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_timer.c
# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_unistd.c
# #               ${BL_SDK_BASE}/components/os/freertos/posix/source/FreeRTOS_POSIX_utils.c

# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/platform/freertos/freertos_malloc.c
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/platform/freertos/freertos_platform.c
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/platform/freertos/freertos_thread.c
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/platform/freertos/freertos_time.c
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/mem-alloc/mem_alloc.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/mem-alloc/ems/ems_kfc.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/mem-alloc/ems/ems_alloc.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/mem-alloc/ems/ems_hmu.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_assert.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_common.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_hashmap.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_list.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_log.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_queue.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/bh_vector.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/shared/utils/runtime_timer.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/libraries/libc-builtin/libc_builtin_wrapper.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_application.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_runtime_common.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_native.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_exec_env.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_memory.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/wasm_c_api.c 
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/common/arch/invokeNative_riscv.S
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/interpreter/wasm_interp_classic.c
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/interpreter/wasm_loader.c
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/interpreter/wasm_runtime.c
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/aot/aot_loader.c
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/aot/arch/aot_reloc_riscv.c
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/aot/aot_runtime.c
# # #               # ${CMAKE_CURRENT_SOURCE_DIR}/wamr/core/iwasm/aot/aot_intrinsic.c
#               iwasm.c
#               )

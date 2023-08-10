/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <stdlib.h>
#include <string.h>
#include "bh_platform.h"
#include "bh_log.h"
#include "wasm_export.h"
#include "test_wasm.h"
#include "platform_api_vmcore.h"

static int app_argc = 2;
static char * para[] = {"7","8","NULL"};
static char **app_argv = para;

/**
 * Find the unique main function from a WASM module instance
 * and execute that function.
 *
 * @param module_inst the WASM module instance
 * @param argc the number of arguments
 * @param argv the arguments array
 *
 * @return true if the main function is called, false otherwise.
 */
bool
wasm_application_execute_main(wasm_module_inst_t module_inst, int32_t argc,
                              char *argv[]);

static void *
app_instance_main(wasm_module_inst_t module_inst)
{
    const char *exception;
    printf("A.0\r\n");
    wasm_application_execute_main(module_inst, 0, NULL);
    printf("A.1\r\n");
    if ((exception = wasm_runtime_get_exception(module_inst))){
        printf("%s\n", exception);
        printf("generate exception!\r\n");
    }
    return NULL;
}

static void *
app_instance_func(wasm_module_inst_t module_inst, const char *func_name)
{
    const char *exception;
    if(wasm_application_execute_func(module_inst, func_name, app_argc, app_argv))
        printf("application done successfully!\r\n");
    /* The result of wasm function or exception info was output inside
       wasm_application_execute_func(), here we don't output them again. */
    if ((exception = wasm_runtime_get_exception(module_inst))){
        printf("%s\n", exception);
        printf("generate exception!\r\n");
    }
    return NULL;
}

void
iwasm_main(void* args)
{
    uint8 *wasm_file_buf = NULL;
    uint32 wasm_file_size;
    wasm_module_t wasm_module = NULL;
    wasm_module_inst_t wasm_module_inst = NULL;
    RuntimeInitArgs init_args;
    char error_buf[128];
#if WASM_ENABLE_LOG != 0
    int log_verbose_level = 2;
#endif

    memset(&init_args, 0, sizeof(RuntimeInitArgs));

    init_args.mem_alloc_type = Alloc_With_Allocator;
    init_args.mem_alloc_option.allocator.malloc_func = os_malloc;
    init_args.mem_alloc_option.allocator.realloc_func = os_realloc;
    init_args.mem_alloc_option.allocator.free_func = os_free;
    
    /* initialize runtime environment */
    if (!wasm_runtime_full_init(&init_args)) {
        printf("Init runtime environment failed.\n");
        return;
    }
    
#if WASM_ENABLE_LOG != 0
    bh_log_set_verbose_level(log_verbose_level);
#endif
#if WASM_ENABLE_INTERP != 0
    /* load WASM byte buffer from byte buffer of include file */
//     wasm_file_buf = (uint8 *)wasm_test_file_interp;
//     wasm_file_size = sizeof(wasm_test_file_interp);

//     /* load WASM module */
//     if (!(wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size,
//                                           error_buf, sizeof(error_buf)))) {
//         printf("%s\n", error_buf);
//         goto fail1;
//     }
    
//     /* instantiate the module */
//     if (!(wasm_module_inst = wasm_runtime_instantiate(
//               wasm_module, 1024 * 1024, 1024 * 1024 * 16, error_buf, sizeof(error_buf)))) {
//         printf("%s\n", error_buf);
//         goto fail2;
//     }
    
//     // app_instance_main(wasm_module_inst);
//     app_instance_func(wasm_module_inst, "add");
    
//     /* destroy the module instance */
//     printf("start to destroy the module instance\r\n");
//     wasm_runtime_deinstantiate(wasm_module_inst);
//     printf("destroy the module instance successfully\r\n");

// fail2:
//     /* unload the module */
//     wasm_runtime_unload(wasm_module);

// fail1:
//     /* destroy runtime environment */
//     wasm_runtime_destroy();
#endif
    printf("Run wamr with AoT.\n");
    wasm_file_buf = (uint8_t *)wasm_test_file_aot;
    wasm_file_size = sizeof(wasm_test_file_aot);

    /* load WASM module */
    if (!(wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size,
                                          error_buf, sizeof(error_buf)))) {
        printf("%s\n", error_buf);
        goto fail1aot;
    }

    printf("Instantiate WASM runtime\r\n");
    if (!(wasm_module_inst =
              wasm_runtime_instantiate(wasm_module, 32 * 1024, // stack size
                                       32 * 1024,              // heap size
                                       error_buf, sizeof(error_buf)))) {
        printf("%s\n", error_buf);
        goto fail2aot;
    }

    printf("run function of the application\r\n");
    app_instance_func(wasm_module_inst, "add");
    // app_instance_main(wasm_module_inst);

    /* destroy the module instance */
    printf("Deinstantiate WASM runtime\r\n");
    wasm_runtime_deinstantiate(wasm_module_inst);

fail2aot:
    /* unload the module */
    printf("Unload WASM module\r\n");
    wasm_runtime_unload(wasm_module);
fail1aot:
    /* destroy runtime environment */
    printf("Destroy WASM runtime\r\n");
    wasm_runtime_destroy();
}

#define DEFAULT_THREAD_STACKSIZE (6 * 1024)
#define DEFAULT_THREAD_PRIORITY 50

bool
iwasm_init(void *pvParameters)
{
    pthread_t t;
    int res;
    pthread_attr_t tattr;


    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setstacksize(&tattr, 4096);

    puts("[OS] iwasm_main...\r\n");
    res = pthread_create(&t, &tattr, iwasm_main, (void *)NULL);
    // configASSERT(res)

    res = pthread_join(t, NULL);
    // configASSERT(res)
    

    // iwasm_main();
    // while(1){
    //     printf("programe ended!\r\n");
    //     bflb_mtimer_delay_ms(10000);
    // }
}
 
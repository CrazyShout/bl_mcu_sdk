#include <FreeRTOS.h>
#include <stdbool.h>
#include "bflb_mtimer.h"
#include "board.h"
#include "FreeRTOS_POSIX/pthread.h"


#define DBG_TAG "MAIN"
#include "log.h"

#include "bl808_glb_gpio.h"

extern bool iwasm_init(void *pvParameters);

static void sipeed_bl_sys_enabe_jtag(int cpuid)
{
    GLB_GPIO_Cfg_Type gpio_cfg;

    gpio_cfg.drive = 0;
    gpio_cfg.smtCtrl = 1;
    gpio_cfg.pullType = GPIO_PULL_NONE;

    gpio_cfg.gpioMode = GPIO_MODE_AF;
    switch (cpuid) {
        case 0: {
            gpio_cfg.gpioFun = GPIO_FUN_JTAG_M0;
            puts("Enable CPU0 (M0/E907) on PIN0/PIN1/PIN2/PIN3(TCK/TDI/TMS/TDO)\r\n");
        } break;
        case 1: {
            gpio_cfg.gpioFun = GPIO_FUN_JTAG_D0;
            puts("Enable CPU1 (D0/C906) on PIN0/PIN1/PIN2/PIN3(TCK/TDI/TMS/TDO)\r\n");
        } break;
        default: {
        } break;
    }
    gpio_cfg.gpioPin = GLB_GPIO_PIN_0;
    GLB_GPIO_Init(&gpio_cfg);

    gpio_cfg.gpioPin = GLB_GPIO_PIN_1;
    GLB_GPIO_Init(&gpio_cfg);

    gpio_cfg.gpioPin = GLB_GPIO_PIN_2;
    GLB_GPIO_Init(&gpio_cfg);

    gpio_cfg.gpioPin = GLB_GPIO_PIN_3;
    GLB_GPIO_Init(&gpio_cfg);
}

static void main_task(void* args)
{
    printf("main task started!\r\n");
    iwasm_init(NULL);
    // vTaskDelete(NULL);
    while(1)
    {
        printf("main task has ended!\r\n");
        bflb_mtimer_delay_ms(10000);
    }
}

extern uint32_t __HeapBase;
extern uint32_t __HeapLimit;

int main(void)
{
    // pthread_t t;
    // int res;

    // pthread_attr_t tattr;
    size_t heap_len = ((size_t)&__HeapLimit - (size_t)&__HeapBase);

    board_init();
    
    pmem_init((void*)&__HeapBase, heap_len);
    printf("Try to pmem init, Heap base is %x and heap len is %x \r\n", __HeapBase,heap_len);
    sipeed_bl_sys_enabe_jtag(1);
    mailbox_init();// 初始化mailbox

    // pthread_attr_init(&tattr);
    // pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);
    // pthread_attr_setstacksize(&tattr, 4096);
    // puts("[OS] iwasm_init...\r\n");
    // res = pthread_create(&t, &tattr, iwasm_init, (void *)NULL);
    // assert(res == 0);

    // res = pthread_join(t, NULL);
    // assert(res == 0);


    puts("[OS] Start...\r\n");
    xTaskCreate(main_task, (char*)"main_task", 1024, NULL, 1, NULL);
    // printf("iwasm start!\r\n");
    vTaskStartScheduler();

    // iwasm_init(NULL);
    while(1){
        LOG_I("First Hello!\r\n");
        LOG_F("hello world fatal\r\n");
        LOG_E("hello world error\r\n");
        LOG_W("hello world warning\r\n");
        LOG_I("hello world information\r\n");
        LOG_D("hello world debug\r\n");
        LOG_T("hello world trace\r\n");
        LOG_RF("hello world fatal raw\r\n");
        LOG_RE("hello world error raw\r\n");
        LOG_RW("hello world warning raw\r\n");
        LOG_RI("hello world information raw\r\n");
        LOG_RD("hello world debug raw\r\n");
        LOG_RT("hello world trace raw\r\n");
        printf("Hello! This is os printf!\r\n");
        bflb_mtimer_delay_ms(1000);
        // iwasm_init();
        LOG_RT("GoodBye!\r\n");
    }

}

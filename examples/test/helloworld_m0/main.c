#include "bflb_mtimer.h"
#include "board.h"

#define DBG_TAG "MAIN"
#include "log.h
#include "bl808_glb_gpio.h"

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
int main(void)
{
    board_init();
    sipeed_bl_sys_enabe_jtag(1);
    while (1) {
        LOG_F("hello world m0\r\n");
        LOG_E("hello world m0\r\n");
        LOG_W("hello world m0\r\n");
        LOG_I("hello world m0\r\n");
        LOG_D("hello world m0\r\n");
        LOG_T("hello world m0\r\n");
        bflb_mtimer_delay_ms(1000);
    }
}

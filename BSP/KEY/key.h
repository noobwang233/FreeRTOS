#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

typedef enum
{
    KEY_PRESS = 0,
    KEY_RELEASE = !KEY_PRESS,
} key_gpio_state;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

typedef enum
{
    LOW_VALID = 0,
    HIGH_VALID = !LOW_VALID
} key_valid_type;

struct exti_type
{
    EXTI_InitTypeDef    exit_cfg;
    uint8_t             port_source;
    uint8_t             pin_source;
};
struct key_init_type
{
    GPIO_InitTypeDef        gpio_type;
    GPIO_TypeDef *          gpio_port;
    uint32_t                key_clk;
    struct exti_type        key_exit;
    NVIC_InitTypeDef        key_nvic;
    const key_valid_type    key_valid;
};

typedef enum
{
    RELEASE_STATE,        //空闲
    SHORT_PRESS_STATE,    //短按
    LONG_PRESS_STATE,     //长按
    DOUBLE_PRESS_STATE,   //双击
} KeyState;

struct key_task_type
{
    uint8_t key_index;
    char * key_name;
    KeyState key_state;
    TaskHandle_t key_handle;
};

/* configure key */
void key_init(uint8_t key_num, keymode_typedef_enum key_mode);
/* return the selected key state */
key_gpio_state key_state_get(uint8_t key_num);
void key_task_init(uint8_t key_num);

#endif

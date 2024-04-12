#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10x.h"

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

struct exti_type
{
    EXTI_InitTypeDef    exit_cfg;
    uint8_t             port_source;
    uint8_t             pin_source;
};
struct key_init_type
{
    GPIO_InitTypeDef    gpio_type;
    GPIO_TypeDef *      gpio_port;
    uint32_t            key_clk;
    struct exti_type    key_exit;
    NVIC_InitTypeDef    key_nvic;
};

typedef enum
{
    RELEASE_STATE,        //¿ÕÏÐ
    SHORT_PRESS_STATE,    //¶Ì°´
    LONG_PRESS_STATE,     //³¤°´
    DOUBLE_PRESS_STATE,   //Ë«»÷
} KeyState;

struct key
{
    key_typedef_enum key;
    char * key_name;
    KeyState key_state;
}

/* configure key */
void key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
/* return the selected key state */
key_state key_state_get(key_typedef_enum key);

#endif

#ifndef STM32F103_ATK_H
#define STM32F103_ATK_H

#include <stdint.h>
#ifdef cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"



/* exported types */
typedef enum 
{
    LED0,           //PB5
    LED1,           //PE5
    BEEP            //PB8 BEEP
} led_typedef_enum;

typedef enum 
{
    KEY_0,         //PE4
    KEY_1,         //PE3
    KEY_UP         //PA0  WK_UP
} key_typedef_enum;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;



/* eval board low layer led */
#define LEDn                             2U
struct led_type
{
    GPIO_InitTypeDef    gpio_type;
    GPIO_TypeDef *      gpio_port;
    uint32_t            led_clk;
};

/* user push-button */
#define KEYn                             3U
struct exti_type
{
    EXTI_InitTypeDef    exit_cfg;
    uint8_t             port_source;
    uint8_t             pin_source;
};
struct key_type
{
    GPIO_InitTypeDef    gpio_type;
    GPIO_TypeDef *      gpio_port;
    uint32_t            key_clk;
    struct exti_type    key_exit;
    NVIC_InitTypeDef    key_nvic;
};


#define COMn                             1U
struct com_type
{
    USART_TypeDef *     usart;
    USART_InitTypeDef   usart_cfg;
    uint32_t            usart_clk;
    uint16_t            tx_pin;
    uint16_t            rx_pin;
    GPIO_TypeDef *      gpio_port;
    uint32_t            gpio_clk;
};

#define COM   com0

/* function declarations */
/* configure led GPIO */
void board_led_init(led_typedef_enum lednum);
/* turn on selected led */
void board_led_on(led_typedef_enum lednum);
/* turn off selected led */
void board_led_off(led_typedef_enum lednum);
/* toggle the selected led */
void board_led_toggle(led_typedef_enum lednum);
/* configure key */
void board_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
/* return the selected key state */
uint8_t board_key_state_get(key_typedef_enum key);
/* configure COM port */
void board_com_init(uint32_t com);
#ifdef cplusplus
}
#endif

#endif /* GD32F303C_START_H */

#ifndef STM32F103_ATK_H
#define STM32F103_ATK_H

#include <stdint.h>
#ifdef cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"
#include "key.h"


/* exported types */
#define LEDn                             3U
typedef enum 
{
    LED0,           //PB5
    LED1,           //PE5
    BEEP            //PB8 BEEP
} led_typedef_enum;

#define KEYn                             3U
typedef enum 
{
    KEY_0 = 0,          //PE4
    KEY_1,              //PE3
    KEY_UP              //PA0  WK_UP
} key_index;

#define COM   com0

/* eval board low layer led */
struct led_type
{
    GPIO_InitTypeDef    gpio_type;
    GPIO_TypeDef *      gpio_port;
    uint32_t            led_clk;
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

/* function declarations */
/* configure led GPIO */
void board_led_init(led_typedef_enum lednum);
/* turn on selected led */
void board_led_on(led_typedef_enum lednum);
/* turn off selected led */
void board_led_off(led_typedef_enum lednum);
/* toggle the selected led */
void board_led_toggle(led_typedef_enum lednum);

/* configure COM port */
void board_com_init(uint32_t com);
#ifdef cplusplus
}
#endif

#endif /* GD32F303C_START_H */

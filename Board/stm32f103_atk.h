#ifndef STM32F103_ATK_H
#define STM32F103_ATK_H

#ifdef cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"
     
/* exported types */
typedef enum 
{
    LED0,           //PB5
    LED1            //PE5
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

#define LED0_PIN                         GPIO_PIN_5
#define LED0_GPIO_PORT                   GPIOB
#define LED0_GPIO_CLK                    RCC_APB2Periph_GPIOB

#define LED1_PIN                         GPIO_PIN_5
#define LED1_GPIO_PORT                   GPIOE
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOB

#define KEYn                             1U

/* user push-button */
#define KEY0_PIN                     GPIO_PIN_4
#define KEY0_GPIO_PORT               GPIOE
#define KEY0_GPIO_CLK                RCC_APB2Periph_GPIOE
#define KEY0_EXTI_LINE               EXTI_0
#define KEY0_EXTI_PORT_SOURCE        GPIO_PORT_SOURCE_GPIOA
#define KEY0_EXTI_PIN_SOURCE         GPIO_PIN_SOURCE_0
#define KEY0_EXTI_IRQn               EXTI0_IRQn

#define COMn                             2U

#define EVAL_COM1                        USART0
#define EVAL_COM1_CLK                    RCU_USART0
#define EVAL_COM1_TX_PIN                 GPIO_PIN_9
#define EVAL_COM1_RX_PIN                 GPIO_PIN_10
#define EVAL_COM1_GPIO_PORT              GPIOA
#define EVAL_COM1_GPIO_CLK               RCU_GPIOA

#define EVAL_COM2                        USART1
#define EVAL_COM2_CLK                    RCU_USART1
#define EVAL_COM2_TX_PIN                 GPIO_PIN_2
#define EVAL_COM2_RX_PIN                 GPIO_PIN_3
#define EVAL_COM2_GPIO_PORT              GPIOA
#define EVAL_COM2_GPIO_CLK               RCU_GPIOA

/* function declarations */
/* configure led GPIO */
void gd_eval_led_init(led_typedef_enum lednum);
/* turn on selected led */
void gd_eval_led_on(led_typedef_enum lednum);
/* turn off selected led */
void gd_eval_led_off(led_typedef_enum lednum);
/* toggle the selected led */
void gd_eval_led_toggle(led_typedef_enum lednum);
/* configure key */
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
/* return the selected key state */
uint8_t gd_eval_key_state_get(key_typedef_enum key);
/* configure COM port */
void gd_eval_com_init(uint32_t com);
#ifdef cplusplus
}
#endif

#endif /* GD32F303C_START_H */

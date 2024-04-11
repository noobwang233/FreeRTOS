#include "stm32f103_atk.h"
#include <stdint.h>

/* private variables */
struct key_type* KEYS[KEYn] = {&key0, &key1, &key_up};
struct led_type* LEDS[KEYn] = {&led0, &led1, &beep};
struct com_type* COMS[KEYn] = {&com0};
/*!
    \brief      configure led GPIO
    \param[in]  led_num: specify the led to be configured
      \arg        LED0
    \param[out] none
    \retval     none
*/
void  board_led_init (led_typedef_enum led_num)
{
    /* enable the led clock */
    RCC_APB2PeriphClockCmd(LEDS[led_num]->led_clk, ENABLE);
    /* configure led GPIO port */ 
    GPIO_Init(LEDS[led_num]->gpio_port, &LEDS[led_num]->gpio_type);

    GPIO_ResetBits(LEDS[led_num]->gpio_port, LEDS[led_num]->gpio_type.GPIO_Pin);
}

/*!
    \brief      turn on selected led
    \param[in]  led_num: specify the led to be turned on
      \arg        LED0
    \param[out] none
    \retval     none
*/
void board_led_on(led_typedef_enum led_num)
{
    GPIO_SetBits(LEDS[led_num]->gpio_port, LEDS[led_num]->gpio_type.GPIO_Pin);
}

/*!
    \brief      turn off selected led
    \param[in]  led_num: specify the led to be turned off
      \arg        LED0
    \param[out] none
    \retval     none
*/
void board_led_off(led_typedef_enum led_num)
{
    GPIO_ResetBits(LEDS[led_num]->gpio_port, LEDS[led_num]->gpio_type.GPIO_Pin);
}

/*!
    \brief      toggle selected led
    \param[in]  led_num: specify the led to be toggled
      \arg        LED0
    \param[out] none
    \retval     none
*/
void board_led_toggle(led_typedef_enum led_num)
{
    uint8_t led_state = GPIO_ReadOutputDataBit(LEDS[led_num]->gpio_port, LEDS[led_num]->gpio_type.GPIO_Pin);
    GPIO_WriteBit(LEDS[led_num]->gpio_port, LEDS[led_num]->gpio_type.GPIO_Pin, !led_state);
}

/*!
    \brief      configure key
    \param[in]  key_num: specify the key to be configured
      \arg        KEY_USER: user key
      \arg        KEY...
    \param[in]  key_mode: specify button mode
      \arg        KEY_MODE_GPIO: key will be used as simple IO
      \arg        KEY_MODE_EXTI: key will be connected to EXTI line with interrupt
    \param[out] none
    \retval     none
*/
void board_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode)
{
    /* enable the key clock */
    RCC_APB2PeriphClockCmd(KEYS[key_num]->key_clk, ENABLE);

    /* configure button pin as input */
    GPIO_Init(KEYS[key_num]->gpio_port, &KEYS[key_num]->gpio_type);

    if (key_mode == KEY_MODE_EXTI) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
        
        GPIO_EXTILineConfig(KEYS[key_num]->key_exit.port_source,KEYS[key_num]->key_exit.pin_source);
        
        EXTI_Init(&KEYS[key_num]->key_exit.exit_cfg);

        NVIC_Init(&KEYS[key_num]->key_nvic);
    }
}

/*!
    \brief      return the selected key state
    \param[in]  key: specify the key to be checked
      \arg        KEY_USER: user key
    \param[out] none
    \retval     the key's GPIO pin value
*/
uint8_t board_key_state_get(key_typedef_enum key)
{
    return GPIO_ReadInputDataBit(KEYS[key]->gpio_port, KEYS[key]->gpio_type.GPIO_Pin);
}

void board_com_init(uint32_t bound)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    /* enable GPIO clock */
    RCC_APB2PeriphClockCmd(COM.gpio_clk, ENABLE);

    /* enable USART clock */
    RCC_APB2PeriphClockCmd(COM.usart_clk, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    /* connect port to USARTx_Tx */
    GPIO_InitStructure.GPIO_Pin = COM.tx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM.gpio_port, &GPIO_InitStructure);

    /* connect port to USARTx_Rx */
    GPIO_InitStructure.GPIO_Pin = COM.rx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(COM.gpio_port, &GPIO_InitStructure);

    /* USART configure */
    COM.usart_cfg.USART_BaudRate = bound;
    USART_Init(COM.usart, &COM.usart_cfg); //初始化串口
    USART_Cmd(COM.usart, ENABLE);
}

/* retarget the C library printf function to the USART */
int _write (int fd, char *pBuffer, int size)  
{  
    for (int i = 0; i < size; i++)  
    {  
		USART_SendData(COM.usart, (uint8_t)pBuffer[i]);
		while(RESET == USART_GetFlagStatus(COM.usart, USART_FLAG_TC));		
    }  
    return size;  
}
#include "stm32f103_atk.h"
#include <stdint.h>
#include "key.h"
#include "led.h"

/* private variables */
static struct key_init_type key0 = {
    {
        GPIO_Pin_4, 
        0, 
        GPIO_Mode_IPU
    }, 
    GPIOE, 
    RCC_APB2Periph_GPIOE, 
    {
        {
            EXTI_Line4,
            EXTI_Mode_Interrupt,
            EXTI_Trigger_Falling,
            ENABLE
        },
        GPIO_PortSourceGPIOE, 
        GPIO_PinSource4, 
    },
    {
        EXTI4_IRQn,
        0x02,
        0x03,
        ENABLE
    },
    LOW_VALID
};


static struct key_init_type key1 = {
    {
        GPIO_Pin_3, 
        0, 
        GPIO_Mode_IPU
    }, 
    GPIOE, 
    RCC_APB2Periph_GPIOE, 
    {
        {
            EXTI_Line3,
            EXTI_Mode_Interrupt,
            EXTI_Trigger_Falling,
            ENABLE
        },
        GPIO_PortSourceGPIOE, 
        GPIO_PinSource3, 
    },
    {
        EXTI3_IRQn,
        0x02,
        0x02,
        ENABLE
    },
    LOW_VALID
};


static struct key_init_type key_up = {
    {
        GPIO_Pin_0, 
        0, 
        GPIO_Mode_IPD
    }, 
    GPIOA, 
    RCC_APB2Periph_GPIOA, 
    {
        {
            EXTI_Line0,
            EXTI_Mode_Interrupt,
            EXTI_Trigger_Rising,
            ENABLE
        },
        GPIO_PortSourceGPIOA, 
        GPIO_PinSource0, 
    },
    {
        EXTI0_IRQn,
        0x02,
        0x02,
        ENABLE
    },
    HIGH_VALID
};

static struct key_init_type* key_cfg[KEYn] = {&key0, &key1, &key_up};
struct key_init_type** key_cfgs = key_cfg;

struct key_task_type key_0_task = 
{
    .key_index = KEY_0,
    .key_name = "KEY_0",
};
struct key_task_type key_1_task = 
{
    .key_index = KEY_1,
    .key_name = "KEY_1",
};
struct key_task_type key_up_task = 
{
    .key_index = KEY_UP,
    .key_name = "KEY_UP",
};
static struct key_task_type* key_task[KEYn] = {&key_0_task, &key_1_task, &key_up_task};
struct key_task_type** key_tasks = key_task;


static struct led_init_type led0 = {{GPIO_Pin_5, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}, GPIOB, RCC_APB2Periph_GPIOB};
static struct led_init_type led1 = {{GPIO_Pin_5, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}, GPIOE, RCC_APB2Periph_GPIOE};
static struct led_init_type beep = {{GPIO_Pin_8, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}, GPIOB, RCC_APB2Periph_GPIOB};
static struct led_init_type* led_cfg[LEDn] = {&led0, &led1, &beep};
struct led_init_type** led_cfgs = led_cfg;


static struct com_type com0 = {
    USART1,
    {
        115200,
        USART_WordLength_8b,
        USART_StopBits_1,
        USART_Parity_No,
        USART_Mode_Rx | USART_Mode_Tx,
        USART_HardwareFlowControl_None
    },
    RCC_APB2Periph_USART1, 
    GPIO_Pin_9, 
    GPIO_Pin_10, 
    GPIOA, 
    RCC_APB2Periph_GPIOA
};


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
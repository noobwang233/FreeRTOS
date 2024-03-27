/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x 
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x.h"
#include "gd32f303c_start.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include <stdio.h>


TaskHandle_t LED1_Task_Handle = NULL; /* 任务句柄 */
TaskHandle_t LED2_Task_Handle = NULL;
TaskHandle_t TaskCreate_Handle = NULL;

static void LED1_Task(void* parameter)
{
    while (1)
    {
        gd_eval_led_on(LED1);
        vTaskDelay(500); /* 延时500 个tick */
        gd_eval_led_off(LED1);
        vTaskDelay(500); /* 延时500 个tick */
    }
}


static void LED2_Task(void* parameter)
{
    while (1)
    {
        gd_eval_led_on(LED2);
        vTaskDelay(1000); /* 延时500 个tick */
        gd_eval_led_off(LED2);
        vTaskDelay(1000); /* 延时500 个tick */
    }
}

static void TaskCreate_Task(void* parameter)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

    taskENTER_CRITICAL(); //进入临界区

    xReturn = xTaskCreate(  (TaskFunction_t )LED1_Task, /* 任务入口函数 */
                                (const char* )"LED1_Task",/* 任务名字 */
                            (uint16_t )512, /* 任务栈大小 */
                            (void* )NULL, /* 任务入口函数参数 */
                            (UBaseType_t )2, /* 任务的优先级 */
                            (TaskHandle_t* )&LED1_Task_Handle);/* 任务句柄指针 */
    if (pdPASS == xReturn)
    {
        printf("Create LED1_Task successfully!\r\n");
    }
    else
    {
        printf("Create LED1_Task failed!\r\n");
    }

    xReturn = xTaskCreate(  (TaskFunction_t )LED2_Task, /* 任务入口函数 */
                                (const char* )"LED2_Task",/* 任务名字 */
                            (uint16_t )512, /* 任务栈大小 */
                            (void* )NULL, /* 任务入口函数参数 */
                            (UBaseType_t )2, /* 任务的优先级 */
                            (TaskHandle_t* )&LED2_Task_Handle);/* 任务句柄指针 */
    if (pdPASS == xReturn)
    {
        printf("Create LED2_Task successfully!\r\n");
    }
    else
    {
        printf("Create LED2_Task failed!\r\n");
    }

    vTaskDelete(TaskCreate_Handle); //删除TaskCreate 任务

    taskEXIT_CRITICAL(); //退出临界区
}

int main(void)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_com_init(EVAL_COM1);

    xReturn = xTaskCreate(  (TaskFunction_t )TaskCreate_Task, /* 任务入口函数 */
                                (const char* )"TaskCreate_Task",/* 任务名字 */
                            (uint16_t )128, /* 任务栈大小 */
                            (void* )NULL, /* 任务入口函数参数 */
                            (UBaseType_t )3, /* 任务的优先级 */
                            (TaskHandle_t* )&TaskCreate_Handle);/* 任务句柄指针 */
    if (pdPASS == xReturn)
        vTaskStartScheduler(); /* 启动任务，开启调度 */

    while (1){}
}

/* retarget the C library printf function to the USART */
int _write (int fd, char *pBuffer, int size)  
{  
    for (int i = 0; i < size; i++)  
    {  
		usart_data_transmit(EVAL_COM1, (uint8_t)pBuffer[i]);
		while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));		
    }  
    return size;  
}
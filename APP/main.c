#include "stm32f10x.h"
#include "misc.h"
#include "stm32f103_atk.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

TaskHandle_t LED1_Task_Handle = NULL; /* 任务句柄 */
TaskHandle_t LED2_Task_Handle = NULL;
TaskHandle_t TaskCreate_Handle = NULL;

static void LED1_Task(void* parameter)
{
    while (1)
    {
        board_led_on(LED0);
        vTaskDelay(500); /* 延时500 个tick */
        board_led_off(LED0);
        vTaskDelay(500); /* 延时500 个tick */
    }
}


static void LED2_Task(void* parameter)
{
    while (1)
    {
        board_led_on(LED1);
        vTaskDelay(800); /* 延时500 个tick */
        board_led_off(LED1);
        vTaskDelay(800); /* 延时500 个tick */
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

int main()
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    board_led_init(LED0);
    board_led_init(LED1);
    board_com_init(115200);

    xReturn = xTaskCreate(  (TaskFunction_t )TaskCreate_Task, /* 任务入口函数 */
                                (const char* )"TaskCreate_Task",/* 任务名字 */
                            (uint16_t )128, /* 任务栈大小 */
                            (void* )NULL, /* 任务入口函数参数 */
                            (UBaseType_t )3, /* 任务的优先级 */
                            (TaskHandle_t* )&TaskCreate_Handle);/* 任务句柄指针 */
    if (pdPASS == xReturn)
        vTaskStartScheduler(); /* 启动任务，开启调度 */

    while (1){}


    return 0;
}
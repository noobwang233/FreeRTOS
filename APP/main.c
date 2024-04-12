#include "stm32f10x.h"
#include "misc.h"
#include "stm32f103_atk.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

TaskHandle_t LED0_Task_Handle = NULL; /* 任务句柄 */
TaskHandle_t LED1_Task_Handle = NULL;
TaskHandle_t TaskCreate_Handle = NULL;
TaskHandle_t KEY_0_Task_Handle = NULL;
TaskHandle_t KEY_1_Task_Handle = NULL;
TaskHandle_t KEY_UP_Task_Handle = NULL;

static void LED_Task(void* parameter)
{
    led_typedef_enum LED = (led_typedef_enum)parameter;
    while (1)
    {
        board_led_on(LED);
        vTaskDelay(pdMS_TO_TICKS(500)); /* 延时500 ms */
        board_led_off(LED);
        vTaskDelay(pdMS_TO_TICKS(500)); /* 延时500 ms */
    }
}



static void TaskCreate_Task(void* parameter)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

    taskENTER_CRITICAL(); //进入临界区

    xReturn = xTaskCreate(  (TaskFunction_t )LED_Task, /* 任务入口函数 */
                                (const char* )"LED0_Task",/* 任务名字 */
                            (uint16_t )128, /* 任务栈大小 */
                            (void* )LED0, /* 任务入口函数参数 */
                            (UBaseType_t )2, /* 任务的优先级 */
                            (TaskHandle_t* )&LED0_Task_Handle);/* 任务句柄指针 */
    if (pdPASS == xReturn)
    {
        printf("Create LED0_Task successfully!\r\n");
    }
    else
    {
        printf("Create LED0_Task failed!\r\n");
    }

    xReturn = xTaskCreate(  (TaskFunction_t )LED_Task, /* 任务入口函数 */
                                (const char* )"LED1_Task",/* 任务名字 */
                            (uint16_t )128, /* 任务栈大小 */
                            (void* )LED1, /* 任务入口函数参数 */
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

    // xReturn = xTaskCreate(  (TaskFunction_t )KEY_Task, /* 任务入口函数 */
    //                             (const char* )"KEY_0_Task",/* 任务名字 */
    //                         (uint16_t )256, /* 任务栈大小 */
    //                         (void* )KEY_0, /* 任务入口函数参数 */
    //                         (UBaseType_t )2, /* 任务的优先级 */
    //                         (TaskHandle_t* )&KEY_0_Task_Handle);/* 任务句柄指针 */
    // if (pdPASS == xReturn)
    // {
    //     printf("Create KEY_0_Task successfully!\r\n");
    // }
    // else
    // {
    //     printf("Create KEY_0_Task failed!\r\n");
    // }

    // xReturn = xTaskCreate(  (TaskFunction_t )KEY_Task, /* 任务入口函数 */
    //                             (const char* )"KEY_1_Task",/* 任务名字 */
    //                         (uint16_t )256, /* 任务栈大小 */
    //                         (void* )KEY_1, /* 任务入口函数参数 */
    //                         (UBaseType_t )2, /* 任务的优先级 */
    //                         (TaskHandle_t* )&KEY_1_Task_Handle);/* 任务句柄指针 */
    // if (pdPASS == xReturn)
    // {
    //     printf("Create KEY_1_Task successfully!\r\n");
    // }
    // else
    // {
    //     printf("Create KEY_1_Task failed!\r\n");
    // }

    // xReturn = xTaskCreate(  (TaskFunction_t )KEY_Task, /* 任务入口函数 */
    //                             (const char* )"KEY_UP_Task",/* 任务名字 */
    //                         (uint16_t )256, /* 任务栈大小 */
    //                         (void* )KEY_UP, /* 任务入口函数参数 */
    //                         (UBaseType_t )2, /* 任务的优先级 */
    //                         (TaskHandle_t* )&KEY_UP_Task_Handle);/* 任务句柄指针 */
    // if (pdPASS == xReturn)
    // {
    //     printf("Create KEY_UP_Task successfully!\r\n");
    // }
    // else
    // {
    //     printf("Create KEY_UP_Task failed!\r\n");
    // }
    vTaskDelete(TaskCreate_Handle); //删除TaskCreate 任务

    taskEXIT_CRITICAL(); //退出临界区
}

int main()
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
    board_led_init(LED0);
    board_led_init(LED1);
    key_init(KEY_0, KEY_MODE_GPIO);
    key_init(KEY_1, KEY_MODE_GPIO);
    key_init(KEY_UP, KEY_MODE_GPIO);
    board_com_init(115200);

    xReturn = xTaskCreate(  (TaskFunction_t )TaskCreate_Task, /* 任务入口函数 */
                                (const char* )"TaskCreate_Task",/* 任务名字 */
                            (uint16_t )256, /* 任务栈大小 */
                            (void* )NULL, /* 任务入口函数参数 */
                            (UBaseType_t )3, /* 任务的优先级 */
                            (TaskHandle_t* )&TaskCreate_Handle);/* 任务句柄指针 */
    if (pdPASS == xReturn)
        vTaskStartScheduler(); /* 启动任务，开启调度 */

    while (1){}


    return 0;
}
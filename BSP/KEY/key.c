#include "stm32f10x.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

#define SHORT_CLICK     200     // 这个是第一次松开时间和第二次按下时间的判断时长 200ms
#define LONG_PRESS      700     // 定义长按的时间阈值0.7s
#define PRESS           15      // 判断毛刺时长 15ms
#define BUTTON_ERROR    10000   // 按键长久状态卡死阈值10s

extern struct key_init_type** key_cfgs;
extern struct key_task_type** key_tasks;


static void KEY_Task(void* parameter)
{
	struct key_task_type* key = (struct key_task_type *)parameter;

    typedef enum
    {
        IDLE_STATE,   //空闲
        PRESS_DETECTED_STATE,  //按键按下
        RELEASE_DETECTED_STATE, //按键释放
        SHORT_PRESS_STATE,     //短按
        LONG_PRESS_STATE,     //长按
        DOUBLE_PRESS_STATE,    //双击
        LONG_PRESS_STATE_END //长按结束状态
    } ButtonState;

    TickType_t SHORT_CLICK_THRESHOLD = pdMS_TO_TICKS(SHORT_CLICK);
    TickType_t LONG_PRESS_THRESHOLD = pdMS_TO_TICKS(LONG_PRESS);       
    TickType_t PRESS_Time = pdMS_TO_TICKS(PRESS);                  
    TickType_t BUTTON_ERROR_Time = pdMS_TO_TICKS(BUTTON_ERROR);        

    ButtonState buttonState = IDLE_STATE;  //按键状态
    TickType_t pressStartTime = 0;         //记录按下时间
    TickType_t ReleaseTime = 0;


    while (1)
    {
        int keyStatus = key_state_get(key->key_index);  //检测按键
        TickType_t currentTime = xTaskGetTickCount();
        switch (buttonState)
        {
            case IDLE_STATE:
                if (keyStatus == 0)   //如果按键按下
                {
                    buttonState = PRESS_DETECTED_STATE; //切换到按键按下状态
                    pressStartTime = currentTime;       //记录按下时间
                }
                break;

            case PRESS_DETECTED_STATE:    //按键 按下状态
                if (keyStatus == 1)       //检测按键松开时间
                {
                    ReleaseTime = currentTime;  //记录按键放松时间
                    buttonState = RELEASE_DETECTED_STATE;    //如果按下后释放则进入这里
                }
                else if (currentTime - pressStartTime > LONG_PRESS_THRESHOLD) //判断按键长按的时长如果符合则跳转到这里。
                {
                    buttonState = LONG_PRESS_STATE;
                }
                else if(currentTime - pressStartTime > BUTTON_ERROR_Time) //防止按键卡死
                {
                    buttonState = IDLE_STATE;
                }
                break;
            case LONG_PRESS_STATE_END:    //长按结束后的状态跳转这里等待按键松开 防止一直处于长按状态   /
                if (keyStatus == 1)
                {
                    buttonState = IDLE_STATE;
                }
                break;
            case RELEASE_DETECTED_STATE:    //按键释放过后的状态
                if ( (keyStatus == 0) && (currentTime - ReleaseTime < SHORT_CLICK_THRESHOLD))        //如果按键再次按下并且第二次时长是在500ms以内按下的
                {
                    buttonState = DOUBLE_PRESS_STATE;
                }
                else if ( (ReleaseTime - pressStartTime > PRESS_Time) && (currentTime - ReleaseTime > SHORT_CLICK_THRESHOLD)) //判断按键按下时长防止毛刺 在判断当前是否在双击范围内
                {
                    buttonState = SHORT_PRESS_STATE;
                }
                else if (currentTime - ReleaseTime > BUTTON_ERROR_Time) //按键出现无法判断情况回到初态
                {
                    buttonState = IDLE_STATE;
                }
                break;

            case SHORT_PRESS_STATE:
                printf ("%s: 单击\r\n", key->key_name);
                buttonState = LONG_PRESS_STATE_END;
                break;

            case LONG_PRESS_STATE:
                printf ("%s: 长按\r\n", key->key_name);
                buttonState = LONG_PRESS_STATE_END;
                break;

            case DOUBLE_PRESS_STATE:
                buttonState = LONG_PRESS_STATE_END;
                printf ("%s: 双击\r\n", key->key_name);
                break;
            }
            vTaskDelay (8);
    }
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
void key_init(uint8_t key_num, keymode_typedef_enum key_mode)
{
    /* enable the key clock */
    RCC_APB2PeriphClockCmd(key_cfgs[key_num]->key_clk, ENABLE);

    /* configure button pin as input */
    GPIO_Init(key_cfgs[key_num]->gpio_port, &key_cfgs[key_num]->gpio_type);

    if (key_mode == KEY_MODE_EXTI) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
        
        GPIO_EXTILineConfig(key_cfgs[key_num]->key_exit.port_source,key_cfgs[key_num]->key_exit.pin_source);
        
        EXTI_Init(&key_cfgs[key_num]->key_exit.exit_cfg);

        NVIC_Init(&key_cfgs[key_num]->key_nvic);
    }
}

/*!
    \brief      return the selected key state
    \param[in]  key: specify the key to be checked
      \arg        KEY_USER: user key
    \param[out] none
    \retval     the key's GPIO pin value
*/
key_gpio_state key_state_get(uint8_t key_index)
{
    key_gpio_state key_state = GPIO_ReadInputDataBit(key_cfgs[key_index]->gpio_port, key_cfgs[key_index]->gpio_type.GPIO_Pin);
    if(key_cfgs[key_index]->key_valid == LOW_VALID)
        return key_state;
    else
        return !key_state;
}

BaseType_t key_task_init(uint8_t key_index, uint16_t stack_size)
{
    key_tasks[key_index]->key_state = RELEASE_STATE;
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    taskENTER_CRITICAL(); //进入临界区
    xReturn = xTaskCreate(  (TaskFunction_t )KEY_Task, /* 任务入口函数 */
                                (const char* )key_tasks[key_index]->key_name,/* 任务名字 */
                            (uint16_t )stack_size, /* 任务栈大小 */
                            (void* )key_tasks[key_index], /* 任务入口函数参数 */
                            (UBaseType_t )2, /* 任务的优先级 */
                            (TaskHandle_t* )&key_tasks[key_index]->key_handle);/* 任务句柄指针 */
    if (pdPASS == xReturn)
    {
        printf("Create KEY_0_Task successfully!\r\n");
    }
    else
    {
        printf("Create KEY_0_Task failed!\r\n");
    }
    taskEXIT_CRITICAL(); //退出临界区
    return xReturn;
}

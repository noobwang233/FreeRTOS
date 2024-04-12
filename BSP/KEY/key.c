#include "stm32f10x.h"
#include "key.h"
#include "FreeRTOS.h"


static void KEY_Task(void* parameter)
{
	struct key_task_type keyx = (struct key_task_type)parameter;

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

    TickType_t SHORT_CLICK_THRESHOLD = pdMS_TO_TICKS(100);     // 这个是第一次松开时间和第二次按下时间的判断时长 100ms
    TickType_t LONG_PRESS_THRESHOLD = pdMS_TO_TICKS(800);   // 定义长按的时间阈值0.8s
    TickType_t PRESS_Time = pdMS_TO_TICKS(15);               // 判断毛刺时长 15ms
    TickType_t BUTTON_ERROR_Time = pdMS_TO_TICKS(50000);     // 按键长久状态卡死阈值50s

    ButtonState buttonState = IDLE_STATE;  //按键状态
    TickType_t pressStartTime = 0;         //记录按下时间
    TickType_t ReleaseTime = 0;


    while (1)
    {
        int keyStatus = board_key_state_get(keyx.key);  //检测按键
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
                printf ("%s: 单击, 暂停LED %d\r\n", keyx.key_name, key);
                vTaskSuspend(led_handle);
                buttonState = LONG_PRESS_STATE_END;
                break;

            case LONG_PRESS_STATE:
                printf ("%s: 长按, 恢复LED %d\r\n", keyx.key_name, key);
                vTaskResume(led_handle);
                buttonState = LONG_PRESS_STATE_END;
                break;

            case DOUBLE_PRESS_STATE:
                buttonState = LONG_PRESS_STATE_END;
                vTaskResume(led_handle);
                printf ("%s: 双击, 恢复LED %d\r\n", keyx.key_name, key);
                break;
            }
            vTaskDelay (10);
    }
}
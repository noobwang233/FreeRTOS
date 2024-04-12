#include "stm32f10x.h"
#include "key.h"
#include "FreeRTOS.h"


static void KEY_Task(void* parameter)
{
	struct key_task_type keyx = (struct key_task_type)parameter;

    typedef enum
    {
        IDLE_STATE,   //����
        PRESS_DETECTED_STATE,  //��������
        RELEASE_DETECTED_STATE, //�����ͷ�
        SHORT_PRESS_STATE,     //�̰�
        LONG_PRESS_STATE,     //����
        DOUBLE_PRESS_STATE,    //˫��
        LONG_PRESS_STATE_END //��������״̬
    } ButtonState;

    TickType_t SHORT_CLICK_THRESHOLD = pdMS_TO_TICKS(100);     // ����ǵ�һ���ɿ�ʱ��͵ڶ��ΰ���ʱ����ж�ʱ�� 100ms
    TickType_t LONG_PRESS_THRESHOLD = pdMS_TO_TICKS(800);   // ���峤����ʱ����ֵ0.8s
    TickType_t PRESS_Time = pdMS_TO_TICKS(15);               // �ж�ë��ʱ�� 15ms
    TickType_t BUTTON_ERROR_Time = pdMS_TO_TICKS(50000);     // ��������״̬������ֵ50s

    ButtonState buttonState = IDLE_STATE;  //����״̬
    TickType_t pressStartTime = 0;         //��¼����ʱ��
    TickType_t ReleaseTime = 0;


    while (1)
    {
        int keyStatus = board_key_state_get(keyx.key);  //��ⰴ��
        TickType_t currentTime = xTaskGetTickCount();
        switch (buttonState)
        {
            case IDLE_STATE:
                if (keyStatus == 0)   //�����������
                {
                    buttonState = PRESS_DETECTED_STATE; //�л�����������״̬
                    pressStartTime = currentTime;       //��¼����ʱ��
                }
                break;

            case PRESS_DETECTED_STATE:    //���� ����״̬
                if (keyStatus == 1)       //��ⰴ���ɿ�ʱ��
                {
                    ReleaseTime = currentTime;  //��¼��������ʱ��
                    buttonState = RELEASE_DETECTED_STATE;    //������º��ͷ����������
                }
                else if (currentTime - pressStartTime > LONG_PRESS_THRESHOLD) //�жϰ���������ʱ�������������ת�����
                {
                    buttonState = LONG_PRESS_STATE;
                }
                else if(currentTime - pressStartTime > BUTTON_ERROR_Time) //��ֹ��������
                {
                    buttonState = IDLE_STATE;
                }
                break;
            case LONG_PRESS_STATE_END:    //�����������״̬��ת����ȴ������ɿ� ��ֹһֱ���ڳ���״̬   /
                if (keyStatus == 1)
                {
                    buttonState = IDLE_STATE;
                }
                break;
            case RELEASE_DETECTED_STATE:    //�����ͷŹ����״̬
                if ( (keyStatus == 0) && (currentTime - ReleaseTime < SHORT_CLICK_THRESHOLD))        //��������ٴΰ��²��ҵڶ���ʱ������500ms���ڰ��µ�
                {
                    buttonState = DOUBLE_PRESS_STATE;
                }
                else if ( (ReleaseTime - pressStartTime > PRESS_Time) && (currentTime - ReleaseTime > SHORT_CLICK_THRESHOLD)) //�жϰ�������ʱ����ֹë�� ���жϵ�ǰ�Ƿ���˫����Χ��
                {
                    buttonState = SHORT_PRESS_STATE;
                }
                else if (currentTime - ReleaseTime > BUTTON_ERROR_Time) //���������޷��ж�����ص���̬
                {
                    buttonState = IDLE_STATE;
                }
                break;

            case SHORT_PRESS_STATE:
                printf ("%s: ����, ��ͣLED %d\r\n", keyx.key_name, key);
                vTaskSuspend(led_handle);
                buttonState = LONG_PRESS_STATE_END;
                break;

            case LONG_PRESS_STATE:
                printf ("%s: ����, �ָ�LED %d\r\n", keyx.key_name, key);
                vTaskResume(led_handle);
                buttonState = LONG_PRESS_STATE_END;
                break;

            case DOUBLE_PRESS_STATE:
                buttonState = LONG_PRESS_STATE_END;
                vTaskResume(led_handle);
                printf ("%s: ˫��, �ָ�LED %d\r\n", keyx.key_name, key);
                break;
            }
            vTaskDelay (10);
    }
}
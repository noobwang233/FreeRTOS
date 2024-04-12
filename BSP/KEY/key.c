#include "stm32f10x.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

extern struct key_init_type** key_cfgs;
extern struct key_task_type** key_tasks;

static void KEY_Task(void* parameter)
{
	struct key_task_type* key = (struct key_task_type *)parameter;

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
        int keyStatus = key_state_get(key->key_index);  //��ⰴ��
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
                printf ("%s: ����\r\n", key->key_name);
                buttonState = LONG_PRESS_STATE_END;
                break;

            case LONG_PRESS_STATE:
                printf ("%s: ����\r\n", key->key_name);
                buttonState = LONG_PRESS_STATE_END;
                break;

            case DOUBLE_PRESS_STATE:
                buttonState = LONG_PRESS_STATE_END;
                printf ("%s: ˫��\r\n", key->key_name);
                break;
            }
            vTaskDelay (10);
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

void key_task_init(uint8_t key_index)
{
    key_tasks[key_index]->key_state = RELEASE_STATE;
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    taskENTER_CRITICAL(); //�����ٽ���
    xReturn = xTaskCreate(  (TaskFunction_t )KEY_Task, /* ������ں��� */
                                (const char* )key_tasks[key_index]->key_name,/* �������� */
                            (uint16_t )256, /* ����ջ��С */
                            (void* )key_tasks[key_index], /* ������ں������� */
                            (UBaseType_t )2, /* ��������ȼ� */
                            (TaskHandle_t* )&key_tasks[key_index]->key_handle);/* ������ָ�� */
    if (pdPASS == xReturn)
    {
        printf("Create KEY_0_Task successfully!\r\n");
    }
    else
    {
        printf("Create KEY_0_Task failed!\r\n");
    }
}
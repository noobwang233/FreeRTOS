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
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "gd32f303e_eval.h"
/*!
    \brief      toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_spark(void)
{
    static  uint32_t timingdelaylocal = 0U;

    if(timingdelaylocal){

        if(timingdelaylocal < 500U){
            gd_eval_led_on(LED2);
        }else{
            gd_eval_led_off(LED2);
        }

        timingdelaylocal--;
    }else{
        timingdelaylocal = 1000U;
    }
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
    /* configure systick */
    systick_config();
    /* initilize the LEDs, USART and key */
    gd_eval_led_init(LED2); 
    gd_eval_led_init(LED3); 
    gd_eval_led_init(LED4);
    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);

    while (1){
        if(RESET == gd_eval_key_state_get(KEY_WAKEUP)){
            gd_eval_led_on(LED3);
            delay_1ms(500);
            gd_eval_led_off(LED3);
            gd_eval_led_toggle(LED4);
        }
    }
}

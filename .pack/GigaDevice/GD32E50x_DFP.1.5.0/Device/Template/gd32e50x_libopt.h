/*!
    \file    gd32e50x_libopt.h
    \brief   library optional for gd32e50x

    \version 2020-03-10, V1.0.0, firmware for GD32E50x
    \version 2020-08-26, V1.1.0, firmware for GD32E50x
    \version 2021-03-23, V1.2.0, firmware for GD32E50x
*/

/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

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

#ifndef GD32E50X_LIBOPT_H
#define GD32E50X_LIBOPT_H
/* Comment the line below to disable peripheral header file inclusion */
#include "RTE_Components.h"

#ifndef GD32EPRT

#ifdef RTE_DEVICE_STDPERIPHERALS_ADC
#include "gd32e50x_adc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_BKP
#include "gd32e50x_bkp.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_CAN
#include "gd32e50x_can.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_CRC
#include "gd32e50x_crc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_CTC
#include "gd32e50x_ctc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_DAC
#include "gd32e50x_dac.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_DBG
#include "gd32e50x_dbg.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_DMA
#include "gd32e50x_dma.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_EXMC
#include "gd32e50x_exmc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_EXTI
#include "gd32e50x_exti.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_FMC
#include "gd32e50x_fmc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_FWDGT
#include "gd32e50x_fwdgt.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_GPIO
#include "gd32e50x_gpio.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_SHRTIMER
#include "gd32e50x_shrtimer.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_I2C
#include "gd32e50x_i2c.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_MISC
#include "gd32e50x_misc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_PMU
#include "gd32e50x_pmu.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_RCU
#include "gd32e50x_rcu.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_RTC
#include "gd32e50x_rtc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_SPI
#include "gd32e50x_spi.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_TIMER
#include "gd32e50x_timer.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_USART
#include "gd32e50x_usart.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_WWDGT
#include "gd32e50x_wwdgt.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_SQPI
#include "gd32e50x_sqpi.h"
#endif


#if defined (GD32E50X_CL) || defined (GD32E508)
#ifdef RTE_DEVICE_STDPERIPHERALS_ENET
#include "gd32e50x_enet.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_TMU
#include "gd32e50x_tmu.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_CMP
#include "gd32e50x_cmp.h"
#endif

#else /* GD32E50X_CL or GD32E508 */

#ifdef RTE_DEVICE_STDPERIPHERALS_SDIO
#include "gd32e50x_sdio.h"
#endif

#endif /* GD32E50X_CL or GD32E508 */

#else /* GD32EPRT */
#ifdef RTE_DEVICE_STDPERIPHERALS_ADC
#include "gd32e50x_adc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_BKP
#include "gd32e50x_bkp.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_CAN
#include "gd32e50x_can.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_CRC
#include "gd32e50x_crc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_CTC
#include "gd32e50x_ctc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_DAC
#include "gd32e50x_dac.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_DBG
#include "gd32e50x_dbg.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_DMA
#include "gd32e50x_dma.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_EXMC
#include "gd32e50x_exmc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_EXTI
#include "gd32e50x_exti.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_FMC
#include "gd32e50x_fmc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_FWDGT
#include "gd32e50x_fwdgt.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_GPIO
#include "gd32e50x_gpio.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_SHRTIMER
#include "gd32e50x_shrtimer.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_I2C
#include "gd32e50x_i2c.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_MISC
#include "gd32e50x_misc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_PMU
#include "gd32e50x_pmu.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_RCU
#include "gd32e50x_rcu.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_RTC
#include "gd32e50x_rtc.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_SPI
#include "gd32e50x_spi.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_TIMER
#include "gd32e50x_timer.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_USART
#include "gd32e50x_usart.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_WWDGT
#include "gd32e50x_wwdgt.h"
#endif

#ifdef RTE_DEVICE_STDPERIPHERALS_SQPI
#include "gd32e50x_sqpi.h"
#endif

#endif /* GD32EPRT */

#endif /* GD32E50X_LIBOPT_H */

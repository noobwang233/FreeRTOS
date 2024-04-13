#ifndef __WDG_H
#define __WDG_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//看门狗 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/5/30
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


void IWDG_Init(uint8_t prer,uint16_t rlr);
void IWDG_Feed(void);

void WWDG_Init(uint8_t tr,uint8_t wr,uint32_t fprer);//初始化WWDG
void WWDG_Set_Counter(uint8_t cnt);       //设置WWDG的计数器
void WWDG_NVIC_Init(void);
#endif

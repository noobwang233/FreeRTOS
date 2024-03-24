#include "stm32f10x.h"
#include "stm32f10x_conf.h"
/************************************************
 ALIENTEK 精英STM32F103开发板实验0
 工程模板
 注意，这是手册中的新建工程章节使用的main文件 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


 void Delay(u32 count)
 {
   u32 i=0;
   for(;i<count;i++);
 }
 int main(void)
 {	
  GPIO_InitTypeDef  GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	    //使能PB,PE端口时钟
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	            //LED1-->PE.5推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);	  	       //初始化GPIO
  GPIO_ResetBits(GPIOC,GPIO_Pin_13); 			 //PE.5 输出高 	  
  while(1)
	{
	  GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay(3000000);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay(3000000);
	}
 }

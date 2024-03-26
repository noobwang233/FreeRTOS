  .syntax unified   /* 意思是下面的指令是ARM和THUMB通用格式的 */
  .cpu cortex-m4    /* CPU内核型号 */
  .fpu softvfp      /* 选择使用的浮点单元，与-mfpu命令行选项的相同 */
  .thumb            /* 选择使用thumb指令 */

/* 声明可以被汇编器使用的符号 */
.global g_pfnVectors    /* 在文件末尾定义的中断向量 */
.global Default_Handler /* 是一个死循环，用来处理异常情况 */

/* start address of the static initialization data */
.word  _sidata
/* start address of the data section */
/* .data对应初始化了的全局变量，编译后将位于可执行文件中，
    由启动代码负责加载到数据区中（在单片机中这部分数据会存于flash中，需要由启动代码把这部分内容拷贝到ram中）
*/
.word  _sdata
/* end address of the data section */
.word  _edata
/* start address of the bss section */
/*
  .bss段是没有初始值的全局变量，由启动代码把这 部分内容全初始化为0
*/
.word  _sbss
/* end address of the bss section */
.word  _ebss

/* 定义Reset_Handler函数，该函数的作用1.设置堆栈指针；2.全局变量的初始化 */
.section  .text.Reset_Handler
.weak  Reset_Handler
.type  Reset_Handler, %function 
/*这行指令设置了 Reset_Handler 符号的类型为函数（function）。这有助于编译器和链接器识别 Reset_Handler 符号是一个函数，而不是其他类型的符号。*/

Reset_Handler:
/* copy the data segment into ram */  
  movs r1, #0  // 将r1赋值为0
  b DataInit   // 跳转到DataInit

CopyData:
  ldr r3, =_sidata // 将初始化数据地址加载到r3中
  ldr r3, [r3, r1] // 寄存器间接寻址，将r3 + r1地址中的数据加载到r3中 
  str r3, [r0, r1] // 将r3的内容写入 r0 + r1地址中
  adds r1, r1, #4  // r1 + 4, 然后将结果写入r1，

DataInit:
  ldr r0, =_sdata  // r0为需要初始化的全局变量起始地址
  ldr r3, =_edata  // r3为需要初始化的全局变量结束地址
  adds r2, r0, r1  // 将r0 + r1 相加，保存到r2 同时检查溢出标志
  cmp r2, r3       // 比较r2 和 r3，如果r2小于r3继续拷贝数据
  bcc CopyData     // 如果r2小于r3继续拷贝数据
  ldr r2, =_sbss   // 将r2赋值为没有初始值的全局变量起始地址即bss区首地址
  b Zerobss        // 跳转到Zerobss
/* zero the bss segment */ 
FillZerobss:
  movs r3, #0
  str r3, [r2], #4 //采用后变址，先将r3中的值写入r2，也就是bss数据区首地址中，然后r2自加4

Zerobss:
  ldr r3, = _ebss
  cmp r2, r3
  bcc FillZerobss
/* configure the clock */
  bl  SystemInit
/* start execution of the program */
  bl main
  bx lr
.size Reset_Handler, .-Reset_Handler
/* 设置了 Reset_Handler 符号的大小。
.size 指令的语法是 .size symbol, expression，它用于指定符号的大小。
在这里，. 表示当前地址，.-Reset_Handler 表示当前地址与 Reset_Handler 符号地址的差值，即 Reset_Handler 符号的大小。 */


/* "ax": 这是节的标志（flags）。在这里，a 表示该节是可分配的（allocatable），x 表示该节是可执行的（executable）。这意味着该节中的代码可以被加载到内存并执行。*/
/* %progbits: 这是节的属性（attributes）。%progbits 属性表示该节包含了程序数据。这意味着该节中的内容是实际的程序代码或数据。*/
.section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler

.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
/* %object：表示该符号是一个对象（object），在汇编语言中一般用于数据或变量。对象类型的符号在链接时会被保留，并且它们的地址是可用的。*/
.size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
                    /* 上电之后MCU 会自动将0地址的数据加载到MSP */
                    .word _estack                    /*在当前位置放置一个word型的值，这个值为_estack；已下同理  */
                    .word Reset_Handler              /* Vector Number 1,Reset Handler */
                    .word NMI_Handler                /* Vector Number 2,NMI Handler */
                    .word HardFault_Handler          /* Vector Number 3,Hard Fault Handler */
                    .word MemManage_Handler          /* Vector Number 4,MPU Fault Handler */
                    .word BusFault_Handler           /* Vector Number 5,Bus Fault Handler */
                    .word UsageFault_Handler         /* Vector Number 6,Usage Fault Handler */
                    .word 0                          /* Reserved  */
                    .word 0                          /* Reserved */
                    .word 0                          /* Reserved */
                    .word 0                          /* Reserved */
                    .word SVC_Handler                /* Vector Number 11,SVCall Handler */
                    .word DebugMon_Handler           /* Vector Number 12,Debug Monitor Handler */
                    .word 0                          /* Reserved */
                    .word PendSV_Handler             /* Vector Number 14,PendSV Handler */
                    .word SysTick_Handler            /* Vector Number 15,SysTick Handler */

                    /* External Interrupts */
                    .word WWDGT_IRQHandler               /* Vector Number 16,Window Watchdog Timer */
                    .word LVD_IRQHandler                 /* Vector Number 17,LVD through EXTI Line detect */
                    .word TAMPER_IRQHandler              /* Vector Number 18,Tamper Interrupt */
                    .word RTC_IRQHandler                 /* Vector Number 19,RTC through EXTI Line */
                    .word FMC_IRQHandler                 /* Vector Number 20,FMC */
                    .word RCU_IRQHandler                 /* Vector Number 21,RCU */
                    .word EXTI0_IRQHandler               /* Vector Number 22,EXTI Line 0 */
                    .word EXTI1_IRQHandler               /* Vector Number 23,EXTI Line 1 */
                    .word EXTI2_IRQHandler               /* Vector Number 24,EXTI Line 2 */
                    .word EXTI3_IRQHandler               /* Vector Number 25,EXTI Line 3 */
                    .word EXTI4_IRQHandler               /* Vector Number 26,EXTI Line 4 */
                    .word DMA0_Channel0_IRQHandler       /* Vector Number 27,DMA0 Channel 0 */
                    .word DMA0_Channel1_IRQHandler       /* Vector Number 28,DMA0 Channel 1 */
                    .word DMA0_Channel2_IRQHandler       /* Vector Number 29,DMA0 Channel 2 */
                    .word DMA0_Channel3_IRQHandler       /* Vector Number 30,DMA0 Channel 3 */
                    .word DMA0_Channel4_IRQHandler       /* Vector Number 31,DMA0 Channel 4 */
                    .word DMA0_Channel5_IRQHandler       /* Vector Number 32,DMA0 Channel 5 */
                    .word DMA0_Channel6_IRQHandler       /* Vector Number 33,DMA0 Channel 6 */
                    .word ADC0_1_IRQHandler              /* Vector Number 34,ADC0 and ADC1  */
                    .word USBD_HP_CAN0_TX_IRQHandler     /* Vector Number 35,USBD and CAN0 TX  */
                    .word USBD_LP_CAN0_RX0_IRQHandler    /* Vector Number 36,USBD and CAN0 RX0 */
                    .word CAN0_RX1_IRQHandler            /* Vector Number 37,CAN0 RX1  */
                    .word CAN0_EWMC_IRQHandler           /* Vector Number 38,CAN0 EWMC  */
                    .word EXTI5_9_IRQHandler             /* Vector Number 39,EXTI Line 5..9  */
                    .word TIMER0_BRK_IRQHandler          /* Vector Number 40,TIMER0 Break  */
                    .word TIMER0_UP_IRQHandler           /* Vector Number 41,TIMER0 Update  */
                    .word TIMER0_TRG_CMT_IRQHandler      /* Vector Number 42,TIMER0 Trigger and Commutation  */
                    .word TIMER0_Channel_IRQHandler      /* Vector Number 43,TIMER0 Channel Capture Compare */
                    .word TIMER1_IRQHandler              /* Vector Number 44,TIMER1 */
                    .word TIMER2_IRQHandler              /* Vector Number 45,TIMER2 */
                    .word TIMER3_IRQHandler              /* Vector Number 46,TIMER3 */
                    .word I2C0_EV_IRQHandler             /* Vector Number 47,I2C0 Event */
                    .word I2C0_ER_IRQHandler             /* Vector Number 48,I2C0 Error */
                    .word I2C1_EV_IRQHandler             /* Vector Number 49,I2C1 Event */
                    .word I2C1_ER_IRQHandler             /* Vector Number 50,I2C1 Error */
                    .word SPI0_IRQHandler                /* Vector Number 51,SPI0 */
                    .word SPI1_IRQHandler                /* Vector Number 52,SPI1 */
                    .word USART0_IRQHandler              /* Vector Number 53,USART0 */
                    .word USART1_IRQHandler              /* Vector Number 54,USART1 */
                    .word USART2_IRQHandler              /* Vector Number 55,USART2 */
                    .word EXTI10_15_IRQHandler           /* Vector Number 56,EXTI Line 10 to EXTI Line 15 */
                    .word RTC_Alarm_IRQHandler           /* Vector Number 57,RTC Alarm through EXTI Line */
                    .word USBD_WKUP_IRQHandler           /* Vector Number 58,USBD WakeUp from suspend through EXTI Line */
                    .word TIMER7_BRK_IRQHandler          /* Vector Number 59,TIMER7 Break Interrupt */
                    .word TIMER7_UP_IRQHandler           /* Vector Number 60,TIMER7 Update Interrupt */
                    .word TIMER7_TRG_CMT_IRQHandler      /* Vector Number 61,TIMER7 Trigger and Commutation Interrupt */
                    .word TIMER7_Channel_IRQHandler      /* Vector Number 62,TIMER7 Channel Capture Compare */ 
                    .word ADC2_IRQHandler                /* Vector Number 63,ADC2  */
                    .word EXMC_IRQHandler                /* Vector Number 64,EXMC */
                    .word SDIO_IRQHandler                /* Vector Number 65,SDIO */
                    .word TIMER4_IRQHandler              /* Vector Number 66,TIMER4 */
                    .word SPI2_IRQHandler                /* Vector Number 67,SPI2 */
                    .word UART3_IRQHandler               /* Vector Number 68,UART3 */
                    .word UART4_IRQHandler               /* Vector Number 69,UART4 */
                    .word TIMER5_IRQHandler              /* Vector Number 70,TIMER5 */
                    .word TIMER6_IRQHandler              /* Vector Number 71,TIMER6 */
                    .word DMA1_Channel0_IRQHandler       /* Vector Number 72,DMA1 Channel0 */
                    .word DMA1_Channel1_IRQHandler       /* Vector Number 73,DMA1 Channel1 */
                    .word DMA1_Channel2_IRQHandler       /* Vector Number 74,DMA1 Channel2 */
                    .word DMA1_Channel3_4_IRQHandler     /* Vector Number 75,DMA1 Channel3 and Channel4 */

  .weak NMI_Handler   /*弱定义一个符号，名字叫 NMI_Handler*/
  .thumb_set NMI_Handler,Default_Handler /*如果没有重写这个弱定义的符号，则执行 Default_Handler，反之则执行重写的NMI_Handler，已下同理 */

  .weak HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak MemManage_Handler
  .thumb_set MemManage_Handler,Default_Handler

  .weak BusFault_Handler
  .thumb_set BusFault_Handler,Default_Handler

  .weak UsageFault_Handler
  .thumb_set UsageFault_Handler,Default_Handler

  .weak SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak DebugMon_Handler
  .thumb_set DebugMon_Handler,Default_Handler

  .weak PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak WWDGT_IRQHandler
  .thumb_set WWDGT_IRQHandler,Default_Handler

  .weak LVD_IRQHandler
  .thumb_set LVD_IRQHandler,Default_Handler

  .weak TAMPER_IRQHandler
  .thumb_set TAMPER_IRQHandler,Default_Handler

  .weak RTC_IRQHandler
  .thumb_set RTC_IRQHandler,Default_Handler

  .weak FMC_IRQHandler
  .thumb_set FMC_IRQHandler,Default_Handler

  .weak RCU_IRQHandler
  .thumb_set RCU_IRQHandler,Default_Handler

  .weak EXTI0_IRQHandler
  .thumb_set EXTI0_IRQHandler,Default_Handler

  .weak EXTI1_IRQHandler
  .thumb_set EXTI1_IRQHandler,Default_Handler

  .weak EXTI2_IRQHandler
  .thumb_set EXTI2_IRQHandler,Default_Handler

  .weak EXTI3_IRQHandler
  .thumb_set EXTI3_IRQHandler,Default_Handler

  .weak EXTI4_IRQHandler
  .thumb_set EXTI4_IRQHandler,Default_Handler

  .weak DMA0_Channel0_IRQHandler
  .thumb_set DMA0_Channel0_IRQHandler,Default_Handler

  .weak DMA0_Channel1_IRQHandler
  .thumb_set DMA0_Channel1_IRQHandler,Default_Handler

  .weak DMA0_Channel2_IRQHandler
  .thumb_set DMA0_Channel2_IRQHandler,Default_Handler

  .weak DMA0_Channel3_IRQHandler
  .thumb_set DMA0_Channel3_IRQHandler,Default_Handler

  .weak DMA0_Channel4_IRQHandler
  .thumb_set DMA0_Channel4_IRQHandler,Default_Handler

  .weak DMA0_Channel5_IRQHandler
  .thumb_set DMA0_Channel5_IRQHandler,Default_Handler

  .weak DMA0_Channel6_IRQHandler
  .thumb_set DMA0_Channel6_IRQHandler,Default_Handler

  .weak ADC0_1_IRQHandler
  .thumb_set ADC0_1_IRQHandler,Default_Handler

  .weak USBD_HP_CAN0_TX_IRQHandler
  .thumb_set USBD_HP_CAN0_TX_IRQHandler,Default_Handler

  .weak USBD_LP_CAN0_RX0_IRQHandler
  .thumb_set USBD_LP_CAN0_RX0_IRQHandler,Default_Handler

  .weak CAN0_RX1_IRQHandler
  .thumb_set CAN0_RX1_IRQHandler,Default_Handler

  .weak CAN0_EWMC_IRQHandler
  .thumb_set CAN0_EWMC_IRQHandler,Default_Handler

  .weak EXTI5_9_IRQHandler
  .thumb_set EXTI5_9_IRQHandler,Default_Handler

  .weak TIMER0_BRK_IRQHandler
  .thumb_set TIMER0_BRK_IRQHandler,Default_Handler

  .weak TIMER0_UP_IRQHandler
  .thumb_set TIMER0_UP_IRQHandler,Default_Handler

  .weak TIMER0_TRG_CMT_IRQHandler
  .thumb_set TIMER0_TRG_CMT_IRQHandler,Default_Handler

  .weak TIMER0_Channel_IRQHandler
  .thumb_set TIMER0_Channel_IRQHandler,Default_Handler

  .weak TIMER1_IRQHandler
  .thumb_set TIMER1_IRQHandler,Default_Handler

  .weak TIMER2_IRQHandler
  .thumb_set TIMER2_IRQHandler,Default_Handler

  .weak TIMER3_IRQHandler
  .thumb_set TIMER3_IRQHandler,Default_Handler

  .weak I2C0_EV_IRQHandler
  .thumb_set I2C0_EV_IRQHandler,Default_Handler

  .weak I2C0_ER_IRQHandler
  .thumb_set I2C0_ER_IRQHandler,Default_Handler

  .weak I2C1_EV_IRQHandler
  .thumb_set I2C1_EV_IRQHandler,Default_Handler

  .weak I2C1_ER_IRQHandler
  .thumb_set I2C1_ER_IRQHandler,Default_Handler

  .weak SPI0_IRQHandler
  .thumb_set SPI0_IRQHandler,Default_Handler

  .weak SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Default_Handler

  .weak USART0_IRQHandler
  .thumb_set USART0_IRQHandler,Default_Handler

  .weak USART1_IRQHandler
  .thumb_set USART1_IRQHandler,Default_Handler

  .weak USART2_IRQHandler
  .thumb_set USART2_IRQHandler,Default_Handler

  .weak EXTI10_15_IRQHandler
  .thumb_set EXTI10_15_IRQHandler,Default_Handler

  .weak RTC_Alarm_IRQHandler
  .thumb_set RTC_Alarm_IRQHandler,Default_Handler

  .weak USBD_WKUP_IRQHandler 
  .thumb_set USBD_WKUP_IRQHandler,Default_Handler

  .weak TIMER7_BRK_IRQHandler
  .thumb_set TIMER7_BRK_IRQHandler,Default_Handler

  .weak TIMER7_UP_IRQHandler
  .thumb_set TIMER7_UP_IRQHandler,Default_Handler

  .weak TIMER7_TRG_CMT_IRQHandler
  .thumb_set TIMER7_TRG_CMT_IRQHandler,Default_Handler

  .weak TIMER7_Channel_IRQHandler
  .thumb_set TIMER7_Channel_,Default_Handler

  .weak ADC2_IRQHandler
  .thumb_set ADC2_IRQHandler,Default_Handler

  .weak EXMC_IRQHandler
  .thumb_set EXMC_IRQHandler,Default_Handler

  .weak SDIO_IRQHandler
  .thumb_set SDIO_IRQHandler,Default_Handler

  .weak TIMER4_IRQHandler
  .thumb_set TIMER4_IRQHandler,Default_Handler

  .weak SPI2_IRQHandler
  .thumb_set SPI2_IRQHandler,Default_Handler

  .weak UART3_IRQHandler
  .thumb_set UART3_IRQHandler,Default_Handler

  .weak UART4_IRQHandler
  .thumb_set UART4_IRQHandler,Default_Handler

  .weak TIMER5_IRQHandler
  .thumb_set TIMER5_IRQHandler,Default_Handler

  .weak TIMER6_IRQHandler
  .thumb_set TIMER6_IRQHandler,Default_Handler

  .weak DMA1_Channel0_IRQHandler
  .thumb_set DMA1_Channel0_IRQHandler,Default_Handler

  .weak DMA1_Channel1_IRQHandler
  .thumb_set DMA1_Channel1_IRQHandler,Default_Handler

  .weak DMA1_Channel2_IRQHandler
  .thumb_set DMA1_Channel2_IRQHandler,Default_Handler

  .weak DMA1_Channel3_4_IRQHandler
  .thumb_set DMA1_Channel3_4_IRQHandler,Default_Handler

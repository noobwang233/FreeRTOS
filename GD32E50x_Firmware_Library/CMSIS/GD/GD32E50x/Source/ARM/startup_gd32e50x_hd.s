  .syntax unified   /* 意思是下面的指令是ARM和THUMB通用格式的 */
  .cpu cortex-m4    /* CPU内核型号 */
  .fpu fpv4-sp-d16  /* 选择使用的浮点单元，与-mfpu命令行选项的相同 */
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
                .long    _estack                       /*     Initial Stack Pointer */
                .long    Reset_Handler                      /*     Reset Handler */
                .long    NMI_Handler                        /* -14 NMI Handler */
                .long    HardFault_Handler                  /* -13 Hard Fault Handler */
                .long    MemManage_Handler                  /* -12 MPU Fault Handler */
                .long    BusFault_Handler                   /* -11 Bus Fault Handler */
                .long    UsageFault_Handler                 /* -10 Usage Fault Handler */
                .long    SecureFault_Handler                /*  -9 Secure Fault Handler */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    SVC_Handler                        /*  -5 SVCall Handler */
                .long    DebugMon_Handler                   /*  -4 Debug Monitor Handler */
                .long    0                                  /*     Reserved */
                .long    PendSV_Handler                     /*  -2 PendSV Handler */
                .long    SysTick_Handler                    /*  -1 SysTick Handler */

                /* Interrupts */
                .long     WWDGT_IRQHandler                  // 16:Window Watchdog Timer
                .long     LVD_IRQHandler                    // 17:LVD through EXTI Line detect
                .long     TAMPER_IRQHandler                 // 18:Tamper through EXTI Line detect
                .long     RTC_IRQHandler                    // 19:RTC through EXTI Line
                .long     FMC_IRQHandler                    // 20:FMC
                .long     RCU_CTC_IRQHandler                // 21:RCU and CTC
                .long     EXTI0_IRQHandler                  // 22:EXTI Line 0
                .long     EXTI1_IRQHandler                  // 23:EXTI Line 1
                .long     EXTI2_IRQHandler                  // 24:EXTI Line 2
                .long     EXTI3_IRQHandler                  // 25:EXTI Line 3
                .long     EXTI4_IRQHandler                  // 26:EXTI Line 4
                .long     DMA0_Channel0_IRQHandler          // 27:DMA0 Channel0
                .long     DMA0_Channel1_IRQHandler          // 28:DMA0 Channel1
                .long     DMA0_Channel2_IRQHandler          // 29:DMA0 Channel2
                .long     DMA0_Channel3_IRQHandler          // 30:DMA0 Channel3
                .long     DMA0_Channel4_IRQHandler          // 31:DMA0 Channel4
                .long     DMA0_Channel5_IRQHandler          // 32:DMA0 Channel5
                .long     DMA0_Channel6_IRQHandler          // 33:DMA0 Channel6
                .long     ADC0_1_IRQHandler                 // 34:ADC0 and ADC1
                .long     USBD_HP_CAN0_TX_IRQHandler        // 35:USBD HP or CAN0 TX
                .long     USBD_LP_CAN0_RX0_IRQHandler       // 36:USBD LP or CAN0 RX0
                .long     CAN0_RX1_IRQHandler               // 37:CAN0 RX1
                .long     CAN0_EWMC_IRQHandler              // 38:CAN0 EWMC
                .long     EXTI5_9_IRQHandler                // 39:EXTI5 to EXTI9
                .long     TIMER0_BRK_TIMER8_IRQHandler      // 40:TIMER0 Break and TIMER8
                .long     TIMER0_UP_TIMER9_IRQHandler       // 41:TIMER0 Update and TIMER9
                .long     TIMER0_TRG_CMT_TIMER10_IRQHandler // 42:TIMER0 Trigger and Commutation and TIMER10
                .long     TIMER0_Channel_IRQHandler         // 43:TIMER0 Channel Capture Compare
                .long     TIMER1_IRQHandler                 // 44:TIMER1
                .long     TIMER2_IRQHandler                 // 45:TIMER2
                .long     TIMER3_IRQHandler                 // 46:TIMER3
                .long     I2C0_EV_IRQHandler                // 47:I2C0 Event
                .long     I2C0_ER_IRQHandler                // 48:I2C0 Error
                .long     I2C1_EV_IRQHandler                // 49:I2C1 Event
                .long     I2C1_ER_IRQHandler                // 50:I2C1 Error
                .long     SPI0_IRQHandler                   // 51:SPI0
                .long     SPI1_I2S1ADD_IRQHandler           // 52:SPI1 or I2S1ADD
                .long     USART0_IRQHandler                 // 53:USART0
                .long     USART1_IRQHandler                 // 54:USART1
                .long     USART2_IRQHandler                 // 55:USART2
                .long     EXTI10_15_IRQHandler              // 56:EXTI10 to EXTI15
                .long     RTC_Alarm_IRQHandler              // 57:RTC Alarm
                .long     USBD_WKUP_IRQHandler              // 58:USBD Wakeup
                .long     TIMER7_BRK_TIMER11_IRQHandler     // 59:TIMER7 Break and TIMER11
                .long     TIMER7_UP_TIMER12_IRQHandler      // 60:TIMER7 Update and TIMER12
                .long     TIMER7_TRG_CMT_TIMER13_IRQHandler // 61:TIMER7 Trigger and Commutation and TIMER13
                .long     TIMER7_Channel_IRQHandler         // 62:TIMER7 Channel Capture Compare
                .long     ADC2_IRQHandler                   // 63:ADC2
                .long     EXMC_IRQHandler                   // 64:EXMC
                .long     SDIO_IRQHandler                   // 65:SDIO
                .long     TIMER4_IRQHandler                 // 66:TIMER4
                .long     SPI2_I2S2ADD_IRQHandler           // 67:SPI2 or I2S2ADD
                .long     UART3_IRQHandler                  // 68:UART3
                .long     UART4_IRQHandler                  // 69:UART4
                .long     TIMER5_DAC_IRQHandler             // 70:TIMER5 or DAC
                .long     TIMER6_IRQHandler                 // 71:TIMER6
                .long     DMA1_Channel0_IRQHandler          // 72:DMA1 Channel0
                .long     DMA1_Channel1_IRQHandler          // 73:DMA1 Channel1
                .long     DMA1_Channel2_IRQHandler          // 74:DMA1 Channel2
                .long     DMA1_Channel3_4_IRQHandler        // 75:DMA1 Channel3 and Channel4
                .long     0                                 // Reserved
                .long     0                                 // Reserved
                .long     0                                 // Reserved
                .long     CAN1_TX_IRQHandler                // 79:CAN1 TX
                .long     CAN1_RX0_IRQHandler               // 80:CAN1 RX0
                .long     CAN1_RX1_IRQHandler               // 81:CAN1 RX1
                .long     CAN1_EWMC_IRQHandler              // 82:CAN1 EWMC
                .long     0                                 // Reserved
                .long     0                                 // Reserved
                .long     SHRTIMER_IRQ2_IRQHandler          // 85:SHRTIMER IRQ2
                .long     SHRTIMER_IRQ3_IRQHandler          // 86:SHRTIMER IRQ3
                .long     SHRTIMER_IRQ4_IRQHandler          // 87:SHRTIMER IRQ4
                .long     SHRTIMER_IRQ5_IRQHandler          // 88:SHRTIMER IRQ5
                .long     SHRTIMER_IRQ6_IRQHandler          // 89:SHRTIMER IRQ6
                .long     0                                 // Reserved
                .long     0                                 // Reserved
                .long     SHRTIMER_IRQ0_IRQHandler          // 92:SHRTIMER IRQ0
                .long     SHRTIMER_IRQ1_IRQHandler          // 93:SHRTIMER IRQ1
                .long     0                                 // Reserved
                .long     0                                 // Reserved
                .long     0                                 // Reserved
                .long     0                                 // Reserved
                .long     I2C2_EV_IRQHandler                // 98:I2C2 Event
                .long     I2C2_ER_IRQHandler                // 99:I2C2 Error
                .long     USART5_IRQHandler                 // 100:USART5
                .long     I2C2_WKUP_IRQHandler              // 101:I2C2 Wakeup
                .long     USART5_WKUP_IRQHandler            // 102:USART5 Wakeup
                .long     0                                 // Reserved


.weak NMI_Handler   /*弱定义一个符号，名字叫 NMI_Handler*/
.thumb_set NMI_Handler,Default_Handler /*如果没有重写这个弱定义的符号，则执行 Default_Handler，反之则执行重写的NMI_Handler，已下同理 */
.weak    HardFault_Handler                  /* -13 Hard Fault Handler */
.thumb_set HardFault_Handler,Default_Handler
.weak    MemManage_Handler                  /* -12 MPU Fault Handler */
.thumb_set MemManage_Handler,Default_Handler
.weak    BusFault_Handler                   /* -11 Bus Fault Handler */
.thumb_set BusFault_Handler,Default_Handler
.weak    UsageFault_Handler                 /* -10 Usage Fault Handler */
.thumb_set UsageFault_Handler,Default_Handler
.weak    SecureFault_Handler                /*  -9 Secure Fault Handler */
.thumb_set SecureFault_Handler,Default_Handler
.weak    SVC_Handler                        /*  -5 SVCall Handler */
.thumb_set SVC_Handler,Default_Handler
.weak    DebugMon_Handler                   /*  -4 Debug Monitor Handler */
.thumb_set DebugMon_Handler,Default_Handler
.weak    PendSV_Handler                     /*  -2 PendSV Handler */
.thumb_set PendSV_Handler,Default_Handler
.weak    SysTick_Handler                    /*  -1 SysTick Handler */
.thumb_set SysTick_Handler,Default_Handler

/* Interrupts */
.weak     WWDGT_IRQHandler                  // 16:Window Watchdog Timer
.thumb_set WWDGT_IRQHandler,Default_Handler
.weak     LVD_IRQHandler                    // 17:LVD through EXTI Line detect
.thumb_set LVD_IRQHandler,Default_Handler
.weak     TAMPER_IRQHandler                 // 18:Tamper through EXTI Line detect
.thumb_set TAMPER_IRQHandler,Default_Handler
.weak     RTC_IRQHandler                    // 19:RTC through EXTI Line
.thumb_set RTC_IRQHandler,Default_Handler
.weak     FMC_IRQHandler                    // 20:FMC
.thumb_set FMC_IRQHandler,Default_Handler
.weak     RCU_CTC_IRQHandler                // 21:RCU and CTC
.thumb_set RCU_CTC_IRQHandler,Default_Handler
.weak     EXTI0_IRQHandler                  // 22:EXTI Line 0
.thumb_set EXTI0_IRQHandler,Default_Handler
.weak     EXTI1_IRQHandler                  // 23:EXTI Line 1
.thumb_set EXTI1_IRQHandler,Default_Handler
.weak     EXTI2_IRQHandler                  // 24:EXTI Line 2
.thumb_set EXTI2_IRQHandler,Default_Handler
.weak     EXTI3_IRQHandler                  // 25:EXTI Line 3
.thumb_set EXTI3_IRQHandler,Default_Handler
.weak     EXTI4_IRQHandler                  // 26:EXTI Line 4
.thumb_set EXTI4_IRQHandler,Default_Handler
.weak     DMA0_Channel0_IRQHandler          // 27:DMA0 Channel0
.thumb_set DMA0_Channel0_IRQHandler,Default_Handler
.weak     DMA0_Channel1_IRQHandler          // 28:DMA0 Channel1
.thumb_set DMA0_Channel1_IRQHandler,Default_Handler
.weak     DMA0_Channel2_IRQHandler          // 29:DMA0 Channel2
.thumb_set DMA0_Channel2_IRQHandler,Default_Handler
.weak     DMA0_Channel3_IRQHandler          // 30:DMA0 Channel3
.thumb_set DMA0_Channel3_IRQHandler,Default_Handler
.weak     DMA0_Channel4_IRQHandler          // 31:DMA0 Channel4
.thumb_set DMA0_Channel4_IRQHandler,Default_Handler
.weak     DMA0_Channel5_IRQHandler          // 32:DMA0 Channel5
.thumb_set DMA0_Channel5_IRQHandler,Default_Handler
.weak     DMA0_Channel6_IRQHandler          // 33:DMA0 Channel6
.thumb_set DMA0_Channel6_IRQHandler,Default_Handler
.weak     ADC0_1_IRQHandler                 // 34:ADC0 and ADC1
.thumb_set ADC0_1_IRQHandler,Default_Handler
.weak     USBD_HP_CAN0_TX_IRQHandler        // 35:USBD HP or CAN0 TX
.thumb_set USBD_HP_CAN0_TX_IRQHandler,Default_Handler
.weak     USBD_LP_CAN0_RX0_IRQHandler       // 36:USBD LP or CAN0 RX0
.thumb_set USBD_LP_CAN0_RX0_IRQHandler,Default_Handler
.weak     CAN0_RX1_IRQHandler               // 37:CAN0 RX1
.thumb_set CAN0_RX1_IRQHandler,Default_Handler
.weak     CAN0_EWMC_IRQHandler              // 38:CAN0 EWMC
.thumb_set CAN0_EWMC_IRQHandler,Default_Handler
.weak     EXTI5_9_IRQHandler                // 39:EXTI5 to EXTI9
.thumb_set EXTI5_9_IRQHandler,Default_Handler
.weak     TIMER0_BRK_TIMER8_IRQHandler      // 40:TIMER0 Break and TIMER8
.thumb_set TIMER0_BRK_TIMER8_IRQHandler,Default_Handler
.weak     TIMER0_UP_TIMER9_IRQHandler       // 41:TIMER0 Update and TIMER9
.thumb_set TIMER0_UP_TIMER9_IRQHandler,Default_Handler
.weak     TIMER0_TRG_CMT_TIMER10_IRQHandler // 42:TIMER0 Trigger and Commutation and TIMER10
.thumb_set TIMER0_TRG_CMT_TIMER10_IRQHandler,Default_Handler
.weak     TIMER0_Channel_IRQHandler         // 43:TIMER0 Channel Capture Compare
.thumb_set TIMER0_Channel_IRQHandler,Default_Handler
.weak     TIMER1_IRQHandler                 // 44:TIMER1
.thumb_set TIMER1_IRQHandler,Default_Handler
.weak     TIMER2_IRQHandler                 // 45:TIMER2
.thumb_set TIMER2_IRQHandler,Default_Handler
.weak     TIMER3_IRQHandler                 // 46:TIMER3
.thumb_set TIMER3_IRQHandler,Default_Handler
.weak     I2C0_EV_IRQHandler                // 47:I2C0 Event
.thumb_set I2C0_EV_IRQHandler,Default_Handler
.weak     I2C0_ER_IRQHandler                // 48:I2C0 Error
.thumb_set I2C0_ER_IRQHandler,Default_Handler
.weak     I2C1_EV_IRQHandler                // 49:I2C1 Event
.thumb_set I2C1_EV_IRQHandler,Default_Handler
.weak     I2C1_ER_IRQHandler                // 50:I2C1 Error
.thumb_set I2C1_ER_IRQHandler,Default_Handler
.weak     SPI0_IRQHandler                   // 51:SPI0
.thumb_set SPI0_IRQHandler,Default_Handler
.weak     SPI1_I2S1ADD_IRQHandler           // 52:SPI1 or I2S1ADD
.thumb_set SPI1_I2S1ADD_IRQHandler,Default_Handler
.weak     USART0_IRQHandler                 // 53:USART0
.thumb_set USART0_IRQHandler,Default_Handler
.weak     USART1_IRQHandler                 // 54:USART1
.thumb_set USART1_IRQHandler,Default_Handler
.weak     USART2_IRQHandler                 // 55:USART2
.thumb_set USART2_IRQHandler,Default_Handler
.weak     EXTI10_15_IRQHandler              // 56:EXTI10 to EXTI15
.thumb_set EXTI10_15_IRQHandler,Default_Handler
.weak     RTC_Alarm_IRQHandler              // 57:RTC Alarm
.thumb_set RTC_Alarm_IRQHandler,Default_Handler
.weak     USBD_WKUP_IRQHandler              // 58:USBD Wakeup
.thumb_set USBD_WKUP_IRQHandler,Default_Handler
.weak     TIMER7_BRK_TIMER11_IRQHandler     // 59:TIMER7 Break and TIMER11
.thumb_set TIMER7_BRK_TIMER11_IRQHandler,Default_Handler
.weak     TIMER7_UP_TIMER12_IRQHandler      // 60:TIMER7 Update and TIMER12
.thumb_set TIMER7_UP_TIMER12_IRQHandler,Default_Handler
.weak     TIMER7_TRG_CMT_TIMER13_IRQHandler // 61:TIMER7 Trigger and Commutation and TIMER13
.thumb_set TIMER7_TRG_CMT_TIMER13_IRQHandler,Default_Handler
.weak     TIMER7_Channel_IRQHandler         // 62:TIMER7 Channel Capture Compare
.thumb_set TIMER7_Channel_IRQHandler,Default_Handler
.weak     ADC2_IRQHandler                   // 63:ADC2
.thumb_set ADC2_IRQHandler,Default_Handler
.weak     EXMC_IRQHandler                   // 64:EXMC
.thumb_set EXMC_IRQHandler,Default_Handler
.weak     SDIO_IRQHandler                   // 65:SDIO
.thumb_set SDIO_IRQHandler,Default_Handler
.weak     TIMER4_IRQHandler                 // 66:TIMER4
.thumb_set TIMER4_IRQHandler,Default_Handler
.weak     SPI2_I2S2ADD_IRQHandler           // 67:SPI2 or I2S2ADD
.thumb_set SPI2_I2S2ADD_IRQHandler,Default_Handler
.weak     UART3_IRQHandler                  // 68:UART3
.thumb_set UART3_IRQHandler,Default_Handler
.weak     UART4_IRQHandler                  // 69:UART4
.thumb_set UART4_IRQHandler,Default_Handler
.weak     TIMER5_DAC_IRQHandler             // 70:TIMER5 or DAC
.thumb_set TIMER5_DAC_IRQHandler,Default_Handler
.weak     TIMER6_IRQHandler                 // 71:TIMER6
.thumb_set TIMER6_IRQHandler,Default_Handler
.weak     DMA1_Channel0_IRQHandler          // 72:DMA1 Channel0
.thumb_set DMA1_Channel0_IRQHandler,Default_Handler
.weak     DMA1_Channel1_IRQHandler          // 73:DMA1 Channel1
.thumb_set DMA1_Channel1_IRQHandler,Default_Handler
.weak     DMA1_Channel2_IRQHandler          // 74:DMA1 Channel2
.thumb_set DMA1_Channel2_IRQHandler,Default_Handler
.weak     DMA1_Channel3_4_IRQHandler        // 75:DMA1 Channel3 and Channel4
.thumb_set DMA1_Channel3_4_IRQHandler,Default_Handler

.weak     CAN1_TX_IRQHandler                // 79:CAN1 TX
.thumb_set CAN1_TX_IRQHandler,Default_Handler
.weak     CAN1_RX0_IRQHandler               // 80:CAN1 RX0
.thumb_set CAN1_RX0_IRQHandler,Default_Handler
.weak     CAN1_RX1_IRQHandler               // 81:CAN1 RX1
.thumb_set CAN1_RX1_IRQHandler,Default_Handler
.weak     CAN1_EWMC_IRQHandler              // 82:CAN1 EWMC
.thumb_set CAN1_EWMC_IRQHandler,Default_Handler

.weak     SHRTIMER_IRQ2_IRQHandler          // 85:SHRTIMER IRQ2
.thumb_set SHRTIMER_IRQ2_IRQHandler,Default_Handler
.weak     SHRTIMER_IRQ3_IRQHandler          // 86:SHRTIMER IRQ3
.thumb_set SHRTIMER_IRQ3_IRQHandler,Default_Handler
.weak     SHRTIMER_IRQ4_IRQHandler          // 87:SHRTIMER IRQ4
.thumb_set SHRTIMER_IRQ4_IRQHandler,Default_Handler
.weak     SHRTIMER_IRQ5_IRQHandler          // 88:SHRTIMER IRQ5
.thumb_set SHRTIMER_IRQ5_IRQHandler,Default_Handler
.weak     SHRTIMER_IRQ6_IRQHandler          // 89:SHRTIMER IRQ6
.thumb_set SHRTIMER_IRQ6_IRQHandler,Default_Handler

.weak     SHRTIMER_IRQ0_IRQHandler          // 92:SHRTIMER IRQ0
.thumb_set SHRTIMER_IRQ0_IRQHandler,Default_Handler
.weak     SHRTIMER_IRQ1_IRQHandler          // 93:SHRTIMER IRQ1
.thumb_set SHRTIMER_IRQ1_IRQHandler,Default_Handler

.weak     I2C2_EV_IRQHandler                // 98:I2C2 Event
.thumb_set I2C2_EV_IRQHandler,Default_Handler
.weak     I2C2_ER_IRQHandler                // 99:I2C2 Error
.thumb_set I2C2_ER_IRQHandler,Default_Handler
.weak     USART5_IRQHandler                 // 100:USART5
.thumb_set USART5_IRQHandler,Default_Handler
.weak     I2C2_WKUP_IRQHandler              // 101:I2C2 Wakeup
.thumb_set I2C2_WKUP_IRQHandler,Default_Handler
.weak     USART5_WKUP_IRQHandler            // 102:USART5 Wakeup
.thumb_set USART5_WKUP_IRQHandler,Default_Handler



# 实验1




[TOC]



## 环境配置

​       实时操作系统$μC/OS-II$

​       实验平台 $Keil$

## 任务一  设计系统调用

### 任务描述

　　设计以下系统调用，以完成用户态下的时钟初始化和显示输出：
1. 初始化时钟

2. 显示输出

  要求：

3. 以上两个系统调用同时存在

4. 中断处理过程尽可能短

### 实现过程

#### 根据提示输出Hello world!

   在主函数中通过反注释掉 ASM_Switch_To_Unprivileged()来启动用户模式。此时运行lab1程序会发现无法成功输出，因为用户模式下无法进行正常输出，因此需要逐步调试main函数。当程序执行到print_str()函数时，会调用fputcp函数。但在用户模式下，由于fputcp函数涉及IO操作，无法正常执行。



​     首先，在user_asm.s 文件中声明 ASM_Switch_To_Unprivileged()为全局属性，这样其他文件才能访问，同 时 , 在 调 用 ASM_Switch_To_Unprivileged() 的 文 件 中 也 要 声 明ASM_Switch_To_Unprivileged()的属性。



​      由于在用户模式下不能进行正常的输出，因此对main函数进行逐步调试。当进行到print_str()函数时，函数调用fputcp 函数。而在用户模式下，fputcp函数因为有IO操作所以不能正常进行，需要将print_str()函数切换到系统调用下进行。在新的系统调用函数syscall_print_str()中，通过定义全局指针buffer，将需要打印的内容进行复制。然后通过SWI 0x01跳转到startup_stm32f10x_md.s 文件的中断处理程序SVC_Handler中。由中断处理程序进行调用stm32f10x_it.c 文件中的处理函数SVC_Handler_Main，再通过判断立即数，执行相应的真正中断处理，进行case 0x01的操作。



​      在case 0x01中，需要解决用户态下无法运行的fputcp函数进行打印的问题。在现在的处理程序下，已经处于内核态，系统调用介入，所以可以调用print_str()函数进行打印输出。而由于一开始在syscall_print_str()函数中，将需要打印的内容复制到了全局指针buffer中，所以可以通过添加语句print_str((char*)buffer)，实现打印Hello world!的目的。

  

#### 实现时钟初始化

​       和2.2.1实现输出的步骤相同，需要在main函数中进行逐步调试。然而在用户模式下，无法正常输出信息。当我们执行systick_init()函数中的语句SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC)时，会出现中断并输出错误信息“Hard Fault exception occurs!”。这表明在用户态下，systick_init()函数无法正常运行，我们需要切换到系统调用才能继续运行。

​       于是定义新的系统调用函数syscall_systick()，通过定义全局指针buffer，将时钟间隔进行复制。同时执行相应的中断处理，进行case 0x02的操作.



​    在0x02中，需要解决用户态下无法运行systick()函数进行初始化的问题。由于当前处理程序处于内核态且系统调用已介入，因此可以调用SysTick_Config()函数进行时钟初始化。由于在syscall_systick()函数中已将时钟间隔复制到全局指针buffer中，因此可以通过添加语句SysTick_Config(*(uint32_t *) buffer)来实现初始化时钟的目的。然后，需要调用SysTick_Handler()函数来输出“hello from sysTick”。



最终成果输出结果





## 任务二 使用 OSTaskCreate ()建立多个任务

### 任务描述

　　修改操作系统与 CPU 有关的源代码，使任务运行在用户态：
1. 使用 OSTaskCreate ()建立两个任务，并交替输出
要求：
1. CPU 为 unprivileged 模式
【注意：实验开始之前，注释掉 SysTick_Handle 函数中的“print_str("hello from
sysTick!\n");”以去掉不必要的输出】

### 实现过程

​     Unprivileged模式下，用户模式不能进行正常的输出。

​     因此对main函数进行逐步调试。当进行到OSStart()函数时，函数调用OSStartHighRdy()函数。而在用户模式下，OSStartHighRdy()不能正常进行，需要将OSStartHighRdy()函数切换到系统调用下进行，使用0x03跳转。

​      在stm32f10x_it.c文件的case 0x03中，已经处于内核态，系统调用介入，所以可以调用OSStartHighRdy()函数进行继续运行，从而解决该问题。



​     继续调试，程序仍然报错“Hard Fault exception occurs!”，发现在os_time.c文件下的OSTimeDly ()函数调用切换任务的函数OS_Sched()出错，在用户模式下不能正常运行，需要将OS_Sched()函数切换到系统调用下进行。



​      与前面三处改动相同，在在case 0x04中，已经处于内核态，系统调用介入，所以可以调用OS_Sched()进行任务切换。



​       继续调试程序，发现可以正常建立两个任务并切换交替输出。

 



## 附加题

### 任务描述

​     前面的任务中打印函数 syscall_print_str 需要事先准备一个 buffer，尝试给出
一个不需要 buffer 的版本 

  ### 实现过程

​        去掉指针，逐步调试源代码，发现在调用函数 syscall_print_str() 时，程序将字符串的地址放到了寄存器$R_0 $中，随后又转移到寄存器$R_5$中，所以尝试修改软中断处理程序，将 $R_5 $的值传到函数 SVC_Handler_Main ()中，利用其访问目的地址，即可在内核态中输出字符串。

​        修改user_asm.s 文件中的 SVC_Handler，将寄存器值传给函数。

​       修改SVC_Handler_Main ()函数：

​       得到正确输出：



## 心得

​     一，在操作系统中，用户态和内核态是两个重要的概念。用户态是指进程在运行时所处的状态，它只能访问自己的内存空间，不能访问系统内核的资源，如硬件设备等。而内核态是指进程在执行系统调用时，由用户态进入内核态，此时进程可以访问系统内核的资源，并使用特权指令来操作硬件设备。

​      在用户态下，如果需要访问内核态的资源或调用内核态的函数，就需要通过系统调用的方式来实现。而正确的做法是设置软中断，进行系统调用，程序会自动由用户态变为内核态，然后可以使用特权指令，完成相应的操作。这种方式保证了操作系统的安全性和稳定性，同时也保证了应用程序的稳定性。

​     二，关于附加题，可以学习到，寄存器是CPU中的一个重要部件，它用于存储CPU指令的操作数和结果。在函数执行过程中，寄存器的数值会不断地改变，这些变化反映了函数的执行过程和逻辑。因此，通过观察寄存器的数值变化，可以更好地理解函数的运行原理，帮助我们进行调试和优化。


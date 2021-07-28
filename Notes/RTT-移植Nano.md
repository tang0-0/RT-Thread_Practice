# RTT-移植Nano

## 一、准备工作

1. STM32F103模板工程
2. RTT-nano源码 <https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-nano/an0038-nano-introduction>

## 二、增减文件
1. rt-thread\bsp里只保留`board.c`和`rtconfig.h`
2. rt-thread\libcpu里只保留`cortex-m3`
3. 在MDK的工程文件路径里加入:

- `rt-thread\bsp`
- `rt-thread\components\finsh`
- `rt-thread\include`
- `rt-thread\include\libc`

![](RTT-%E7%A7%BB%E6%A4%8DNano.assets/rtt1.png)

4. 在MDK的工程文件Groups中添加`RTT/src`，加入`rt-thread\src`文件夹中的所有*.c文件
5. 在MDK的工程文件Groups中添加`RTT/port`，加入`rt-thread\libcpu\cortex-m3`文件夹中`context_rvds.S`和`cpuport.c`以及`rt-thread\bsp`中的`rtconfig.h`

![](RTT-%E7%A7%BB%E6%A4%8DNano.assets/rtt2.png)

## 三、修改文件

1. 注释`rtconfig.h`中的#include "RTE_Components.h"
2. 编译会发现报错，解决方法是将`stm32f10x_it.c`文件中的这个三个中断函数注释掉

```
linking...
.\Output\F103xE.axf: Error: L6200E: Symbol HardFault_Handler multiply defined (by context_rvds.o and stm32f10x_it.o).
.\Output\F103xE.axf: Error: L6200E: Symbol PendSV_Handler multiply defined (by context_rvds.o and stm32f10x_it.o).
.\Output\F103xE.axf: Error: L6200E: Symbol SysTick_Handler multiply defined (by board.o and bsp_systick.o).
Not enough information to list image symbols.
Not enough information to list load addresses in the image map.
Finished: 2 information, 0 warning and 3 error messages.
".\Output\F103xE.axf" - 3 Error(s), 0 Warning(s).
```

3. 修改`rtconfig.h`，取消`#define RT_USING_HEAP`这一行的注释，开启动态堆栈
4. 新建board.h，修改board.c

```c
#ifndef __BOARD_H__
#define __BOARD_H__

/* STM32 固件库头文件 */
#include "stm32f10x.h"
/* 开发板硬件bsp头文件 */
#include "bsp_uart.h"

void rt_hw_board_init(void);
void SysTick_Handler(void);
	
#endif 

```

```c
/* 开发板硬件相关头文件 */
#include "board.h" 
/* RT-Thread相关头文件 */
#include <rthw.h>
#include <rtthread.h>

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 2048
/* 从内部SRAM里面分配一部分静态内存来作为rtt的堆空间，这里配置为8KB */
static uint32_t rt_heap[RT_HEAP_SIZE];
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
  * @brief  开发板硬件初始化函数
  * @param  无
  * @retval 无
  *
  * @attention
  * RTT把开发板相关的初始化函数统一放到board.c文件中实现，
  * 当然，你想把这些函数统一放到main.c文件也是可以的。
  */
void rt_hw_board_init()
{
    /* 初始化SysTick */
    SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );	
    
	/* 硬件BSP初始化统统放在这里，比如LED，串口，LCD等 */

	
/* 调用组件初始化函数 (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
    
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
	rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
    
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

/**
  * @brief  SysTick中断服务函数
  * @param  无
  * @retval 无
  *
  * @attention
  * SysTick中断服务函数在固件库文件stm32f10x_it.c中也定义了，而现在
  * 在board.c中又定义一次，那么编译的时候会出现重复定义的错误，解决
  * 方法是可以把stm32f10x_it.c中的注释或者删除即可。
  */
void SysTick_Handler(void)
{
    /* 进入中断 */
    rt_interrupt_enter();

    /* 更新时基 */
    rt_tick_increase();

    /* 离开中断 */
    rt_interrupt_leave();
}

```

## 四、适配rt_printf

1. 在`void rt_hw_board_init()`中加入相应串口初始化
2. 在`board.c`中适配`void rt_hw_console_output(const char *str)`

```c
void rt_hw_console_output(const char *str)
{	
	/* 进入临界段 */
    rt_enter_critical();

	/* 直到字符串结束 */
    while (*str!='\0')
	{
		
		USART_SendData(DEBUG_USARTx, *str++); 				
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);	
	}	

	/* 退出临界段 */
    rt_exit_critical();
}
```

3. 编写测试线程

```c
#include "board.h"
#include "rtthread.h"

/* 定义线程控制块指针 */
static rt_thread_t test_thread = RT_NULL;
/* 线程实现 */
static void test_thread_entry(void *parameter)
{
    while (1)
    {
		rt_kprintf("test code!\n");
        rt_thread_delay(1000);   /* 延时1000个tick */
    }
}

int main(void)
{	
	
	test_thread =                          /* 线程控制块指针 */
        rt_thread_create("test",               /* 线程名字 */
                         test_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         3,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (test_thread != RT_NULL)
        rt_thread_startup(test_thread);
    else
        return -1;
}

```

![](RTT-%E7%A7%BB%E6%A4%8DNano.assets/rtt3.png)

## 五、 适配Finsh

1. 在MDK的工程文件Groups中添加`RTT/components`，加入`rt-thread\components\finsh`文件夹中的 `cmd.c`、`msh.c`、`shell.c`、 `finsh_port.c`
2. 适配`finsh_port.c`中的`char rt_hw_console_getchar(void)`

```c
char rt_hw_console_getchar(void)
{
    /* Note: the initial value of ch must < 0 */
    int ch = -1;

    /*等待串口1输入数据*/
    if (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) != RESET)
    {
        ch = (int)USART_ReceiveData(DEBUG_USARTx);
        USART_ClearFlag(DEBUG_USARTx, USART_FLAG_RXNE);
    }
    else
    {
        if (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_ORE) != RESET)
        {
            USART_ClearFlag(DEBUG_USARTx, USART_FLAG_ORE);
        }
        rt_thread_mdelay( 10 );
    }

    return ch;
}

```

3. 在`rtconfig.h`中加入`#include "finsh_config.h"`

![](RTT-%E7%A7%BB%E6%A4%8DNano.assets/rtt4.png)

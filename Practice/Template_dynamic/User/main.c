/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RT-Thread 3.0 + STM32 工程模版
  *********************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */

/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/
#include "board.h"
#include "rtthread.h"



/*
*************************************************************************
*                               变量
*************************************************************************
*/

/* 定义线程控制块指针 */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t led2_thread = RT_NULL;
static rt_thread_t get_cpu_use_thread = RT_NULL;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/

static void led1_thread_entry(void *parameter)
{
    while (1)
    {
        LED1_ON;
        // rt_kprintf("led1 on\r\n");
        rt_thread_delay(500);   /* 延时500个tick */

        LED1_OFF;
        // rt_kprintf("led1 off\r\n");
        rt_thread_delay(500);   /* 延时500个tick */

    }
}

static void led2_thread_entry(void *parameter)
{
    rt_uint16_t i;
    while (1)
    {
        LED2_TOGGLE;
        for (i = 0; i < 10000; i++);
        rt_thread_delay(5);
        
    }
}

static void get_cpu_use_thread_entry(void *parameter)
{
    rt_uint8_t major, minor;

    while (1)
    {
        /* 获取 CPU 利用率数据 */
        cpu_usage_get(&major, &minor);

        /* 打印 CPU 利用率 */
        rt_kprintf("CPU %d.%d%\r\n", major, minor);

        rt_thread_delay(1000); /* 延时 1000 个 tick */

    }
}



/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    // led1_thread =                          /* 线程控制块指针 */
    //     rt_thread_create("led1",               /* 线程名字 */
    //                      led1_thread_entry,   /* 线程入口函数 */
    //                      RT_NULL,             /* 线程入口函数参数 */
    //                      512,                 /* 线程栈大小 */
    //                      3,                   /* 线程的优先级 */
    //                      20);                 /* 线程时间片 */

    // /* 启动线程，开启调度 */
    // if (led1_thread != RT_NULL)
    //     rt_thread_startup(led1_thread);
    // else
    //     return -1;

    led2_thread =                          /* 线程控制块指针 */
        rt_thread_create("led2",               /* 线程名字 */
                         led2_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         3,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (led2_thread != RT_NULL)
        rt_thread_startup(led2_thread);
    else
        return -1;

    get_cpu_use_thread = /* 线程控制块指针 */
        rt_thread_create("get_cpu_use",  /* 线程名字 */
                         get_cpu_use_thread_entry, /* 线程入口函数 */
                         RT_NULL, /* 线程入口函数参数 */
                         512, /* 线程栈大小 */
                         5, /* 线程的优先级 */
                         20); /* 线程时间片 */

    if (get_cpu_use_thread != RT_NULL)
        rt_thread_startup(get_cpu_use_thread);
    else
        return -1;

}


/********************************END OF FILE****************************/

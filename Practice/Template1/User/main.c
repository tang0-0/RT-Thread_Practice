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
/* 定义线软件定时器制块 */
static rt_timer_t swtmr1 = RT_NULL;
static rt_timer_t swtmr2 = RT_NULL;

static uint32_t TmrCb_Count1 = 0;
static uint32_t TmrCb_Count2 = 0;

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
    while (1)
    {
        LED2_ON;
        rt_thread_delay(300);   /* 延时300个tick */

        LED2_OFF;
        rt_thread_delay(300);   /* 延时300个tick */

    }
}

static void swtmr1_callback(void *parameter)
{
    uint32_t tick_num1;

    TmrCb_Count1++;                     /* 每回调一次加一 */

    tick_num1 = (uint32_t)rt_tick_get();    /* 获取滴答定时器的计数值 */

    rt_kprintf("swtmr1_callback: %d\n", TmrCb_Count1);
    rt_kprintf("systick=%d\n", tick_num1);

}


static void swtmr2_callback(void *parameter)
{
    uint32_t tick_num2;

    TmrCb_Count2++;             /* 每回调一次加一 */

    tick_num2 = (uint32_t)rt_tick_get();    /* 获取滴答定时器的计数值 */

    rt_kprintf("swtmr2_callback:%d\n", TmrCb_Count2);

    rt_kprintf("systick=%d\n", tick_num2);

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
    led1_thread =                          /* 线程控制块指针 */
        rt_thread_create("led1",               /* 线程名字 */
                         led1_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         4,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (led1_thread != RT_NULL)
        rt_thread_startup(led1_thread);
    else
        return -1;

    led2_thread =                          /* 线程控制块指针 */
        rt_thread_create("led2",               /* 线程名字 */
                         led2_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         4,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (led2_thread != RT_NULL)
        rt_thread_startup(led2_thread);
    else
        return -1;

    /* 创建一个软件定时器 */
    swtmr1 = rt_timer_create("swtmr1_callback", /* 软件定时器的名称 */
                             swtmr1_callback,/* 软件定时器的回调函数 */
                             RT_NULL,          /* 定时器超时函数的入口参数 */
                             5000,   /* 软件定时器的超时时间(周期回调时间) */
                             RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
    /* 软件定时器模式 一次模式 */
    /* 启动定时器 */
    if (swtmr1 != RT_NULL)
        rt_timer_start(swtmr1);

    /* 创建一个软件定时器 */
    swtmr2 = rt_timer_create("swtmr2_callback", /* 软件定时器的名称 */
                             swtmr2_callback,/* 软件定时器的回调函数 */
                             RT_NULL,          /* 定时器超时函数的入口参数 */
                             1000,   /* 软件定时器的超时时间(周期回调时间) */
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER);
    /* 软件定时器模式 周期模式 */
    /* 启动定时器 */
    if (swtmr2 != RT_NULL)
        rt_timer_start(swtmr2);

}


/********************************END OF FILE****************************/

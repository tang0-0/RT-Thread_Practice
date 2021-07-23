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
static rt_thread_t key0_thread = RT_NULL;
static rt_thread_t key1_thread = RT_NULL;
/* 定义互斥量控制块 */
static rt_mutex_t test_mux = RT_NULL;
uint32_t num = 0;
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

static void key0_thread_entry(void *parameter)
{
    while (1)
    {
        rt_mutex_take(test_mux,           /* 获取互斥量 */
                      RT_WAITING_FOREVER);    /* 等待时间：一直等 */
        ++num;
        rt_thread_delay(1000);
        rt_thread_delay(1000);
        rt_thread_delay(1000);

        rt_mutex_release(test_mux);    //释放互斥量

        rt_thread_delay(10);

    }

}

static void key1_thread_entry(void *parameter)
{
    while (1)
    {
        rt_mutex_take(test_mux,           /* 获取互斥量 */
                      RT_WAITING_FOREVER);    /* 等待时间：一直等 */

        rt_kprintf("num:%d\n", num);

        rt_mutex_release(test_mux);    //释放互斥量

        rt_thread_delay(10);

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
    led1_thread =                          /* 线程控制块指针 */
        rt_thread_create("led1",               /* 线程名字 */
                         led1_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         5,                   /* 线程的优先级 */
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
    /* 
    使用 RT_IPC_FLAG_PRIO 优先级 flag 创建的 IPC 对象，在多个线程等
    待资源时，将由优先级高的线程优先获得资源。而使用 RT_IPC_FLAG_FIFO 先进先出 flag
    创建的 IPC 对象，在多个线程等待资源时，将按照先来先得的顺序获得资源。 
    */
    test_mux = rt_mutex_create("test_mux", RT_IPC_FLAG_PRIO);
    if (test_mux != RT_NULL)
        rt_kprintf("creat mux success\n\n");

    key1_thread =                          /* 线程控制块指针 */
        rt_thread_create("key1",               /* 线程名字 */
                         key1_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         3,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (key1_thread != RT_NULL)
        rt_thread_startup(key1_thread);
    else
        return -1;

    key0_thread =                          /* 线程控制块指针 */
        rt_thread_create("key0",               /* 线程名字 */
                         key0_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         2,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (key0_thread != RT_NULL)
        rt_thread_startup(key0_thread);
    else
        return -1;
}


/********************************END OF FILE****************************/

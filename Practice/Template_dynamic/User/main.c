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
static rt_thread_t alloc_thread = RT_NULL;
static rt_thread_t free_thread = RT_NULL;
/* 定义申请内存的指针 */
static char *p_test = RT_NULL;

/* 相关宏定义 */
#define  TEST_SIZE   100      //内存大小

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

static void alloc_thread_entry(void *parameter)
{

    p_test = rt_malloc(TEST_SIZE);    /* 申请动态内存 */
    if (RT_NULL == p_test) /* 没有申请成功 */
        rt_kprintf("alloc failed\n");
    else
        rt_kprintf("alloc success:%d\n\n", p_test);

    rt_memmove(p_test,"hello coder",13);
    rt_kprintf("*p_test = %s,address:%d \n\n", p_test, p_test);

    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        rt_thread_delay(1000);     //每1000ms扫描一次
    }
}

static void free_thread_entry(void *parameter)
{
    rt_free(p_test);
    rt_kprintf("free success\n\n");

    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        LED1_TOGGLE;
        rt_thread_delay(500);     //每500ms扫描一次
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
                         5,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (led2_thread != RT_NULL)
        rt_thread_startup(led2_thread);
    else
        return -1;

    alloc_thread =                          /* 线程控制块指针 */
    rt_thread_create( "alloc",              /* 线程名字 */
                      alloc_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      1,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
                   
    /* 启动线程，开启调度 */
   if (alloc_thread != RT_NULL)
        rt_thread_startup(alloc_thread);
    else
        return -1;
    
  free_thread =                          /* 线程控制块指针 */
    rt_thread_create( "free",              /* 线程名字 */
                      free_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      2,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
                   
    /* 启动线程，开启调度 */
   if (free_thread != RT_NULL)
        rt_thread_startup(free_thread);
    else
        return -1;



}


/********************************END OF FILE****************************/

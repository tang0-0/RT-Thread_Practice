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
        rt_kprintf("led1 on\r\n");
        rt_thread_delay(500);   /* 延时500个tick */

        LED1_OFF;
        rt_kprintf("led1 off\r\n");
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
    rt_err_t res = RT_EOK;

    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
        {
            rt_kprintf("suspend led1\n");
            res = rt_thread_suspend(led1_thread);/* 挂起LED1线程 */
            if (RT_EOK == res)
            {
                rt_kprintf("suspend led1 success\n");
            }
            else
            {
                rt_kprintf("suspend led1 failed:0x%lx\n", res);
            }

        }

        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)/* K1 被按下 */
        {
            printf("resume led1\n");
            res = rt_thread_resume(led1_thread);/* 恢复LED1线程！ */
            if (RT_EOK == res)
            {
                rt_kprintf("resume led1 success\n");
            }
            else
            {
                rt_kprintf("resume led1 failed:0x%lx\n", res);
            }
        }

        rt_thread_delay(20);
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
                         3,                   /* 线程的优先级 */
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

    key0_thread =                          /* 线程控制块指针 */
        rt_thread_create("key",               /* 线程名字 */
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

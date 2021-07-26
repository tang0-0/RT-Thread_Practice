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
static rt_thread_t receive0_thread = RT_NULL;
static rt_thread_t receive1_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
/* 定义邮箱控制块 */
static rt_mailbox_t test_mail = RT_NULL;

char test_str1[] = "this is a mail test 1";/* 邮箱消息test1 */
char test_str2[] = "this is a mail test 2";/* 邮箱消息test2 */

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

static void receive0_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    char *r_str;
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        /* 等待接邮箱消息 */
        uwRet = rt_mb_recv(test_mail, /* 邮箱对象句柄 */
                           (rt_uint32_t *)&r_str, /* 接收邮箱消息 */
                           0);/* 指定超时事件,一直等 */

        if (RT_EOK == uwRet) /* 如果接收完成并且正确 */
        {
            rt_kprintf("rev0:%s\n\n", r_str);
        }
        // else
        //     rt_kprintf("mb0 failed:0x%x\n", uwRet);
        
        rt_thread_delay(10);
    }
}

static void receive1_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    char *r_str;
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        /* 等待接邮箱消息 */
        uwRet = rt_mb_recv(test_mail, /* 邮箱对象句柄 */
                           (rt_uint32_t *)&r_str, /* 接收邮箱消息 */
                           0);/* 指定超时事件,一直等 */

        if (RT_EOK == uwRet) /* 如果接收完成并且正确 */
        {
            rt_kprintf("rev1:%s\n\n", r_str);
        }
        // else
        //     rt_kprintf("mb1 failed:0x%x\n", uwRet);
        
        rt_thread_delay(10);
    }
}

static void send_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        //如果KEY1被单击
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
        {
            rt_kprintf("KEY0\n");
            /* 发送一个邮箱消息1 */
            uwRet = rt_mb_send(test_mail, (rt_uint32_t)&test_str1);
            if (RT_EOK == uwRet)
                rt_kprintf("mb0 send success\n");
            else
                rt_kprintf("mb0 send failed\n");
        }
        //如果KEY2被单击
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
        {
            rt_kprintf("KEY1\n");
            /* 发送一个邮箱2 */
            uwRet = rt_mb_send(test_mail, (rt_uint32_t)&test_str2);
            if (RT_EOK == uwRet)
                rt_kprintf("mb1 send success\n");
            else
                rt_kprintf("mb1 send failed\n");
        }
        rt_thread_delay(20);     //每20ms扫描一次
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
    //                      5,                   /* 线程的优先级 */
    //                      20);                 /* 线程时间片 */

    // /* 启动线程，开启调度 */
    // if (led1_thread != RT_NULL)
    //     rt_thread_startup(led1_thread);
    // else
    //     return -1;

    // led2_thread =                          /* 线程控制块指针 */
    //     rt_thread_create("led2",               /* 线程名字 */
    //                      led2_thread_entry,   /* 线程入口函数 */
    //                      RT_NULL,             /* 线程入口函数参数 */
    //                      512,                 /* 线程栈大小 */
    //                      5,                   /* 线程的优先级 */
    //                      20);                 /* 线程时间片 */

    // /* 启动线程，开启调度 */
    // if (led2_thread != RT_NULL)
    //     rt_thread_startup(led2_thread);
    // else
    //     return -1;

    /* 创建一个邮箱 */
    test_mail = rt_mb_create("test_mail", /* 邮箱名字 */
                             10,
                             RT_IPC_FLAG_FIFO);/* 信号量模式 FIFO(0x00)*/
    if (test_mail != RT_NULL)
        rt_kprintf("creat mb success\n\n");

    receive0_thread =                          /* 线程控制块指针 */
        rt_thread_create("receive0",               /* 线程名字 */
                         receive0_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         3,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (receive0_thread != RT_NULL)
        rt_thread_startup(receive0_thread);
    else
        return -1;
    
    receive1_thread =                          /* 线程控制块指针 */
        rt_thread_create("receive1",               /* 线程名字 */
                         receive1_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         3,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (receive1_thread != RT_NULL)
        rt_thread_startup(receive1_thread);
    else
        return -1;

    send_thread =                          /* 线程控制块指针 */
        rt_thread_create("send",               /* 线程名字 */
                         send_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         2,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (send_thread != RT_NULL)
        rt_thread_startup(send_thread);
    else
        return -1;



}


/********************************END OF FILE****************************/

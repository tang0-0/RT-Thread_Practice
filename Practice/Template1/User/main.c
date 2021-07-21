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
static rt_thread_t receive_thread = RT_NULL;
/* 定义消息队列控制块 */
static rt_mq_t test_mq = RT_NULL;

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
    rt_err_t uwRet = RT_EOK;
    uint32_t send_data1 = 1;
    uint32_t send_data2 = 2;
    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)/* K1 被按下 */
        {
            /* 将数据写入（发送）到队列中，等待时间为 0  */
            uwRet = rt_mq_send(test_mq,  /* 写入（发送）队列的ID(句柄) */
                               &send_data1,            /* 写入（发送）的数据 */
                               sizeof(send_data1));            /* 数据的长度 */
            if (RT_EOK != uwRet)
            {
                rt_kprintf("send1 failed:%lx\n", uwRet);
            }
        }
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)/* K1 被按下 */
        {
            /* 将数据写入（发送）到队列中，等待时间为 0  */
            uwRet = rt_mq_send(test_mq,  /* 写入（发送）队列的ID(句柄) */
                               &send_data2,            /* 写入（发送）的数据 */
                               sizeof(send_data2));            /* 数据的长度 */
            if (RT_EOK != uwRet)
            {
                rt_kprintf("send2 failed:%lx\n", uwRet);
            }
        }
        rt_thread_delay(20);
    }


}

static void receive_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    uint32_t r_queue;
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        /* 队列读取（接收），等待时间为一直等待 */
        uwRet = rt_mq_recv(test_mq, /* 读取（接收）队列的ID(句柄) */
                           &r_queue,           /* 读取（接收）的数据保存位置 */
                           sizeof(r_queue),        /* 读取（接收）的数据的长度 */
                           RT_WAITING_FOREVER);    /* 等待时间：一直等 */
        if (RT_EOK == uwRet)
        {
            rt_kprintf("rev:%d\n", r_queue);
        }
        else
        {
            rt_kprintf("rev failed:0x%lx\n", uwRet);
        }
        rt_thread_delay(200);
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

    test_mq = rt_mq_create("test_mq",/* 消息队列名字 */
                           4,     /* 消息的最大长度 */
                           20,    /* 消息队列的最大容量 */
                           RT_IPC_FLAG_FIFO);/* 队列模式 FIFO(0x00)*/
    if (test_mq != RT_NULL)
        rt_kprintf("creat message line success\r\n");

    receive_thread =                          /* 线程控制块指针 */
        rt_thread_create("receive",               /* 线程名字 */
                         receive_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         3,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (receive_thread != RT_NULL)
        rt_thread_startup(receive_thread);
    else
        return -1;

    key0_thread =                          /* 线程控制块指针 */
        rt_thread_create("send",               /* 线程名字 */
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

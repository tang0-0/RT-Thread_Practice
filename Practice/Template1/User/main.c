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
static rt_thread_t handler_thread = RT_NULL;
/* 定义事件控制块 */
static rt_event_t test_event = RT_NULL;

#define KEY1_EVENT  (0x01 << 0)//设置事件掩码的位0
#define KEY2_EVENT  (0x01 << 1)//设置事件掩码的位1

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
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)       //如果KEY2被单击
        {
            rt_kprintf("KEY1 pressed\n");
            /* 发送一个事件1 */
            rt_event_send(test_event, KEY1_EVENT);
        }

        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)       //如果KEY2被单击
        {
            rt_kprintf("KEY2 pressed\n");
            /* 发送一个事件2 */
            rt_event_send(test_event, KEY2_EVENT);
        }
        rt_thread_delay(20);     //每20ms扫描一次
    }

}

static void handler_thread_entry(void *parameter)
{
    rt_uint32_t recved;

    while (1)
    {
        /* 等待接收事件标志 */
        rt_event_recv(test_event,  /* 事件对象句柄 */
                      KEY1_EVENT | KEY2_EVENT, /* 接收线程感兴趣的事件 */
                      RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, /* 接收选项 */
                      RT_WAITING_FOREVER,/* 指定超时事件,一直等 */
                      &recved);    /* 指向接收到的事件 */
        if (recved == (KEY1_EVENT | KEY2_EVENT)) /* 如果接收完成并且正确 */
        {
            rt_kprintf("Key1 and Key2\n");
        }
        else
            rt_kprintf("failed\n");

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
    test_event = rt_event_create("test_event",/* 事件标志组名字 */
                                 RT_IPC_FLAG_PRIO); /* 事件模式 FIFO(0x00)*/
    if (test_event != RT_NULL)
        rt_kprintf("creat event success\r\n");

    handler_thread =                          /* 线程控制块指针 */
        rt_thread_create("handler",               /* 线程名字 */
                         handler_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         3,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (handler_thread != RT_NULL)
        rt_thread_startup(handler_thread);
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

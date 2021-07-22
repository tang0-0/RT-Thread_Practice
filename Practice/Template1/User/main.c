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
/* 定义信号量控制块 */
static rt_sem_t test_sem = RT_NULL;
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
    rt_err_t uwRet = RT_EOK;

    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)/* K1 被按下 */
        {
            uwRet = rt_sem_release(	test_sem	);   //释放二值信号量
            if (RT_EOK != uwRet)
            {
                rt_kprintf("release failed:%lx\n", uwRet);
            }
            else
            {
                rt_kprintf("release\n");
            }
        }
        
        rt_thread_delay(20);
    }


}

static void receive_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        uwRet = rt_sem_take(test_sem,	/* 获取信号量 */
                  RT_WAITING_FOREVER); /* 等待时间：一直等 */	

        if (RT_EOK == uwRet)
        {
            rt_kprintf("key pressed\n");
        }
        else
        {
            rt_kprintf("sem_take failed:0x%lx\n", uwRet);
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

    /* 创建一个信号量 */
	test_sem = rt_sem_create("test_sem",/* 信号量名字 */
                     0,     /* 信号量初始值 */
                     RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
    if (test_sem != RT_NULL)
        rt_kprintf("create sem success\n\n");

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

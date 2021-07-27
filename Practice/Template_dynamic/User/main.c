/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RT-Thread 3.0 + STM32 ����ģ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */

/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/
#include "board.h"
#include "rtthread.h"



/*
*************************************************************************
*                               ����
*************************************************************************
*/

/* �����߳̿��ƿ�ָ�� */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t led2_thread = RT_NULL;
static rt_thread_t get_cpu_use_thread = RT_NULL;

/*
*************************************************************************
*                             ��������
*************************************************************************
*/

static void led1_thread_entry(void *parameter)
{
    while (1)
    {
        LED1_ON;
        // rt_kprintf("led1 on\r\n");
        rt_thread_delay(500);   /* ��ʱ500��tick */

        LED1_OFF;
        // rt_kprintf("led1 off\r\n");
        rt_thread_delay(500);   /* ��ʱ500��tick */

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
        /* ��ȡ CPU ���������� */
        cpu_usage_get(&major, &minor);

        /* ��ӡ CPU ������ */
        rt_kprintf("CPU %d.%d%\r\n", major, minor);

        rt_thread_delay(1000); /* ��ʱ 1000 �� tick */

    }
}



/*
*************************************************************************
*                             main ����
*************************************************************************
*/
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    // led1_thread =                          /* �߳̿��ƿ�ָ�� */
    //     rt_thread_create("led1",               /* �߳����� */
    //                      led1_thread_entry,   /* �߳���ں��� */
    //                      RT_NULL,             /* �߳���ں������� */
    //                      512,                 /* �߳�ջ��С */
    //                      3,                   /* �̵߳����ȼ� */
    //                      20);                 /* �߳�ʱ��Ƭ */

    // /* �����̣߳��������� */
    // if (led1_thread != RT_NULL)
    //     rt_thread_startup(led1_thread);
    // else
    //     return -1;

    led2_thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create("led2",               /* �߳����� */
                         led2_thread_entry,   /* �߳���ں��� */
                         RT_NULL,             /* �߳���ں������� */
                         512,                 /* �߳�ջ��С */
                         3,                   /* �̵߳����ȼ� */
                         20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (led2_thread != RT_NULL)
        rt_thread_startup(led2_thread);
    else
        return -1;

    get_cpu_use_thread = /* �߳̿��ƿ�ָ�� */
        rt_thread_create("get_cpu_use",  /* �߳����� */
                         get_cpu_use_thread_entry, /* �߳���ں��� */
                         RT_NULL, /* �߳���ں������� */
                         512, /* �߳�ջ��С */
                         5, /* �̵߳����ȼ� */
                         20); /* �߳�ʱ��Ƭ */

    if (get_cpu_use_thread != RT_NULL)
        rt_thread_startup(get_cpu_use_thread);
    else
        return -1;

}


/********************************END OF FILE****************************/

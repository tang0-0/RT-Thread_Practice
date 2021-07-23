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
static rt_thread_t key0_thread = RT_NULL;
static rt_thread_t key1_thread = RT_NULL;
/* ���廥�������ƿ� */
static rt_mutex_t test_mux = RT_NULL;
uint32_t num = 0;
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
    while (1)
    {
        LED2_ON;
        rt_thread_delay(300);   /* ��ʱ300��tick */

        LED2_OFF;
        rt_thread_delay(300);   /* ��ʱ300��tick */

    }
}

static void key0_thread_entry(void *parameter)
{
    while (1)
    {
        rt_mutex_take(test_mux,           /* ��ȡ������ */
                      RT_WAITING_FOREVER);    /* �ȴ�ʱ�䣺һֱ�� */
        ++num;
        rt_thread_delay(1000);
        rt_thread_delay(1000);
        rt_thread_delay(1000);

        rt_mutex_release(test_mux);    //�ͷŻ�����

        rt_thread_delay(10);

    }

}

static void key1_thread_entry(void *parameter)
{
    while (1)
    {
        rt_mutex_take(test_mux,           /* ��ȡ������ */
                      RT_WAITING_FOREVER);    /* �ȴ�ʱ�䣺һֱ�� */

        rt_kprintf("num:%d\n", num);

        rt_mutex_release(test_mux);    //�ͷŻ�����

        rt_thread_delay(10);

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
    led1_thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create("led1",               /* �߳����� */
                         led1_thread_entry,   /* �߳���ں��� */
                         RT_NULL,             /* �߳���ں������� */
                         512,                 /* �߳�ջ��С */
                         5,                   /* �̵߳����ȼ� */
                         20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (led1_thread != RT_NULL)
        rt_thread_startup(led1_thread);
    else
        return -1;

    led2_thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create("led2",               /* �߳����� */
                         led2_thread_entry,   /* �߳���ں��� */
                         RT_NULL,             /* �߳���ں������� */
                         512,                 /* �߳�ջ��С */
                         4,                   /* �̵߳����ȼ� */
                         20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (led2_thread != RT_NULL)
        rt_thread_startup(led2_thread);
    else
        return -1;
    /* 
    ʹ�� RT_IPC_FLAG_PRIO ���ȼ� flag ������ IPC �����ڶ���̵߳�
    ����Դʱ���������ȼ��ߵ��߳����Ȼ����Դ����ʹ�� RT_IPC_FLAG_FIFO �Ƚ��ȳ� flag
    ������ IPC �����ڶ���̵߳ȴ���Դʱ�������������ȵõ�˳������Դ�� 
    */
    test_mux = rt_mutex_create("test_mux", RT_IPC_FLAG_PRIO);
    if (test_mux != RT_NULL)
        rt_kprintf("creat mux success\n\n");

    key1_thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create("key1",               /* �߳����� */
                         key1_thread_entry,   /* �߳���ں��� */
                         RT_NULL,             /* �߳���ں������� */
                         512,                 /* �߳�ջ��С */
                         3,                   /* �̵߳����ȼ� */
                         20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (key1_thread != RT_NULL)
        rt_thread_startup(key1_thread);
    else
        return -1;

    key0_thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create("key0",               /* �߳����� */
                         key0_thread_entry,   /* �߳���ں��� */
                         RT_NULL,             /* �߳���ں������� */
                         512,                 /* �߳�ջ��С */
                         2,                   /* �̵߳����ȼ� */
                         20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (key0_thread != RT_NULL)
        rt_thread_startup(key0_thread);
    else
        return -1;
}


/********************************END OF FILE****************************/

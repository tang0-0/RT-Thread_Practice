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
/* �����������ʱ���ƿ� */
static rt_timer_t swtmr1 = RT_NULL;
static rt_timer_t swtmr2 = RT_NULL;

static uint32_t TmrCb_Count1 = 0;
static uint32_t TmrCb_Count2 = 0;

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

static void swtmr1_callback(void *parameter)
{
    uint32_t tick_num1;

    TmrCb_Count1++;                     /* ÿ�ص�һ�μ�һ */

    tick_num1 = (uint32_t)rt_tick_get();    /* ��ȡ�δ�ʱ���ļ���ֵ */

    rt_kprintf("swtmr1_callback: %d\n", TmrCb_Count1);
    rt_kprintf("systick=%d\n", tick_num1);

}


static void swtmr2_callback(void *parameter)
{
    uint32_t tick_num2;

    TmrCb_Count2++;             /* ÿ�ص�һ�μ�һ */

    tick_num2 = (uint32_t)rt_tick_get();    /* ��ȡ�δ�ʱ���ļ���ֵ */

    rt_kprintf("swtmr2_callback:%d\n", TmrCb_Count2);

    rt_kprintf("systick=%d\n", tick_num2);

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
                         4,                   /* �̵߳����ȼ� */
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

    /* ����һ�������ʱ�� */
    swtmr1 = rt_timer_create("swtmr1_callback", /* �����ʱ�������� */
                             swtmr1_callback,/* �����ʱ���Ļص����� */
                             RT_NULL,          /* ��ʱ����ʱ��������ڲ��� */
                             5000,   /* �����ʱ���ĳ�ʱʱ��(���ڻص�ʱ��) */
                             RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
    /* �����ʱ��ģʽ һ��ģʽ */
    /* ������ʱ�� */
    if (swtmr1 != RT_NULL)
        rt_timer_start(swtmr1);

    /* ����һ�������ʱ�� */
    swtmr2 = rt_timer_create("swtmr2_callback", /* �����ʱ�������� */
                             swtmr2_callback,/* �����ʱ���Ļص����� */
                             RT_NULL,          /* ��ʱ����ʱ��������ڲ��� */
                             1000,   /* �����ʱ���ĳ�ʱʱ��(���ڻص�ʱ��) */
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER);
    /* �����ʱ��ģʽ ����ģʽ */
    /* ������ʱ�� */
    if (swtmr2 != RT_NULL)
        rt_timer_start(swtmr2);

}


/********************************END OF FILE****************************/

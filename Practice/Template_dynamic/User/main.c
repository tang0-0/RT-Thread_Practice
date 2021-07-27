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
static rt_thread_t alloc_thread = RT_NULL;
static rt_thread_t free_thread = RT_NULL;
/* ���������ڴ��ָ�� */
static char *p_test = RT_NULL;

/* ��غ궨�� */
#define  TEST_SIZE   100      //�ڴ��С

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

static void alloc_thread_entry(void *parameter)
{

    p_test = rt_malloc(TEST_SIZE);    /* ���붯̬�ڴ� */
    if (RT_NULL == p_test) /* û������ɹ� */
        rt_kprintf("alloc failed\n");
    else
        rt_kprintf("alloc success:%d\n\n", p_test);

    rt_memmove(p_test,"hello coder",13);
    rt_kprintf("*p_test = %s,address:%d \n\n", p_test, p_test);

    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
        rt_thread_delay(1000);     //ÿ1000msɨ��һ��
    }
}

static void free_thread_entry(void *parameter)
{
    rt_free(p_test);
    rt_kprintf("free success\n\n");

    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
        LED1_TOGGLE;
        rt_thread_delay(500);     //ÿ500msɨ��һ��
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
                         5,                   /* �̵߳����ȼ� */
                         20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (led2_thread != RT_NULL)
        rt_thread_startup(led2_thread);
    else
        return -1;

    alloc_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "alloc",              /* �߳����� */
                      alloc_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      1,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
                   
    /* �����̣߳��������� */
   if (alloc_thread != RT_NULL)
        rt_thread_startup(alloc_thread);
    else
        return -1;
    
  free_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "free",              /* �߳����� */
                      free_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      2,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
                   
    /* �����̣߳��������� */
   if (free_thread != RT_NULL)
        rt_thread_startup(free_thread);
    else
        return -1;



}


/********************************END OF FILE****************************/

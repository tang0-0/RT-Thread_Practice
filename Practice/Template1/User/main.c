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
        rt_kprintf("led1 on\r\n");
        rt_thread_delay(500);   /* ��ʱ500��tick */

        LED1_OFF;
        rt_kprintf("led1 off\r\n");
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
    rt_err_t res = RT_EOK;

    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
        {
            rt_kprintf("suspend led1\n");
            res = rt_thread_suspend(led1_thread);/* ����LED1�߳� */
            if (RT_EOK == res)
            {
                rt_kprintf("suspend led1 success\n");
            }
            else
            {
                rt_kprintf("suspend led1 failed:0x%lx\n", res);
            }

        }

        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)/* K1 ������ */
        {
            printf("resume led1\n");
            res = rt_thread_resume(led1_thread);/* �ָ�LED1�̣߳� */
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
                         3,                   /* �̵߳����ȼ� */
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

    key0_thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create("key",               /* �߳����� */
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

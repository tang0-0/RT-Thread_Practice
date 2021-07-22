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
/* �����ź������ƿ� */
static rt_sem_t test_sem = RT_NULL;
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
    rt_err_t uwRet = RT_EOK;

    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)/* K1 ������ */
        {
            uwRet = rt_sem_release(test_sem);     //�ͷ��ź���
            if (RT_EOK != uwRet)
            {
                rt_kprintf("release failed:%lx\n", uwRet);
            }
            else
            {
                rt_kprintf("release one sem\n");
            }
        }

        rt_thread_delay(20);
    }


}

static void key1_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    while (1)
    {
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)       //���KEY2������
        {
            /* ��ȡһ�������ź��� */
            uwRet = rt_sem_take(test_sem, 0);  /* ��ȡ�ź��� */
            if (RT_EOK == uwRet)
                rt_kprintf("take one sem\r\n");
            else
                rt_kprintf("up\r\n");

        }
        rt_thread_delay(20);     //ÿ20msɨ��һ��

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

    /* ����һ���ź��� */
    test_sem = rt_sem_create("test_sem",/* �ź������� */
                             5,     /* �ź�����ʼֵ */
                             RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
    if (test_sem != RT_NULL)
        rt_kprintf("create sem success\n\n");

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

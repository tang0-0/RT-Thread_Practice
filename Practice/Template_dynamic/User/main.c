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
static rt_thread_t receive0_thread = RT_NULL;
static rt_thread_t receive1_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
/* ����������ƿ� */
static rt_mailbox_t test_mail = RT_NULL;

char test_str1[] = "this is a mail test 1";/* ������Ϣtest1 */
char test_str2[] = "this is a mail test 2";/* ������Ϣtest2 */

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

static void receive0_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    char *r_str;
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
        /* �ȴ���������Ϣ */
        uwRet = rt_mb_recv(test_mail, /* ��������� */
                           (rt_uint32_t *)&r_str, /* ����������Ϣ */
                           0);/* ָ����ʱ�¼�,һֱ�� */

        if (RT_EOK == uwRet) /* ���������ɲ�����ȷ */
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
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
        /* �ȴ���������Ϣ */
        uwRet = rt_mb_recv(test_mail, /* ��������� */
                           (rt_uint32_t *)&r_str, /* ����������Ϣ */
                           0);/* ָ����ʱ�¼�,һֱ�� */

        if (RT_EOK == uwRet) /* ���������ɲ�����ȷ */
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
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
        //���KEY1������
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
        {
            rt_kprintf("KEY0\n");
            /* ����һ��������Ϣ1 */
            uwRet = rt_mb_send(test_mail, (rt_uint32_t)&test_str1);
            if (RT_EOK == uwRet)
                rt_kprintf("mb0 send success\n");
            else
                rt_kprintf("mb0 send failed\n");
        }
        //���KEY2������
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
        {
            rt_kprintf("KEY1\n");
            /* ����һ������2 */
            uwRet = rt_mb_send(test_mail, (rt_uint32_t)&test_str2);
            if (RT_EOK == uwRet)
                rt_kprintf("mb1 send success\n");
            else
                rt_kprintf("mb1 send failed\n");
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
    // led1_thread =                          /* �߳̿��ƿ�ָ�� */
    //     rt_thread_create("led1",               /* �߳����� */
    //                      led1_thread_entry,   /* �߳���ں��� */
    //                      RT_NULL,             /* �߳���ں������� */
    //                      512,                 /* �߳�ջ��С */
    //                      5,                   /* �̵߳����ȼ� */
    //                      20);                 /* �߳�ʱ��Ƭ */

    // /* �����̣߳��������� */
    // if (led1_thread != RT_NULL)
    //     rt_thread_startup(led1_thread);
    // else
    //     return -1;

    // led2_thread =                          /* �߳̿��ƿ�ָ�� */
    //     rt_thread_create("led2",               /* �߳����� */
    //                      led2_thread_entry,   /* �߳���ں��� */
    //                      RT_NULL,             /* �߳���ں������� */
    //                      512,                 /* �߳�ջ��С */
    //                      5,                   /* �̵߳����ȼ� */
    //                      20);                 /* �߳�ʱ��Ƭ */

    // /* �����̣߳��������� */
    // if (led2_thread != RT_NULL)
    //     rt_thread_startup(led2_thread);
    // else
    //     return -1;

    /* ����һ������ */
    test_mail = rt_mb_create("test_mail", /* �������� */
                             10,
                             RT_IPC_FLAG_FIFO);/* �ź���ģʽ FIFO(0x00)*/
    if (test_mail != RT_NULL)
        rt_kprintf("creat mb success\n\n");

    receive0_thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create("receive0",               /* �߳����� */
                         receive0_thread_entry,   /* �߳���ں��� */
                         RT_NULL,             /* �߳���ں������� */
                         512,                 /* �߳�ջ��С */
                         3,                   /* �̵߳����ȼ� */
                         20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (receive0_thread != RT_NULL)
        rt_thread_startup(receive0_thread);
    else
        return -1;
    
    receive1_thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create("receive1",               /* �߳����� */
                         receive1_thread_entry,   /* �߳���ں��� */
                         RT_NULL,             /* �߳���ں������� */
                         512,                 /* �߳�ջ��С */
                         3,                   /* �̵߳����ȼ� */
                         20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (receive1_thread != RT_NULL)
        rt_thread_startup(receive1_thread);
    else
        return -1;

    send_thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create("send",               /* �߳����� */
                         send_thread_entry,   /* �߳���ں��� */
                         RT_NULL,             /* �߳���ں������� */
                         512,                 /* �߳�ջ��С */
                         2,                   /* �̵߳����ȼ� */
                         20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (send_thread != RT_NULL)
        rt_thread_startup(send_thread);
    else
        return -1;



}


/********************************END OF FILE****************************/

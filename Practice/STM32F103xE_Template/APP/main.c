#include "board.h"
#include "rtthread.h"

/* 定义线程控制块指针 */
static rt_thread_t test_thread = RT_NULL;
/* 线程实现 */
static void test_thread_entry(void *parameter)
{
    while (1)
    {
		// rt_kprintf("test code!\n");
        LED1_TOGGLE;
        rt_thread_delay(1000);   /* 延时1000个tick */
    }
}

int main(void)
{	
	
	test_thread =                          /* 线程控制块指针 */
        rt_thread_create("test",               /* 线程名字 */
                         test_thread_entry,   /* 线程入口函数 */
                         RT_NULL,             /* 线程入口函数参数 */
                         512,                 /* 线程栈大小 */
                         3,                   /* 线程的优先级 */
                         20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (test_thread != RT_NULL)
        rt_thread_startup(test_thread);
    else
        return -1;
}


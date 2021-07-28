# RTT-互斥锁

## 概念

互斥锁可以将资源锁定，谁锁定谁开锁，锁定时其它线程无法再对该资源进行锁定。
当资源锁定时，该线程优先级会被提升到所有线程中优先级最高的线程同等级，以防发生优先级翻转情形。
当一个线程拿到互斥锁时，持有该互斥量的线程也能够再次获得这个锁而不被挂起，即递归访问。

## 接口

### 创建静态互斥锁
`rt_err_t 	rt_mutex_init (rt_mutex_t mutex, const char *name, rt_uint8_t flag)`

### 删除静态互斥锁
`rt_err_t 	rt_mutex_detach (rt_mutex_t mutex)`

### 创建动态互斥锁
`rt_mutex_t 	rt_mutex_create (const char *name, rt_uint8_t flag)`

### 删除动态互斥锁
`rt_err_t 	rt_mutex_delete (rt_mutex_t mutex)`

### 获取互斥锁
`rt_err_t 	rt_mutex_take (rt_mutex_t mutex, rt_int32_t time)`

### 释放互斥锁
`rt_err_t 	rt_mutex_release (rt_mutex_t mutex)`

## 用法

1. 定义互斥锁控制块
2. 创建互斥锁
3. 获取互斥锁
4. 释放互斥锁

```c
/* 定义线程控制块指针 */
static rt_thread_t key0_thread = RT_NULL;
static rt_thread_t key1_thread = RT_NULL;
/* 定义互斥量控制块 */
static rt_mutex_t test_mux = RT_NULL;

/* 创建互斥锁 */
test_mux = rt_mutex_create("test_mux", RT_IPC_FLAG_PRIO);
if (test_mux != RT_NULL)
rt_kprintf("creat mux success\n\n");

static void key0_thread_entry(void *parameter)
{
    while (1)
    {
        rt_mutex_take(test_mux,           /* 获取互斥量 */
                      RT_WAITING_FOREVER);    /* 等待时间：一直等 */
        ++num;
        rt_thread_delay(1000);
        rt_thread_delay(1000);
        rt_thread_delay(1000);

        rt_mutex_release(test_mux);    //释放互斥量

        rt_thread_delay(10);

    }

}

static void key1_thread_entry(void *parameter)
{
    while (1)
    {
        rt_mutex_take(test_mux,           /* 获取互斥量 */
                      RT_WAITING_FOREVER);    /* 等待时间：一直等 */

        rt_kprintf("num:%d\n", num);

        rt_mutex_release(test_mux);    //释放互斥量

        rt_thread_delay(10);

    }

}



```
# RTT-信号量

## 概念
信号量就是一个标记位，释放信号量就是标记位加一，获取信号量后自动减一，减到0后不能再获取了。
信号量（Semaphore）是一种实现线程间通信的机制，实现线程之间同步或临界资源的互斥访问，常用于协助一组相互竞争的线程来访问临界资源。在多线程系统中，各线程之间需要同步或互斥实现临界资源的保护，信号量功能可以为用户提供这方面的支持。

## 接口

### 创建静态信号量
rt_err_t 	rt_sem_init (rt_sem_t sem, const char *name, rt_uint32_t value, rt_uint8_t flag)

### 删除静态信号量
rt_err_t 	rt_sem_detach (rt_sem_t sem)

### 创建动态信号量
rt_sem_t 	rt_sem_create (const char *name, rt_uint32_t value, rt_uint8_t flag)

### 删除动态信号量
rt_err_t 	rt_sem_delete (rt_sem_t sem)

### 获取信号量
rt_err_t 	rt_sem_take (rt_sem_t sem, rt_int32_t time)

### 释放信号量
rt_err_t 	rt_sem_release (rt_sem_t sem)

## 用法

1. 定义信号量控制块
2. 创建信号量
3. 释放信号量
4. 获取信号量

```c
/* 按键释放信号量 */
static rt_thread_t key0_thread = RT_NULL;
static rt_thread_t key1_thread = RT_NULL;
/* 定义信号量控制块 */
static rt_sem_t test_sem = RT_NULL;

/* 创建一个信号量 */
test_sem = rt_sem_create("test_sem",/* 信号量名字 */
                         5,     /* 信号量初始值 */
                         RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
if (test_sem != RT_NULL)
   rt_kprintf("create sem success\n\n");

/* 定义信号量控制块 */
static void key0_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;

    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)/* K1 被按下 */
        {
            uwRet = rt_sem_release(test_sem);     //释放信号量
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

/* 按键获取信号量 */
static void key1_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    while (1)
    {
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)       //如果KEY2被单击
        {
            /* 获取一个计数信号量 */
            uwRet = rt_sem_take(test_sem, 0);  /* 获取信号量 */
            if (RT_EOK == uwRet)
                rt_kprintf("take one sem\r\n");
            else
                rt_kprintf("up\r\n");

        }
        rt_thread_delay(20);     //每20ms扫描一次

    }

}


```

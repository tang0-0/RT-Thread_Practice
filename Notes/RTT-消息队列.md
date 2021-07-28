# RTT-消息队列

>参考：《[野火®]《RT-Thread 内核实现与应用开发实战—基于STM32》

## 概念

消息队列就是一个线程给另一个线程发消息，收到消息就处理，没收到就挂起。
用户还可以指定挂起的线程时间 timeout；当队列中有新消息时，挂起的读取线程被唤醒并处理新消息。
当有多个消息发送到消息队列时，通常是将先进入消息队列的消息先传给线程，，即先进先出原则(FIFO)。
同时 RT-Thread 中的消息队列支持优先级，也就是说在所有等待消息的线程中优先级最高的会先获得消息。
内部的实现，是将消息内容从一个地址复制到另一个地址。

## 接口

### 创建静态消息队列
`rt_err_t 	rt_mq_init (rt_mq_t mq, const char *name, void *msgpool, rt_size_t msg_size, rt_size_t pool_size, rt_uint8_t flag)`

### 删除静态消息队列
`rt_err_t 	rt_mq_detach (rt_mq_t mq)`

### 创建动态消息队列
`rt_mq_t 	rt_mq_create (const char *name, rt_size_t msg_size, rt_size_t max_msgs, rt_uint8_t flag)`

### 删除动态消息队列
`rt_err_t 	rt_mq_delete (rt_mq_t mq)`

### 发送消息
`rt_err_t 	rt_mq_send (rt_mq_t mq, void *buffer, rt_size_t size)`

### 发送紧急消息
`rt_err_t 	rt_mq_urgent (rt_mq_t mq, void *buffer, rt_size_t size)`

### 接收消息
`rt_err_t 	rt_mq_recv (rt_mq_t mq, void *buffer, rt_size_t size, rt_int32_t timeout)`

## 用法

1. 定义消息队列控制块
2. 创建消息
3. 发送消息
4. 接收消息

```c
/* 定义线程控制块指针 */
static rt_thread_t key0_thread = RT_NULL;
static rt_thread_t receive_thread = RT_NULL;
/* 定义消息队列控制块 */
static rt_mq_t test_mq = RT_NULL;

/* 创建消息队列 */
test_mq = rt_mq_create("test_mq",/* 消息队列名字 */
                           4,     /* 消息的最大长度 */
                           20,    /* 消息队列的最大容量 */
                           RT_IPC_FLAG_FIFO);/* 队列模式 FIFO(0x00)*/
if (test_mq != RT_NULL)
    rt_kprintf("creat message line success\r\n");

/* 按键发送消息 */
static void key0_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    uint32_t send_data1 = 1;
    uint32_t send_data2 = 2;
    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)/* K1 被按下 */
        {
            /* 将数据写入（发送）到队列中，等待时间为 0  */
            uwRet = rt_mq_send(test_mq,  /* 写入（发送）队列的ID(句柄) */
                               &send_data1,            /* 写入（发送）的数据 */
                               sizeof(send_data1));            /* 数据的长度 */
            if (RT_EOK != uwRet)
            {
                rt_kprintf("send1 failed:%lx\n", uwRet);
            }
        }
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)/* K1 被按下 */
        {
            /* 将数据写入（发送）到队列中，等待时间为 0  */
            uwRet = rt_mq_send(test_mq,  /* 写入（发送）队列的ID(句柄) */
                               &send_data2,            /* 写入（发送）的数据 */
                               sizeof(send_data2));            /* 数据的长度 */
            if (RT_EOK != uwRet)
            {
                rt_kprintf("send2 failed:%lx\n", uwRet);
            }
        }
        rt_thread_delay(20);
    }


}

/* 接收消息 */
static void receive_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    uint32_t r_queue;
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        /* 队列读取（接收），等待时间为一直等待 */
        uwRet = rt_mq_recv(test_mq, /* 读取（接收）队列的ID(句柄) */
                           &r_queue,           /* 读取（接收）的数据保存位置 */
                           sizeof(r_queue),        /* 读取（接收）的数据的长度 */
                           RT_WAITING_FOREVER);    /* 等待时间：一直等 */
        if (RT_EOK == uwRet)
        {
            rt_kprintf("rev:%d\n", r_queue);
        }
        else
        {
            rt_kprintf("rev failed:0x%lx\n", uwRet);
        }
        rt_thread_delay(200);
    }
}

```
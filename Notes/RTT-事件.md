# RTT-事件

## 概念
事件和信号量类似，也是进程间的标志位
这是一个32位的标志位，每个bit位都可以用作一个标志

## 接口

### 创建静态事件
`rt_err_t 	rt_event_init (rt_event_t event, const char *name, rt_uint8_t flag)`

### 删除静态事件
`rt_err_t 	rt_event_detach (rt_event_t event)`

### 创建动态事件
`rt_event_t 	rt_event_create (const char *name, rt_uint8_t flag)`

### 删除动态事件
`rt_err_t 	rt_event_delete (rt_event_t event)`

### 发送事件
`rt_err_t 	rt_event_send (rt_event_t event, rt_uint32_t set)`

### 接收事件
`rt_err_t 	rt_event_recv (rt_event_t event, rt_uint32_t set, rt_uint8_t option, rt_int32_t timeout, rt_uint32_t *recved)`

## 用法

1. 定义事件控制块
2. 创建事件对象
3. 发送事件
4. 接收事件

```c
/* 定义线程控制块指针 */
static rt_thread_t key0_thread = RT_NULL;
static rt_thread_t handler_thread = RT_NULL;
/* 定义事件控制块 */
static rt_event_t test_event = RT_NULL;
/* 定义事件标志位 */
#define KEY1_EVENT  (0x01 << 0)//设置事件掩码的位0
#define KEY2_EVENT  (0x01 << 1)//设置事件掩码的位1

/* 按键发送事件 */
test_event = rt_event_create("test_event",/* 事件标志组名字 */
                                 RT_IPC_FLAG_PRIO); /* 事件模式 FIFO(0x00)*/
if (test_event != RT_NULL)
	rt_kprintf("creat event success\r\n");

/* 创建事件对象 */
static void key0_thread_entry(void *parameter)
{
    while (1)
    {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)       //如果KEY2被单击
        {
            rt_kprintf("KEY1 pressed\n");
            /* 发送一个事件1 */
            rt_event_send(test_event, KEY1_EVENT);
        }

        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)       //如果KEY2被单击
        {
            rt_kprintf("KEY2 pressed\n");
            /* 发送一个事件2 */
            rt_event_send(test_event, KEY2_EVENT);
        }
        rt_thread_delay(20);     //每20ms扫描一次
    }

}

/* 接收处理事件 */
static void handler_thread_entry(void *parameter)
{
    rt_uint32_t recved;

    while (1)
    {
        /* 等待接收事件标志 */
        rt_event_recv(test_event,  /* 事件对象句柄 */
                      KEY1_EVENT | KEY2_EVENT, /* 接收线程感兴趣的事件 */
                      RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, /* 接收选项 */
                      RT_WAITING_FOREVER,/* 指定超时事件,一直等 */
                      &recved);    /* 指向接收到的事件 */
        if (recved == (KEY1_EVENT | KEY2_EVENT)) /* 如果接收完成并且正确 */
        {
            rt_kprintf("Key1 and Key2\n");
        }
        else
            rt_kprintf("failed\n");

    }

}


```
# RTT-邮箱

## 概念

邮箱是特别的消息队列
邮箱中每个邮件的长度固定，是4个字节
如果是大的buff，直接发送地址

## 接口

### 创建静态邮箱
`rt_err_t 	rt_mb_init (rt_mailbox_t mb, const char *name, void *msgpool, rt_size_t size, rt_uint8_t flag)`

### 删除静态邮箱
`rt_err_t 	rt_mb_detach (rt_mailbox_t mb)`

### 创建动态邮箱
`rt_mailbox_t 	rt_mb_create (const char *name, rt_size_t size, rt_uint8_t flag)`

### 删除动态邮箱
`rt_err_t 	rt_mb_delete (rt_mailbox_t mb)`

### 发送邮件
`rt_err_t 	rt_mb_send (rt_mailbox_t mb, rt_uint32_t value)`

### 接收邮件
`rt_err_t 	rt_mb_recv (rt_mailbox_t mb, rt_uint32_t *value, rt_int32_t timeout)`

## 用法

1. 定义邮箱控制块
2. 创建邮箱对象
3. 发送邮件
4. 接收邮件

```c
/* 定义线程控制块指针 */
static rt_thread_t receive0_thread = RT_NULL;
static rt_thread_t receive1_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
/* 定义邮箱控制块 */
static rt_mailbox_t test_mail = RT_NULL;

char test_str1[] = "this is a mail test 1";/* 邮箱消息test1 */
char test_str2[] = "this is a mail test 2";/* 邮箱消息test2 */

/* 创建一个邮箱 */
    test_mail = rt_mb_create("test_mail", /* 邮箱名字 */
                             10,
                             RT_IPC_FLAG_FIFO);/* 信号量模式 FIFO(0x00)*/
if (test_mail != RT_NULL)
	rt_kprintf("creat mb success\n\n");

static void receive0_thread_entry(void *parameter)
{
    rt_err_t uwRet = RT_EOK;
    char *r_str;
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        /* 等待接邮箱消息 */
        uwRet = rt_mb_recv(test_mail, /* 邮箱对象句柄 */
                           (rt_uint32_t *)&r_str, /* 接收邮箱消息 */
                           0);/* 指定超时事件,一直等 */

        if (RT_EOK == uwRet) /* 如果接收完成并且正确 */
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
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        /* 等待接邮箱消息 */
        uwRet = rt_mb_recv(test_mail, /* 邮箱对象句柄 */
                           (rt_uint32_t *)&r_str, /* 接收邮箱消息 */
                           0);/* 指定超时事件,一直等 */

        if (RT_EOK == uwRet) /* 如果接收完成并且正确 */
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
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        //如果KEY1被单击
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
        {
            rt_kprintf("KEY0\n");
            /* 发送一个邮箱消息1 */
            uwRet = rt_mb_send(test_mail, (rt_uint32_t)&test_str1);
            if (RT_EOK == uwRet)
                rt_kprintf("mb0 send success\n");
            else
                rt_kprintf("mb0 send failed\n");
        }
        //如果KEY2被单击
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
        {
            rt_kprintf("KEY1\n");
            /* 发送一个邮箱2 */
            uwRet = rt_mb_send(test_mail, (rt_uint32_t)&test_str2);
            if (RT_EOK == uwRet)
                rt_kprintf("mb1 send success\n");
            else
                rt_kprintf("mb1 send failed\n");
        }
        rt_thread_delay(20);     //每20ms扫描一次
    }
}

```
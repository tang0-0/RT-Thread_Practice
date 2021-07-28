# RTT-软件定时器

## 概念

RTT的软件定时器是利用systick里的计数来实现的
有两种方式，一种是创建一个线程然后在线程里调用回调函数，另一种是直接在systick的中断里调用回调函数
有两种模式，一种是只使用一次，另一种是周期使用

## 接口

### 创建静态定时器
`void 	rt_timer_init (rt_timer_t timer, const char *name, void(*timeout)(void *parameter), void *parameter, rt_tick_t time, rt_uint8_t flag)`

### 删除静态定时器
`rt_err_t 	rt_timer_detach (rt_timer_t timer)`

### 创建动态定时器
`rt_timer_t 	rt_timer_create (const char *name, void(*timeout)(void *parameter), void *parameter, rt_tick_t time, rt_uint8_t flag)`

### 删除动态定时器
`rt_err_t 	rt_timer_delete (rt_timer_t timer)`

### 启动定时器
`rt_err_t 	rt_timer_start (rt_timer_t timer)`

### 停止定时器
`rt_err_t 	rt_timer_stop (rt_timer_t timer)`

### 设置定时器
`rt_err_t 	rt_timer_control (rt_timer_t timer, int cmd, void *arg)`

## 用法

1. 定义软件定时器控制块
2. 定义超时回调函数
3. 创建定时器
4. 启动定时器

```c
    /* 定义线软件定时器制块 */
    static rt_timer_t swtmr1 = RT_NULL;
    static rt_timer_t swtmr2 = RT_NULL;

    static uint32_t TmrCb_Count1 = 0;
    static uint32_t TmrCb_Count2 = 0;

    static void swtmr1_callback(void *parameter)
    {
        uint32_t tick_num1;

        TmrCb_Count1++;                     /* 每回调一次加一 */

        tick_num1 = (uint32_t)rt_tick_get();    /* 获取滴答定时器的计数值 */

        rt_kprintf("swtmr1_callback: %d\n", TmrCb_Count1);
        rt_kprintf("systick=%d\n", tick_num1);

    }


    static void swtmr2_callback(void *parameter)
    {
        uint32_t tick_num2;

        TmrCb_Count2++;             /* 每回调一次加一 */

        tick_num2 = (uint32_t)rt_tick_get();    /* 获取滴答定时器的计数值 */

        rt_kprintf("swtmr2_callback:%d\n", TmrCb_Count2);

        rt_kprintf("systick=%d\n", tick_num2);

    }

	/* 创建一个软件定时器 */
    swtmr1 = rt_timer_create("swtmr1_callback", /* 软件定时器的名称 */
                             swtmr1_callback,/* 软件定时器的回调函数 */
                             RT_NULL,          /* 定时器超时函数的入口参数 */
                             5000,   /* 软件定时器的超时时间(周期回调时间) */
                             RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
    /* 软件定时器模式 一次模式 */
    /* 启动定时器 */
    if (swtmr1 != RT_NULL)
        rt_timer_start(swtmr1);

    /* 创建一个软件定时器 */
    swtmr2 = rt_timer_create("swtmr2_callback", /* 软件定时器的名称 */
                             swtmr2_callback,/* 软件定时器的回调函数 */
                             RT_NULL,          /* 定时器超时函数的入口参数 */
                             1000,   /* 软件定时器的超时时间(周期回调时间) */
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER);
    /* 软件定时器模式 周期模式 */
    /* 启动定时器 */
    if (swtmr2 != RT_NULL)
        rt_timer_start(swtmr2);


```
# RTT-线程管理

>官方API文档：https://www.rt-thread.org/document/api/group___thread.html


![](RTT-%E7%BA%BF%E7%A8%8B%E7%AE%A1%E7%90%86.assets/%E7%BA%BF%E7%A8%8B.jpg)

## 概念
线程是竞争系统资源的最小运行单元。每个线程在自己的环境中运行。在任何时刻，只有一个线程得到运行，RT-Thread 调度器决定运行哪个线程。调度器会不断启动、停止每一个线程，宏观看上去所有的线程都在同时在执行。作为线程，不需要对调度器的活动有所了解，在线程切入切出时保存上下文环境（寄存器值、堆栈内容）是调度器主要的职责。为了实现这点，每个 RT-Thread 线程都需要有自己的堆栈。当线程切出时，它的执行环境会被保存在该线程的堆栈中，这样当线程再次运行时，就能从堆栈中正确的恢复上次的运行环境。
RT-Thread 中的线程是抢占式调度机制，同时支持时间片轮转调度方式。高优先级的线程可打断低优先级线程，低优先级线程必须在高优先级线程阻塞或结束后才能得到调度。同优先级的任务按照时间片轮转调度。

## 接口

### 创建静态线程
`rt_err_t 	rt_thread_init (struct rt_thread *thread, const char *name, void(*entry)(void *parameter), void *parameter, void *stack_start, rt_uint32_t stack_size, rt_uint8_t priority, rt_uint32_t tick)`

### 删除静态线程
`rt_err_t 	rt_thread_detach (rt_thread_t thread)`

### 创建动态线程
`rt_thread_t 	rt_thread_create (const char *name, void(*entry)(void *parameter), void *parameter, rt_uint32_t stack_size, rt_uint8_t priority, rt_uint32_t tick)`

### 删除动态线程
`rt_err_t 	rt_thread_delete (rt_thread_t thread)`

### 启动线程
`rt_err_t 	rt_thread_startup (rt_thread_t thread)`

### 睡眠线程
`rt_err_t 	rt_thread_sleep (rt_tick_t tick)`

### 挂起线程
`rt_err_t 	rt_thread_suspend (rt_thread_t thread)`

### 恢复线程
`rt_err_t 	rt_thread_resume (rt_thread_t thread)`

### 获取当前线程对象指针
`rt_thread_t 	rt_thread_self (void)`

### 修改线程配置参数
`rt_err_t 	rt_thread_control (rt_thread_t thread, int cmd, void *arg)`

### 让出资源
`rt_err_t rt_thread_yield	(void )`
该函数将使当前线程出让处理器资源，它将不再占有处理器，调度器会选择相同优先级的下一个线程执行。 线程调用这个接口后，这个线程仍然在就绪队列中。

## 用法

### 静态创建
1. 定义线程栈
2. 定义线程控制块
3. 定义线程函数
4. 创建静态线程
5. 启动线程

### 动态创建
1. 分配一部分RAM空间来作为 RT-Thread 的堆空间
2. 定义线程控制块
3. 定义线程函数
4. 创建动态线程
5. 启动线程
/**
 * @File Name: log.h
 * @Brief : 日志打印组件,支持显示颜色，支持时间戳，支持显示调用的文件名
 * @Author : ty (tangt_t@foxmail.com)
 * @Version : 1.0
 * @Creat Date : 2021-06-12
 */
#ifndef __LOG_H__
#define __LOG_H__

#ifndef LOG_TAG 
#define LOG_TAG                "LOG"
#endif

// 相关设置
#define MAX_SIZE_LOG_BUFF           1024
#define DISPLAY_TIMESTAMP_ENABLE    1
#define DISPLAY_COLOR_ENABLE        1
#define DISPLAY_TAG_ENABLE          1

#define LOG_COLOR_DEBUG               (F_WHITE)
#define LOG_COLOR_INFO                (F_GREEN)
#define LOG_COLOR_WARN                (F_YELLOW)
#define LOG_COLOR_ERROR               (F_RED)
#define LOG_COLOR_ASSERT              (F_MAGENTA)

#define LOG_LVL_ASSERT                 0
#define LOG_LVL_ERROR                  1
#define LOG_LVL_WARNING                2
#define LOG_LVL_INFO                   3
#define LOG_LVL_DBG                    4


// 初始化
void log_init(void *uart);
// 开关日志
void log_switch(unsigned char en);
// 返回日志是否打开
unsigned char log_get_mode(void);
// 日志输出
void log_output(unsigned char level, const char *tag, const char *format, ...);
void LOG(const char *format, ...);
#define LOG_D(format,...)  log_output(LOG_LVL_DBG,     LOG_TAG, format, ##__VA_ARGS__)
#define LOG_I(format,...)  log_output(LOG_LVL_INFO,    LOG_TAG, format, ##__VA_ARGS__)
#define LOG_W(format,...)  log_output(LOG_LVL_WARNING, LOG_TAG, format, ##__VA_ARGS__)
#define LOG_E(format,...)  log_output(LOG_LVL_ERROR,   LOG_TAG, format, ##__VA_ARGS__)
#define LOG_A(format,...)  log_output(LOG_LVL_ASSERT,  LOG_TAG, format, ##__VA_ARGS__)


#endif


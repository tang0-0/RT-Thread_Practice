#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "log.h"
#include "stm32f10x.h"
#include "bsp_rtc.h"
#include "bsp_uart.h"

#define CSI_START                      "\033["
#define CSI_END                        "\033[0m"
/* output log front color */
#define F_BLACK                        "30m"
#define F_RED                          "31m"
#define F_GREEN                        "32m"
#define F_YELLOW                       "33m"
#define F_BLUE                         "34m"
#define F_MAGENTA                      "35m"
#define F_CYAN                         "36m"
#define F_WHITE                        "37m"

static char log_buff[MAX_SIZE_LOG_BUFF] = {0};
static unsigned char log_enable = 0;
USART_TypeDef *log_uart = NULL;
static const char color_output_info[][4] =
{
    LOG_COLOR_ASSERT,
    LOG_COLOR_ERROR,
    LOG_COLOR_WARN,
    LOG_COLOR_INFO,
    LOG_COLOR_DEBUG,
};


void log_output(uint8_t level, const char *tag, const char *format, ...)
{
    va_list args;
    char *p = log_buff;

    if (log_enable == 0)
    {

        return;
    }

    p[0] = '\0';

#if (DISPLAY_COLOR_ENABLE)
    sprintf(p, "%s%s", CSI_START,  color_output_info[level]);
    p = log_buff + strlen(log_buff);
#endif

#if (DISPLAY_TIMESTAMP_ENABLE)
    sprintf(p, "[");
    get_timestamp(p + 1, 20);
    p = log_buff + strlen(log_buff);
    sprintf(p, "]");
    p += 1 ;
#endif

#if (DISPLAY_TAG_ENABLE)
    if (tag != NULL)
    {
        sprintf(p, "<%s>", tag);
    }
    p = log_buff + strlen(log_buff);
#endif

    va_start(args, format);
    vsprintf(p, format, args);
    va_end(args);
    p = log_buff + strlen(log_buff);

#if (DISPLAY_COLOR_ENABLE)
    sprintf(p, "%s", CSI_END);
    p = log_buff + strlen(log_buff);
#endif

    sprintf(p, "\r\n");

    uart_sendbytes(log_uart, log_buff, strlen(log_buff));
}

void LOG(const char *format, ...)
{
    va_list args;
    char *p = log_buff;

    if (log_enable == 0)
    {

        return;
    }

    p[0] = '\0';

    va_start(args, format);
    vsprintf(p, format, args);
    va_end(args);
    p += strlen(log_buff);

    sprintf(p, "\r\n");

    uart_sendbytes(log_uart, log_buff, strlen(log_buff));

}


void log_init(void *uart)
{
    log_uart = (USART_TypeDef *)uart;
    log_switch(1);
}

void log_switch(unsigned char en)
{
    log_enable = en;
}

uint8_t log_get_mode(void)
{
    return log_enable;
}



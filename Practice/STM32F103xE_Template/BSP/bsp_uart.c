#include <stm32f10x.h>
#include "bsp_uart.h"

// function point
typedef void (*uart_clkcmd_t)(uint32_t, FunctionalState);
typedef void (*gpio_clkcmd_t)(uint32_t, FunctionalState);
typedef void (*nvic_config_t)(void);

// function declaration
static void nvic1_config(void);
static void nvic2_config(void);
static void nvic3_config(void);
static void nvic4_config(void);
static void nvic5_config(void);

// static variable
typedef struct
{
    uint16_t uart_tx_pin;
    uint16_t uart_rx_pin;

    uint32_t uart_clk;//串口时钟源
    uint32_t gpio_clk;//收发引脚时钟源

    USART_TypeDef *uart;
    GPIO_TypeDef *uart_rx_port;
    GPIO_TypeDef *uart_tx_port;

    uart_clkcmd_t uart_clkcmd;//串口时钟使能函数
    gpio_clkcmd_t gpio_clkcmd;//引脚时钟使能函数

    nvic_config_t nvic_config;//串口中断设置函数

} uart_args_t;

static uart_args_t uart_list[] =
{
    {
        .uart =         USART1,
        .uart_clk =     RCC_APB2Periph_USART1,
        .uart_clkcmd =  RCC_APB2PeriphClockCmd,
        .gpio_clk =     RCC_APB2Periph_GPIOA,
        .gpio_clkcmd =  RCC_APB2PeriphClockCmd,

        .uart_tx_port = GPIOA,
        .uart_tx_pin =  GPIO_Pin_9,
        .uart_rx_port = GPIOA,
        .uart_rx_pin =  GPIO_Pin_10,

        .nvic_config =  nvic1_config,

    },
    {
        .uart =         USART2,
        .uart_clk =     RCC_APB1Periph_USART2,
        .uart_clkcmd =  RCC_APB1PeriphClockCmd,
        .gpio_clk =     RCC_APB2Periph_GPIOA,
        .gpio_clkcmd =  RCC_APB2PeriphClockCmd,

        .uart_tx_port = GPIOA,
        .uart_tx_pin =  GPIO_Pin_2,
        .uart_rx_port = GPIOA,
        .uart_rx_pin =  GPIO_Pin_3,

        .nvic_config =  nvic2_config,

    },
    {
        .uart =         USART3,
        .uart_clk =     RCC_APB1Periph_USART3,
        .uart_clkcmd =  RCC_APB1PeriphClockCmd,
        .gpio_clk =     RCC_APB2Periph_GPIOB,
        .gpio_clkcmd =  RCC_APB2PeriphClockCmd,

        .uart_tx_port = GPIOB,
        .uart_tx_pin =  GPIO_Pin_10,
        .uart_rx_port = GPIOA,
        .uart_rx_pin =  GPIO_Pin_11,

        .nvic_config =  nvic3_config,

    },
    {
        .uart =         UART4,
        .uart_clk =     RCC_APB1Periph_UART4,
        .uart_clkcmd =  RCC_APB1PeriphClockCmd,
        .gpio_clk =     RCC_APB2Periph_GPIOC,
        .gpio_clkcmd =  RCC_APB2PeriphClockCmd,

        .uart_tx_port = GPIOC,
        .uart_tx_pin =  GPIO_Pin_10,
        .uart_rx_port = GPIOA,
        .uart_rx_pin =  GPIO_Pin_11,

        .nvic_config =  nvic4_config,

    },
    {
        .uart =         UART5,
        .uart_clk =     RCC_APB1Periph_UART5,
        .uart_clkcmd =  RCC_APB1PeriphClockCmd,
        .gpio_clk = (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD),
        .gpio_clkcmd =  RCC_APB2PeriphClockCmd,

        .uart_tx_port = GPIOC,
        .uart_tx_pin =  GPIO_Pin_12,
        .uart_rx_port = GPIOD,
        .uart_rx_pin =  GPIO_Pin_2,

        .nvic_config =  nvic5_config,

    },


};

// 将串口重定向到"printf()",注意要在MDK中开启MicroLIB
#ifdef USING_PRINTF
int fputc(int ch, FILE *f)
{
    USART_SendData(USING_PRINTF, (uint8_t) ch);

    while (USART_GetFlagStatus(USING_PRINTF, USART_FLAG_TXE) == RESET);

    return (ch);
}

int fgetc(FILE *f)
{
    while (USART_GetFlagStatus(USING_PRINTF, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(USING_PRINTF);
}

#endif


/************************************************** Function Definition **************************************************************/
// static function
static void nvic1_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能串口中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
}
static void nvic2_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能串口中断
    // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
}
static void nvic3_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能串口中断
    // USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
}
static void nvic4_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能串口中断
    // USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
}
static void nvic5_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能串口中断
    // USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);
}


void uart_init(void *uartx, uint32_t baudrate, uint16_t parity, uint8_t nvic_enable)
{
    uint8_t dev;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    if (uartx == USART1)
        dev = 0;
    else if (uartx == USART2)
        dev = 1;
    else if (uartx == USART3)
        dev = 2;
    else if (uartx == UART4)
        dev = 3;
    else if (uartx == UART5)
        dev = 4;
    else
        dev = 0;

    if (parity == 0)
        parity = USART_Parity_No;
    else if (parity == 1)
        parity = USART_Parity_Odd;
    else if (parity == 1)
        parity = USART_Parity_Even;
    else
        parity = USART_Parity_No;


    uart_list[dev].uart_clkcmd(uart_list[dev].uart_clk, ENABLE);
    uart_list[dev].gpio_clkcmd(uart_list[dev].gpio_clk, ENABLE);

    GPIO_InitStructure.GPIO_Pin = uart_list[dev].uart_tx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(uart_list[dev].uart_tx_port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = uart_list[dev].uart_rx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(uart_list[dev].uart_rx_port, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = parity ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init((USART_TypeDef *)uartx, &USART_InitStructure);

    USART_Cmd((USART_TypeDef *)uartx, ENABLE);

    if (nvic_enable)
    {
        uart_list[dev].nvic_config();
    }

}

void single_uart_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
    DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(DEBUG_USARTx, &USART_InitStructure);

    // nvic1_config();
    // USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, ENABLE);

    USART_Cmd(DEBUG_USARTx, ENABLE);
}

void uart_sendbytes(void *pUSARTx, char *str, uint16_t size)
{
    for (int i = 0; i < size; i++)
    {
        USART_SendData(pUSARTx, str[i]);
        while (USART_GetFlagStatus((USART_TypeDef *)pUSARTx, USART_FLAG_TXE) == RESET);
    }

    /* 等待发送完成 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);

}


// example
// void USART1_IRQHandler(void)
// {
//     uint8_t tmp;
//     if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//     {
//         tmp = USART_ReceiveData(USART1);
//         USART_SendData(USART1, tmp);
//     }

// }






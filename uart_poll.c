#include <string.h>

#include "clock.h"
#include "config.h"
#include "gpio.h"

#if !defined(LED1_ON)
#define LED1_OFF    0
#define LED1_ON     1
#endif

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_clock_enable(LED1_PORT);
    GPIO_InitStruct.Pin = LED1_PIN;
    HAL_GPIO_Init(LED1_PORT, &GPIO_InitStruct);

#if defined(UART4_TX_PORT)

    __HAL_RCC_UART4_CLK_ENABLE();

    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = UART4_AF;

    GPIO_clock_enable(UART4_TX_PORT);
    GPIO_InitStruct.Pin = UART4_TX_PIN;
    HAL_GPIO_Init(UART4_TX_PORT, &GPIO_InitStruct);

    GPIO_clock_enable(UART4_RX_PORT);
    GPIO_InitStruct.Pin = UART4_RX_PIN;
    HAL_GPIO_Init(UART4_RX_PORT, &GPIO_InitStruct);

    UART_HandleTypeDef huart;
    memset(&huart, 0, sizeof(huart));
    huart.Instance = UART4;
    huart.Init.BaudRate = 115200;
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.Parity = UART_PARITY_NONE;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart);

#endif
    
    const char *str = "\r\n===== Starting =====\r\n";
    HAL_UART_Transmit(&huart, (uint8_t *)str, strlen(str), 100);

    HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_ON);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_OFF);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_ON);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_OFF);

    while (1)
    {
        if (__HAL_USART_GET_FLAG(&huart, USART_FLAG_RXNE)) {
            uint8_t ch;

            if (HAL_UART_Receive(&huart, &ch, 1, 0) == HAL_OK) {
                if ((ch >= 0x40) && (ch <= 0x7f)) {
                    ch ^= 0x20;
                }
                HAL_UART_Transmit(&huart, &ch, 1, 0);

                HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_ON);
                HAL_Delay(10);
                HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_OFF);
            }
        }
    }
}

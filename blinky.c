#include "clock.h"
#include "config.h"

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = LED1_PIN;
    HAL_GPIO_Init(LED1_PORT, &GPIO_InitStruct);

    while (1)
    {
        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, 1);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, 0);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, 1);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, 0);
        HAL_Delay(700);
    }
}

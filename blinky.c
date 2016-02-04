#include "clock.h"
#include "config.h"
#include "gpio.h"

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_clock_enable(LED1_PORT);
    GPIO_InitStruct.Pin = LED1_PIN;
    HAL_GPIO_Init(LED1_PORT, &GPIO_InitStruct);
    
#if defined(LED2_PORT)
    GPIO_clock_enable(LED2_PORT);
    GPIO_InitStruct.Pin = LED2_PIN;
    HAL_GPIO_Init(LED2_PORT, &GPIO_InitStruct);
#endif
    
#if defined(LED3_PORT)
    GPIO_clock_enable(LED3_PORT);
    GPIO_InitStruct.Pin = LED3_PIN;
    HAL_GPIO_Init(LED3_PORT, &GPIO_InitStruct);
#endif
    
#if defined(LED4_PORT)
    GPIO_clock_enable(LED4_PORT);
    GPIO_InitStruct.Pin = LED4_PIN;
    HAL_GPIO_Init(LED4_PORT, &GPIO_InitStruct);
#endif

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

#if defined(LED2_PORT)
        HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, 1);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, 0);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, 1);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, 0);
        HAL_Delay(700);
#endif

#if defined(LED3_PORT)
        HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, 1);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, 0);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, 1);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, 0);
        HAL_Delay(700);
#endif

#if defined(LED4_PORT)
        HAL_GPIO_WritePin(LED4_PORT, LED4_PIN, 1);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED4_PORT, LED4_PIN, 0);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED4_PORT, LED4_PIN, 1);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED4_PORT, LED4_PIN, 0);
        HAL_Delay(700);
#endif
    }
}

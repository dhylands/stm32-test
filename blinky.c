#include "clock.h"
#include "config.h"
#include "gpio.h"

#if !defined(LED1_ON)
#define LED1_OFF    0
#define LED1_ON     1
#endif

#if !defined(LED2_ON)
#define LED2_OFF    0
#define LED2_ON     1
#endif

#if !defined(LED3_ON)
#define LED3_OFF    0
#define LED3_ON     1
#endif

#if !defined(LED4_ON)
#define LED4_OFF    0
#define LED4_ON     1
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
        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_ON);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_OFF);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_ON);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_OFF);
        HAL_Delay(700);

#if defined(LED2_PORT)
        HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, LED2_ON);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, LED2_OFF);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, LED2_ON);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, LED2_OFF);
        HAL_Delay(700);
#endif

#if defined(LED3_PORT)
        HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, LED3_ON);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, LED3_OFF);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, LED3_ON);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, LED3_OFF);
        HAL_Delay(700);
#endif

#if defined(LED4_PORT)
        HAL_GPIO_WritePin(LED4_PORT, LED4_PIN, LED4_ON);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED4_PORT, LED4_PIN, LED4_OFF);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED4_PORT, LED4_PIN, LED4_ON);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED4_PORT, LED4_PIN, LED4_OFF);
        HAL_Delay(700);
#endif
    }
}

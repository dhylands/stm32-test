#include <stm32f4xx_hal_conf.h>
#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>

#define LED1_PORT     GPIOA
#define LED1_PIN      GPIO_PIN_5

#define SW_PORT       GPIOC
#define SW_PIN        GPIO_PIN_13

#define UART1_AF      GPIO_AF7_USART1

#define UART1_TX_PORT GPIOA
#define UART1_TX_PIN  GPIO_PIN_9

#define UART1_RX_PORT GPIOA
#define UART1_RX_PIN  GPIO_PIN_10

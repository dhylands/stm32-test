#include <stm32f4xx_hal_conf.h>
#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>

#define LED1_PORT     GPIOA
#define LED1_PIN      GPIO_PIN_8

#define SW_PORT       GPIOC
#define SW_PIN        GPIO_PIN_1

#define UART4_AF      GPIO_AF8_UART4

#define UART4_TX_PORT GPIOC
#define UART4_TX_PIN  GPIO_PIN_10

#define UART4_RX_PORT GPIOC
#define UART4_RX_PIN  GPIO_PIN_11

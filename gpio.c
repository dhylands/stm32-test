#include "config.h"

void GPIO_clock_enable(GPIO_TypeDef *port)
{
    if (0) {
#ifdef GPIOA
    } else if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
#endif
#ifdef GPIOB
    } else if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
#endif
#ifdef GPIOC
    } else if (port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
#endif
#ifdef GPIOD
    } else if (port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
#endif
#ifdef GPIOE
    } else if (port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
#endif
#ifdef GPIOF
    } else if (port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
#endif
#ifdef GPIOG
    } else if (port == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE();
#endif
#ifdef GPIOH
    } else if (port == GPIOH) {
        __HAL_RCC_GPIOH_CLK_ENABLE();
#endif
#ifdef GPIOI
    } else if (port == GPIOI) {
        __HAL_RCC_GPIOI_CLK_ENABLE();
#endif
#ifdef GPIOJ
    } else if (port == GPIOJ) {
        __HAL_RCC_GPIOJ_CLK_ENABLE();
#endif
#ifdef GPIOK
    } else if (port == GPIOK) {
        __HAL_RCC_GPIOK_CLK_ENABLE();
#endif
    }
}

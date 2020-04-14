/**
  ******************************************************************************
  * @file    stm32f4xx_hal_wwdg.c
  * @author  MCD Application Team
  * @brief   WWDG HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Window Watchdog (WWDG) peripheral:
  *           + Initialization and Configuration functions
  *           + IO operation functions
  @verbatim
  ==============================================================================
                      ##### WWDG Specific features #####
  ==============================================================================
  [..]
    Once enabled the WWDG generates a system reset on expiry of a programmed
    time period, unless the program refreshes the counter (T[6;0] downcounter)
    before reaching 0x3F value (i.e. a reset is generated when the counter
    value rolls down from 0x40 to 0x3F).

    (+) An MCU reset is also generated if the counter value is refreshed
        before the counter has reached the refresh window value. This
        implies that the counter must be refreshed in a limited window.
    (+) Once enabled the WWDG cannot be disabled except by a system reset.
    (+) WWDGRST flag in RCC CSR register can be used to inform when a WWDG
        reset occurs.
    (+) The WWDG counter input clock is derived from the APB clock divided
        by a programmable prescaler.
    (+) WWDG clock (Hz) = PCLK1 / (4096 * Prescaler)
    (+) WWDG timeout (mS) = 1000 * (T[5;0] + 1) / WWDG clock (Hz)
        where T[5;0] are the lowest 6 bits of Counter.
    (+) WWDG Counter refresh is allowed between the following limits :
        (++) min time (mS) = 1000 * (Counter - Window) / WWDG clock
        (++) max time (mS) = 1000 * (Counter - 0x40) / WWDG clock
    (+) Typical values:
        (++) Counter min (T[5;0] = 0x00) @56MHz (PCLK1) with zero prescaler:

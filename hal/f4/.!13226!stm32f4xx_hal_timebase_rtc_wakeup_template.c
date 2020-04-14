/**
  ******************************************************************************
  * @file    stm32f4xx_hal_timebase_rtc_wakeup_template.c
  * @author  MCD Application Team
  * @brief   HAL time base based on the hardware RTC_WAKEUP Template.
  *
  *          This file overrides the native HAL time base functions (defined as weak)
  *          to use the RTC WAKEUP for the time base generation:
  *           + Intializes the RTC peripheral and configures the wakeup timer to be
  *             incremented each 1ms
  *           + The wakeup feature is configured to assert an interrupt each 1ms
  *           + HAL_IncTick is called inside the HAL_RTCEx_WakeUpTimerEventCallback
  *           + HSE (default), LSE or LSI can be selected as RTC clock source
 @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
    [..]
    This file must be copied to the application folder and modified as follows:
    (#) Rename it to 'stm32f4xx_hal_timebase_rtc_wakeup.c'
    (#) Add this file and the RTC HAL drivers to your project and uncomment
       HAL_RTC_MODULE_ENABLED define in stm32f4xx_hal_conf.h

    [..]

#include "FreeRTOS.h"
#include "task.h"

#include "clock.h"
#include "config.h"
#include "gpio.h"

#if !defined(LED1_ON)
#define LED1_OFF    0
#define LED1_ON     1
#endif

#define mainFLASH_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )


static void prvSetupHardware(void) {
    HAL_Init();

    SystemClock_Config();

    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_clock_enable(LED1_PORT);
    GPIO_InitStruct.Pin = LED1_PIN;
    HAL_GPIO_Init(LED1_PORT, &GPIO_InitStruct);
}

static portTASK_FUNCTION(vLEDFlashTask, pvParameters)
{
    (void) pvParameters;    // not used

    TickType_t xLastFlashTime;
    TickType_t xFlashDelay100msec;

    xFlashDelay100msec = 100 / portTICK_PERIOD_MS;

    /* We need to initialise xLastFlashTime prior to the first call to
     * vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    for(;;) {
        // We use the BSRR register to turn the LED on/off which is 
        // a write only operation, which is atomic. So we don't need
        // critical sections.

        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_ON);
        vTaskDelayUntil(&xLastFlashTime, xFlashDelay100msec);

        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_OFF);
        vTaskDelayUntil(&xLastFlashTime, xFlashDelay100msec);

        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_ON);
        vTaskDelayUntil(&xLastFlashTime, xFlashDelay100msec);

        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_OFF);
        vTaskDelayUntil(&xLastFlashTime, xFlashDelay100msec * 2);

        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_ON);
        vTaskDelayUntil(&xLastFlashTime, xFlashDelay100msec);

        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, LED1_OFF);
        vTaskDelayUntil(&xLastFlashTime, xFlashDelay100msec * 4);
    }
}

int main(void)
{
    prvSetupHardware();

    xTaskCreate(vLEDFlashTask, "LED1", configMINIMAL_STACK_SIZE, NULL, mainFLASH_TASK_PRIORITY, (TaskHandle_t *)NULL);
    vTaskStartScheduler();

	// If all is well, the scheduler will now be running, and the following line
	// will never be reached.  If the following line does execute, then there was
	// insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	// to be created.  See the memory management section on the FreeRTOS web site
	// for more details.
	for (;;) {
        ;
    }
}

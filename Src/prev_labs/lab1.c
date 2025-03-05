#include <stm32f0xx_hal.h>
#include <main.h>

#include "hal_gpio.h"

int lab1_main(void) {
    HAL_Init(); // Reset of all peripherals, init the Flash and Systick
    SystemClock_Config(); //Configure the system clock
    /* This example uses HAL library calls to control
    the GPIOC peripheral. You’ll be redoing this code
    with hardware register access. */
    __HAL_RCC_USART1_CLK_ENABLE();
    My_HAL_RCC_GPIOA_CLK_ENABLE(); // Enable the GPIOC clock in the RCC
    My_HAL_RCC_GPIOC_CLK_ENABLE();
    // Set up a configuration struct to pass to the initialization function
    GPIO_InitTypeDef initStr = (GPIO_InitTypeDef){GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW
    };
    My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Start PC8 high

    initStr = (GPIO_InitTypeDef){GPIO_PIN_0,
        GPIO_MODE_INPUT,
        GPIO_PULLDOWN,
        GPIO_SPEED_FREQ_LOW
    };
    
    My_HAL_GPIO_Init(GPIOA, &initStr);
    uint32_t debouncer = 0;
    while (1) {
        debouncer = (debouncer << 1);
        if (My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
            debouncer |= 0x01;
        }
        if (debouncer == 0x7FFFFFFF) {
            My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
        }
    }
}
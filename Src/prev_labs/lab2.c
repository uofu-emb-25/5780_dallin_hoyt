#include <stm32f0xx_hal.h>
#include <main.h>
#include <assert.h>
#include <core_cm0.h>

#include "hal_gpio.h"

int lab2_main(void) {
    HAL_Init();
    SystemClock_Config();
    My_HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_9 | GPIO_PIN_7 | GPIO_PIN_8,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW
    };
    HAL_GPIO_Init(GPIOC, &initStr);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9, GPIO_PIN_SET);
    initStr = (GPIO_InitTypeDef){GPIO_PIN_0,
        GPIO_MODE_INPUT,
        GPIO_PULLDOWN,
        GPIO_SPEED_FREQ_LOW
    };
    
    My_HAL_GPIO_Init(GPIOA, &initStr);

    // Enable EXTI
    assert(EXTI->IMR == 0x7F840000UL);
    assert(EXTI->RTSR == 0x0UL);
    assert(EXTI->FTSR == 0x0UL);
    unmask_exti(0, RISING);
    assert(EXTI->IMR == 0x7F840001UL);
    assert(EXTI->RTSR == 0x1UL);
    assert(EXTI->FTSR == 0x0UL);


    // Route EXTI        
    My_HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    assert(SYSCFG->EXTICR[0] == 0x0UL);
    My_HAL_Route_EXTI(0, BANKA);
    assert(SYSCFG->EXTICR[0] == 0x0UL);

    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 3);
    NVIC_SetPriority(SysTick_IRQn, 2);

    while (1) {
        HAL_Delay(400);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
    }
}

void EXTI0_1_IRQHandler(void) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9 | GPIO_PIN_8);
    for(int i = 0; i < 1000000; i++) {}
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9 | GPIO_PIN_8);
    EXTI->PR |= 1 << 0;
}
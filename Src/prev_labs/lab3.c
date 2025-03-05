#include <stm32f0xx_hal.h>
#include <main.h>
#include <assert.h>
#include <core_cm0.h>

#include "hal_gpio.h"

int lab3_main(void) {
    HAL_Init();
    //SystemClock_Config();
    My_HAL_RCC_TIM2_CLK_ENABLE();
    My_HAL_RCC_GPIOC_CLK_ENABLE();
    My_HAL_RCC_TIM3_CLK_ENABLE();

    TIM2->PSC = (8000 - 1);
    TIM2->ARR = 250;
    TIM2->DIER |= 1;
    TIM2->CR1 |= 1;
    NVIC_EnableIRQ(TIM2_IRQn);

    TIM3->PSC = (100 - 1);
    TIM3->ARR = 100;
    TIM3->CCMR1 &= ~(0b11 << 8);
    TIM3->CCMR1 &= ~(0b11 << 0);
    TIM3->CCMR1 |= 0b111 << 4;
    TIM3->CCMR1 &= ~(0b111 << 12);
    TIM3->CCMR1 |= 0b110 << 12;
    TIM3->CCMR1 |= 0b1 << 3;
    TIM3->CCMR1 |= 0b1 << 11;
    TIM3->CCER |= TIM_CCER_CC1E;
    TIM3->CCER |= TIM_CCER_CC2E;
    TIM3->CCR1 = 90;
    TIM3->CCR2 = 90;
    TIM3->CR1 |= 1;

    GPIO_InitTypeDef initStr = { GPIO_PIN_9 | GPIO_PIN_8,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW
    };
    HAL_GPIO_Init(GPIOC, &initStr);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

    initStr = (GPIO_InitTypeDef){ GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW
    };

    GPIOC->AFR[0] &= ~(0b1111 << 24);
    GPIOC->AFR[0] &= ~(0b1111 << 28);
    HAL_GPIO_Init(GPIOC, &initStr);

    while(1) {}
}

void TIM2_IRQHandler(void) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9 | GPIO_PIN_8);
    TIM2->SR &= ~1;
}
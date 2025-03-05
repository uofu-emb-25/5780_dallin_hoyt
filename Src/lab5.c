#include <stm32f0xx_hal.h>
#include <main.h>
#include <assert.h>
#include <core_cm0.h>

#include "hal_gpio.h"

void send_data(uint8_t data) {
    SET_BITS(I2C2->CR2, 0, 10, 0x69U << 1); // SADD
    SET_BITS(I2C2->CR2, 16, 8, 1U); // NBYTES
    SET_BITS(I2C2->CR2, 10, 1, 0U); // RD_WRN
    SET_BITS(I2C2->CR2, 13, 1, 1U); // START
    while (!(GET_BITS(I2C2->ISR, 1, 1) || GET_BITS(I2C2->ISR, 4, 1))) {}

    if (GET_BITS(I2C2->ISR, 4, 1)) {
        assert(0);
        while (1) {}
    }
    SET_BITS(I2C2->TXDR, 0, 8, (uint32_t)data);
    while(!GET_BITS(I2C2->ISR, 6, 1)) {}
}

uint8_t read_data() {
    SET_BITS(I2C2->CR2, 0, 10, 0x69U << 1); // SADD
    SET_BITS(I2C2->CR2, 16, 8, 1U); // NBYTES
    SET_BITS(I2C2->CR2, 10, 1, 1U); // RD_WRN
    SET_BITS(I2C2->CR2, 13, 1, 1U); // START
    while (!(GET_BITS(I2C2->ISR, 2, 1) || GET_BITS(I2C2->ISR, 4, 1))) {}
    if (GET_BITS(I2C2->ISR, 4, 1)) {
        assert(0);
        while (1) {}
    }
    while(!GET_BITS(I2C2->ISR, 6, 1)) {}
    uint32_t bruh = I2C2->RXDR;
    return GET_BITS(I2C2->RXDR, 0, 8);
}

int lab5_main(void) {
    HAL_Init(); 
    SystemClock_Config();
    My_HAL_RCC_GPIOB_CLK_ENABLE();
    My_HAL_RCC_GPIOC_CLK_ENABLE();
    My_HAL_RCC_I2C2_CLK_ENABLE();

    GPIO_InitTypeDef initStr = {GPIO_PIN_11,
        GPIO_MODE_AF_OD,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW,
        1U
    };  
    My_HAL_GPIO_Init(GPIOB, &initStr);
    initStr = (GPIO_InitTypeDef){GPIO_PIN_13,
        GPIO_MODE_AF_OD,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW,
        5U
    };
    My_HAL_GPIO_Init(GPIOB, &initStr);
    initStr = (GPIO_InitTypeDef){GPIO_PIN_14,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW    
    };
    My_HAL_GPIO_Init(GPIOB, &initStr);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
    initStr = (GPIO_InitTypeDef){GPIO_PIN_0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW    
    };
    My_HAL_GPIO_Init(GPIOC, &initStr);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    initStr = (GPIO_InitTypeDef){GPIO_PIN_15,
        GPIO_MODE_INPUT,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW    
    };
    My_HAL_GPIO_Init(GPIOB, &initStr);
    initStr = (GPIO_InitTypeDef){GPIO_PIN_6,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW    
    };
    My_HAL_GPIO_Init(GPIOC, &initStr);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
    SET_BITS(I2C2->TIMINGR, 28, 4, 1U);
    SET_BITS(I2C2->TIMINGR, 0, 8, 0x13U);
    SET_BITS(I2C2->TIMINGR, 8, 8, 0xFU);
    SET_BITS(I2C2->TIMINGR, 16, 4, 0x2U);
    SET_BITS(I2C2->TIMINGR, 20, 4, 0x4U);
    SET_BITS(I2C2->CR1, 0, 1, 1U);
    send_data(0x26U);
    uint8_t data = read_data();
    assert(data == 0xD3U);
    SET_BITS(I2C2->CR2, 14, 1, 1U);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
    while (1) {}
}
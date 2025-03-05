#include <stm32f0xx_hal.h>
#include <main.h>
#include <assert.h>
#include <core_cm0.h>

#include "hal_gpio.h"

void send_char(char c) {
    while(!((USART3->ISR >> 7) & 0b1U)) {}
    USART3->TDR = c;
}

void send_string(char* str) {
    while(*str) {
        send_char(*str);
        str++;
    }
}


char read_char() {
    while(!((USART3->ISR >> 5) & 0b1U)) {}
    char c = USART3->RDR;
    send_char(c);
    return c;
}

int toggle_led(uint16_t GPIO_Pin) {
    char command = read_char();
    switch (command) {
        case '0':
            HAL_GPIO_WritePin(GPIOC, GPIO_Pin, GPIO_PIN_RESET);
            send_string("\r\nTurned off ");
            return 1;
        case '1':
            HAL_GPIO_WritePin(GPIOC, GPIO_Pin, GPIO_PIN_SET);
            send_string("\r\nTurned on ");
            return 1;
        case '2':
            HAL_GPIO_TogglePin(GPIOC, GPIO_Pin);
            send_string("\r\nToggled ");
            return 1;
        
        default:
            send_string("\r\nUnrecognized command type: ");
            send_char(command);
            send_string("\r\n");
            return 0;
    }
}

int lab4_main(void) {
    HAL_Init();

    My_HAL_RCC_GPIOC_CLK_ENABLE();
    My_HAL_RCC_GPIOB_CLK_ENABLE();
    My_HAL_RCC_USART3_CLK_ENABLE();

    GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_9 | GPIO_PIN_7 | GPIO_PIN_8,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW
    };  
    HAL_GPIO_Init(GPIOC, &initStr);

    initStr = (GPIO_InitTypeDef){ GPIO_PIN_10 | GPIO_PIN_11,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_LOW,
        4U
    };
    HAL_GPIO_Init(GPIOB, &initStr);

    uint32_t freq = HAL_RCC_GetHCLKFreq();
    USART3->BRR = freq/115200;
    USART3->CR1 |= 0b1100;
    USART3->CR1 |= 0b1;
    while(1) {
        send_string("CMD?");
        char color = read_char();
        switch (color) { 
            case 'r':
                if (toggle_led(GPIO_PIN_6)) {
                    send_string("the red LED!\r\n");
                }
                break;
            case 'b':
                if (toggle_led(GPIO_PIN_7)) {
                    send_string("the blue LED!\r\n");
                }
                break;
            case 'o':
                if (toggle_led(GPIO_PIN_8)) {
                    send_string("the orange LED!\r\n");
                }
                break;
            case 'g':
                if (toggle_led(GPIO_PIN_9)) {
                    send_string("the green LED!\r\n");
                }
                break;
            default:
                send_string("\r\nUnrecognized led color: ");
                send_char(color);
                send_string("\r\n");
                break;
        }
    }
}
#include "main.h"
#include <stm32f0xx_hal.h>

int main(void)
{
  lab5_main();
  #if defined(LAB1)
  lab1_main();
  #elif defined(LAB2)
  lab2_main();
  #elif defined(LAB3)
  lab3_main();
  #elif defined(LAB4)
  lab4_main();
  #elif defined(LAB5)
  lab5_main();
  #elif defined(LAB6)
  lab6_main();
  #elif defined(LAB7)
  lab7_main();
  #else
  #error No valid target specified
  #endif

}

void unmask_exti(uint16_t index, enum EdgeType edge) {
  EXTI->IMR |= 1 << index;
  if (edge == RISING) {
    EXTI->RTSR |= 1 << index;
    EXTI->FTSR &= ~(1 << index);
  } else if (edge == FALLING) {
    EXTI->RTSR &= ~(1 << index);
    EXTI->FTSR |= 1 << index;
  } else {
    EXTI->RTSR |= 1 << index;
    EXTI->FTSR |= 1 << index;
  }
}
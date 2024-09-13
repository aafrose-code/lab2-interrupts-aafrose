#include "stm32f0xx.h"
extern void autotest();
extern void internal_clock();
void initb();
void initc();
void togglexn(GPIO_TypeDef *port, int n);

int main(void) {
    //autotest();//
    initb();
    initc();
}
void initb() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;//Enabling the RCC clock for port B
    GPIOB->MODER &= ~0x00FF0F0F;//setting default input for all the necessary pins (pins 0, 2, 3, 4, 8, 9, 10 , 11)
    GPIOB->MODER |= 0x00055000;//setting the output for pins 8, 9, 10 , 11 (making 0, 2, 3, 4 inputs)
    GPIOB->PUPDR &= ~0x000000F0;//setting pull down for pins 2 and 3
    GPIOB->PUPDR |= 0x000000A0;//executing pull down for pins 2 and 3
}
void initc() {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;//Enabling the RCC clock for port C
    GPIOC->MODER &= ~0xFF000FFF;//setting default input from pins 4-9  
    GPIOC->MODER |= 0x00555000;//setting output from 4-9
    GPIOC->PUPDR &= ~0xFFFF0000;//setting pull down for 0-3
    GPIOC->PUPDR |= 0x0000AAAA;//executing pull down for 0-3
}
void togglexn(GPIO_TypeDef *port, int n) {
    if(port->ODR & 1 << n) {//output the data for that port and read the port value (if the port is 1, then:)
        port->BRR = (1 << n);//reset the port and change it to 0
    } else {//if port is 0, then:
        port->BSRR = (1 << n);//set the port to the value from port n
    }
}



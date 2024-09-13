#include "stm32f0xx.h"
extern void autotest();
extern void internal_clock();
void initb();
void initc();
void togglexn(GPIO_TypeDef *port, int n);
void init_exti();

int main(void) {
    //autotest();//
    initb();
    initc();
    init_exti();
    EXTI0_1_IRQHandler();
    EXTI2_3_IRQHandler();
    EXTI4_15_IRQHandler();
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
        port->BRR = (1 << n);//reset the port and change it to the value from port n
    } else {//if port is 0, then:
        port->BSRR = (1 << n);//set the port to the value from port n
    }
}

void init_exti () {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;//Enabling the SYSCFG clock
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0 | ~SYSCFG_EXTICR1_EXTI2 | ~SYSCFG_EXTICR1_EXTI3;//clearing data and setting input (similar to GPIO->MODER command)
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0 | SYSCFG_EXTICR1_EXTI2 | SYSCFG_EXTICR1_EXTI3;//setting EXTI registers as interrupt sources
    EXTI->RTSR |= EXTI_RTSR_TR0 | EXTI_RTSR_TR2 | EXTI_RTSR_TR3 | EXTI_RTSR_TR4;//Setting bits for rising edge for the interrupt (setting those bits by using the OR operator)
    EXTI->IMR |= EXTI_IMR_IM0 | EXTI_IMR_IM2 | EXTI_IMR_IM3 | EXTI_IMR_IM4;//Setting bits for falling edge for the interrupt (setting those bits by using the OR operator)
    NVIC_EnableIRQ(EXTI0_1_IRQn);//Enabling interrupt using NVIC_EnableIRQ, where EXTI0_1_IRQn is from pins 0 to 1
    NVIC_EnableIRQ(EXTI2_3_IRQn);//EXTI2_3_IRQn is from pins 2 to 3
    NVIC_EnableIRQ(EXTI4_15_IRQn);//EXTI4_15_IRQn is from pins 4 to 15 (No need to involve anything from 5-15 but the stm32f091xc.s file can only include pin 4 if 4-15 were included)
}

void EXTI0_1_IRQHandler() { //found from stm32f091xc.s file  
    EXTI->PR = EXTI_PR_PR0;//clearing bit (not preserving through &=) for pin 0
    togglexn(GPIOB, 8);//toggle to 8 when pin 0 from GPIOB port is pressed
}

void EXTI2_3_IRQHandler() { //found from stm32f091xc.s file
    EXTI->PR = EXTI_PR_PR2;//clearing bit for pin 2
    togglexn(GPIOB, 9);//toggle to 9 when pin 2 from GPIOB port is pressed
    EXTI->PR = EXTI_PR_PR3;//clearing bit for pin 3     
}

void EXTI4_15_IRQHandler() { //found from stm32f091xc.s file
    EXTI->PR = EXTI_PR_PR4;//clearing bit for pin 4
    togglexn(GPIOB, 10);//toggle to 10 when pin 4 from GPIOB port is pressed
}

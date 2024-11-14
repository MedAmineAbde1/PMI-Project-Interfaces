#include <ow.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <systick.h>
#include <string.h>
#include <time.h>

#define time_10µs 0x0a
#define time_60µs 0x3c
#define time_480µs 0x1e0

void timer(uint32_t time)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2 -> CR1 &= ~TIM_CR1_CEN; // disable timer to be able to configure
    //TIM2->CR1 |= TIM_CR1_CEN;
    TIM2 -> PSC = SystemCoreClock/ 1000000 - 1 ; // Prescaler in µs
    TIM2 -> ARR = time;
    TIM2 -> DIER  |= TIM_DIER_UIE;
    //NVIC_ClearPendingIRQ(TIM2_IRQn); //beg
    //NVIC_SetPriority(TIM2_IRQn, 2); //beg
    //NVIC_EnableIRQ(TIM2_IRQn); //beg
    TIM2 -> CR1 |= TIM_CR1_CEN;

    while ((TIM2->SR && TIM_SR_UIF) == 0);//Wait

    TIM2->SR = 0;//reset

    TIM2 -> CR1 &= ~(TIM_CR1_CEN);//reset
}

void ow_high()
{
    GPIOB->ODR |= GPIO_ODR_OD13;
} 

void ow_low()
{
    GPIOB->ODR &= ~(GPIO_ODR_OD13);
}

void TIM2_IRQHandler(void)
{
    if(TIM2 -> SR || TIM_SR_UIF)
    {   
        ow_high();
        TIM2 -> SR &= ~TIM_SR_UIF;
    }    
}



void ow_init()
{   
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN ;//enable clock for GPIO B

    // Set the pins as General Purpose Output Mode
    GPIOB->MODER |= GPIO_MODER_MODE13_0; // Configure the pin PB13 as General Purpose Output Mode (GPOM)
    GPIOB->MODER &= ~(GPIO_MODER_MODE13_1); //The Bits of thr Pin PB13 is set as 01 for GPOM
    GPIOB->MODER |= GPIO_MODER_MODE13; // Setzen Sie den GPIO-Pin auf Ausgang.
    GPIOB->OTYPER |= GPIO_OTYPER_OT_13; // Setzen Sie den GPIO-Pin auf Open-Drain.
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED13_0; // Setzen Sie die Geschwindigkeit des GPIO-Pins.
    GPIOB->ODR |= GPIO_ODR_OD13;

    ow_high();
}

void ow_tx(uint32_t data)
{   
    if (data)
    {
        ow_low();
        timer(time_10µs);
        //TIM2_IRQHandler();
        ow_high();
    }
    else
    {
        ow_low();
        timer(time_60µs);
        //TIM2_IRQHandler();
        ow_high();
    }
    
}

void ow_write(uint32_t buffer)
{
    for (size_t i = 0x08; i >= 0x0; i>>1)
    {
        uint32_t bit= (buffer && i);
        ow_tx(bit);
    }
    
}

uint8_t signal_rx()
{
    return (GPIOB->IDR & GPIO_IDR_ID13);
}

uint32_t ow_rx()
{
    uint8_t data = 0x0; //buffer zum  data empfangen
    ow_low();
    timer(time_10µs);

    data = signal_rx(); //checks if after 10 µs ow signal is high otherwise checks again if after 45µs is ow still low then is 0 otherwise 1
    
    timer(time_60µs);

    return data;
}

uint32_t ow_read()
{
    uint32_t Byte_rx = 0x0;
    for (size_t i = 0; i < 8; i++)
    {
        Byte_rx |= (ow_rx() << i);
    }
    return Byte_rx ;
}

void ow_reset()
{
    ow_low();
    timer(time_480µs);
    //TIM2_IRQHandler();
    ow_high();

}

void ow_strong_pullup()
{
    systick_delay_ms(5);
}




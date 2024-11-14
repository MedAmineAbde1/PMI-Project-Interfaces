#include <spi.h>
#include <i2c.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <systick.h>
#include <string.h>

int32_t spi_init_adxl345 ( void )
{
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN; //enable clock for GPIO
    

    // d.h: 10= ALTERNATIVE FUNKTION EINSETZEN
    GPIOA->MODER &= ~(GPIO_MODER_MODE5_0); //set GPIO for PA5 in [0] in 0
    GPIOA->MODER |= (GPIO_MODER_MODE5_1); // set GPIO for PA5 in [1] in 1 
    
    GPIOA->MODER &= ~(GPIO_MODER_MODE6_0); //set GPIO for PA6 in [0] in 0
    GPIOA->MODER |= (GPIO_MODER_MODE6_1);// set GPIO for PA6 in [1] in 1
    
    GPIOA->MODER &= ~(GPIO_MODER_MODE7_0);// set GPIO for PA7 in [0] in 0
    GPIOA->MODER |= (GPIO_MODER_MODE7_1);// set GPIO for PA7 in [1] in 1

    //to select the slave in GPIO at PA12 should be put at output mode d.h: 01
    GPIOA->MODER |= GPIO_MODER_MODE12_0; // set GPIO for PA12 in [0] in 1 to select the slave in PA12
    GPIOA->MODER &= ~(GPIO_MODER_MODE12_1);// set GPIO for PA12 in [1] in 0 to select the slave in PA12

    GPIOA->AFR[0]  &= ~(GPIO_AFRL_AFSEL5); // Clear 4 bits in afr[0] bzw afrl to select alternate function 0
    GPIOA->AFR[0]  &= ~(GPIO_AFRL_AFSEL6);// Clear 4 bits in afr[0] bzw afrl to select alternate function 0
    GPIOA->AFR[0]  &= ~(GPIO_AFRL_AFSEL7);// Clear 4 bits in afr[0] bzw afrl to select alternate function 0


    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //enable clock for SPI
    /*Set up SPI (page 884 and 913) */
    SPI1->CR1 &= ~(SPI_CR1_SPE); //SPI Disabled zum Konfigurieren 
    

    SPI1->CR1 |= SPI_CR1_MSTR; //SPI auf Master einsetzen
    //SPI1->CR1 |= SPI_CR1_BR_0; //SPI BR auf 011(für r=f_pclk/16 da r_max = 5MHz mit f_pclk= 16 MHz ) eingesetzt
    //SPI1->CR1 |= SPI_CR1_BR_1;
    SPI1->CR1 |= SPI_CR1_BR_2;

    SPI1->CR1 |= SPI_CR1_CPOL; //Cpol und CPHA  auf 1 eingesetzt
    SPI1->CR1 |= SPI_CR1_CPHA;

    SPI1->CR1 |=SPI_CR1_SSM | SPI_CR1_SSI; //Software slave management and Internal select slave enabled
    SPI1->CR1 |= SPI_CR1_SPE;
    return 0;
}

int32_t spi_txrx ( uint8_t * buf , uint32_t size )
{
     // see if spi is busy at transmiting if that's the case then it will wait until it's not
    while (SPI1->SR & SPI_SR_BSY);

    //set the PA12 to low which selects the line Chipselect as a slave
    GPIOA->ODR &= ~(GPIO_ODR_OD12);

    while (size > 0)
    {
        while (!(SPI1->SR & SPI_SR_TXE));//wait that the SPI finishes to transmit data
        SPI1->DR = *buf ;
        while (!(SPI1->SR & SPI_SR_RXNE));//wait that the SPI finishes to receive data
        *buf = SPI1->DR;
        buf++;
        size--;
    }
    GPIOA->ODR |= GPIO_ODR_OD12;
    return 0;
}


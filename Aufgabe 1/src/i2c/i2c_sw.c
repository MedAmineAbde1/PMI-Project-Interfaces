#include <i2c_sw.h>
#include <i2c.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <systick.h>
#include <string.h>



void delay() 
{
    nop_30();
}

int32_t SDA_high(void)
{
    GPIOB->ODR |= GPIO_ODR_OD9 ;
    return 0;
}

int32_t SDA_low(void)
{
    GPIOB->ODR &= ~(GPIO_ODR_OD9);
    return 0;
}

int32_t SCLK_high(void)
{
    GPIOB->ODR |= GPIO_ODR_OD8 ;
    return 0;
}

int32_t SCLK_low(void)
{
    GPIOB->ODR &= ~(GPIO_ODR_OD8) ;
    return 0;
}

void Start_I2C()
{
    SDA_high();
    SCLK_high();//Let's see where it goes 
    delay();
    SDA_low();
    delay();
}

void Stop_I2C()
{
    SDA_low();
    SCLK_high();//Let's see where it goes 
    delay();
    SDA_high();
    delay(); 
}

void Master_Nack()
{
    //Master_Nack
    SCLK_low();
    delay();
    SDA_high();
    delay();
    SCLK_high();

}

void Master_Ack()
{
    //Master_Ack
    SCLK_low();
    delay();
    SDA_high();
    delay();
    SCLK_high();

}

int8_t Slave_Ack(void)
{
    int8_t rc ;
    SDA_high();
    SCLK_high();//Let's see where it goes 
    delay();
    SCLK_low();
    if(GPIOB->IDR & GPIO_IDR_ID9) 
    {
        rc = RC_ERR;
    }
    else
    {   

        rc = RC_SUCC;
    }
    delay();
    SCLK_high();
    delay();

    return rc;
}

int32_t i2c_sw_init()
{   
    RCC->IOPENR |= RCC_IOPENR_IOPBEN;//enable clock for GPIO B
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST ; // Resets I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN ; // Enable Clock for I2C1

    // Set the pins as General Purpose Output Mode
    GPIOB->MODER |= GPIO_MODER_MODE8_0; // Configure the pin PB8 as General Purpose Output Mode (GPOM)
    GPIOB->MODER &= ~(GPIO_MODER_MODE8_1); //The Bits of thr Pin PB8 is set as 01 for GPOM 

    GPIOB->MODER |= GPIO_MODER_MODE9_0; // Configure the pin PB9 as General Purpose Output Mode (GPOM)
    GPIOB->MODER &= ~(GPIO_MODER_MODE9_1); //The Bits of thr Pin PB9 is set as 01 for GPOM

    // Set the output as Open-drain at the Pins PB8 and PB9
    GPIOB->OTYPER |= GPIO_OTYPER_OT_8;
    GPIOB->OTYPER |= GPIO_OTYPER_OT_9;
    SDA_high();
    SCLK_high();

    return 0;
}

int32_t i2c_sw_tx(uint8_t Buffer)
{   
    for (size_t i = 7; i >= 0 ; i--)
    {
        if (Buffer & (1 << i))
        {
            SCLK_low();
            delay();

            SDA_high();
            delay();
            SCLK_high();
            delay();
            
        }
        else
        {
            SCLK_low();
            delay();
            SDA_low();
            delay();
            SCLK_high();
            delay();
            
        }
    }

    Slave_Ack();
}

void i2c_sw_write(uint8_t Adresse,uint8_t Register,uint8_t Data)
{
    Start_I2C();
    i2c_sw_tx(Adresse);
    if (Register != 0b00)
    {
        i2c_sw_tx(Register);
    }
    i2c_sw_tx(Data);
    Stop_I2C();
}

int32_t i2c_sw_rx()
{   
    uint32_t duff = 0x0; //buffer zum Data Empfangen
    for(uint8_t index = 0; index <= 7 ; index++)
    {
        SDA_high();
        delay();
        SCLK_low();
        delay();
        /* Slave changes Bit*/
        delay();
        SCLK_high();
        delay();
        duff = duff || (GPIOB->IDR & GPIO_IDR_ID9 << index);
        delay();
    }
    return duff;
}

uint32_t i2c_sw_read() 
{   
    int8_t Buffer[]={0x0,0x0,0x0,0x0,0x0,0x0};

    GPIOB->IDR |= GPIO_IDR_ID9; //set SDA to input
    delay();
    for (size_t i = 0; i <= 5; i++)
    {
        Buffer[i]=i2c_sw_rx();
        if (i ==5)
        {
            Master_Nack();
        }
        else
        {
            Master_Ack();
        }
        
        
    }
    Stop_I2C();
    return Buffer;
}
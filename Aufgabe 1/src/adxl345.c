#include <spi.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <systick.h>
#include <string.h>


int32_t adxl345_init ( void )
{
    spi_init_adxl345(); //initialise spi for adxl345 purposes
    uint8_t tuf[]={0x31,0x08}; // {write-single byte-register adress, byt to set spi in 4wiremode} -> {0-0-110001,01000000}
    spi_txrx(tuf,2); // transmit tuf with length of 2 bytes to set spi in 4 wires mode
    uint8_t buf[]={0x2D,0x08}; //{write-single byte-register adress, byte for measurementmode} -> {0-0-101101, 00001000}
    spi_txrx(buf,2); // transmit buf with  length of 2 bytes to set adxl345 into measurementmode

    return 0;
}

int32_t adxl345_acc_get ( float *x , float *y , float * z )
{
    uint8_t bebuff[]={0xF2,0x0,0x0,0x0,0x0,0x0,0x0}; // {read-multi byte-register adress of x0,empty data for x0-1 &y0-1 & z0-1}-> {1-1-110010,(6x)00000000}
    spi_txrx(bebuff,7); 

    int16_t temp_x = bebuff[1] | ( bebuff[2] << 8 ); // acceleration is 16 bit long -> add both 8Bit integers:Data[0]+Data[1] shifted by 8 bit to get
    int16_t temp_y = bebuff[3] | ( bebuff[4] << 8 ); 
    int16_t temp_z = bebuff[5] | ( bebuff[6] << 8 ); 

    *x = ( (float)temp_x ) * 0.0039 ; // 0.0039 = factor to get values from int16_t to float 1 = 1G
    *y = ( (float)temp_y ) * 0.0039 ; 
    *z = ( (float)temp_z ) * 0.0039 ; 

    return 0;
}



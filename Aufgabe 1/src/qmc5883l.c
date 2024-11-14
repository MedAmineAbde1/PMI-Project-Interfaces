#include <qmc5883l.h>
#include <i2c_sw.h>
#include <i2c.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <systick.h>
#include <string.h>

int32_t qmc5883l_init()
{   
    i2c_sw_init();
    Start_I2C();

    if (i2c_sw_tx(0x0d) == RC_ERR)
    {
        return RC_ERR;
    }
    
    if (i2c_sw_tx(0x09) == RC_ERR)
    {
        return RC_ERR;
    }

    if (i2c_sw_tx(0x01) == RC_ERR)
    {
        return RC_ERR;
    }
    Stop_I2C();
    return RC_SUCC;
}

int32_t qmc5883l_mag_get( float *x , float *y , float * z )
{   
    
    Start_I2C();
    i2c_sw_tx(0x1A);
    i2c_sw_tx(0x00);
    SDA_high();
    delay();
    Start_I2C();
    i2c_sw_tx(0x1B);
    int32_t Buffer[] = i2c_sw_read();

    *x=Buffer[0] && (Buffer[1] << 8);
    *y=Buffer[2] && (Buffer[3] << 8);
    *z=Buffer[4] && (Buffer[5] << 8);
    return RC_SUCC;
}

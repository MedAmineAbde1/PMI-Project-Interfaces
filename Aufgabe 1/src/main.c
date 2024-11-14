#include <clocks.h>
#include <stm32l053xx.h>
#include <systick.h>
#include <uart.h>
#include <adxl345.h>
#include <i2c_sw.h>
#include <qmc5883l.h>




int main(void)
{
    //Initialisierungsfunktionen
    clocks_init_pmi();
    uart_init_nucusb(115200);
    adxl345_init();
    i2c_sw_init();
    qmc5883l_init();

    float x_acc=0x0;
    float y_acc=0x0;
    float z_acc=0x0;

    while (1)
    {
        //adxl345_acc_get(&x_acc, &y_acc, &z_acc);
        qmc5883l_mag_get(&x_acc, &y_acc, &z_acc);
        systick_delay_ms(10);

        
    }
}
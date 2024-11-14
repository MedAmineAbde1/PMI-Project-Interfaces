#ifndef ADXL345_H
#define ADXL345_H

#include <spi.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <systick.h>
#include <string.h>

int32_t adxl345_init ( void );

int32_t adxl345_acc_get ( float *x , float *y , float * z );


#endif /* ADXL345_H */

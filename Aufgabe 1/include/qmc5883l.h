#ifndef QMC5883L_H
#define QMC5883L_H

#include <i2c_sw.h>
#include <i2c.h>
#include <stm32l0xx.h>
#include <pmi_stddefs.h>
#include <systick.h>
#include <string.h>


int32_t qmc5883l_init ( void );

int32_t qmc5883l_mag_get ( float *x , float *y , float * z );

#endif /* QMC5833L_H */
#ifndef SPI_H
#define SPI_H

#include <stdint.h>

int32_t spi_init_adxl345 ( void );

int32_t spi_txrx ( uint8_t * buf , uint32_t size );

#endif /* SPI_H */
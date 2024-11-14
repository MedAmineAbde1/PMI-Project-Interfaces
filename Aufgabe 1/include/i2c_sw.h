#ifndef I2C_SW_H
#define I2C_SW_H

#include <stdint.h>

void delay();

/**
 * @brief The Bit of SDA is pulled high
 * @return 0
 */
int32_t SDA_high(void);

/**
 * @brief The Bit of SDA is pulled low
 * @return 0
 */
int32_t SDA_low(void);

/**
 * @brief The SCLK_Bit is pulled high
 * @return 0
 */
int32_t SCLK_high(void);

/**
 * @brief The SCLK_Bit is pulled low
 * @return 0
 */
int32_t SCLK_low(void);

/**
 * @brief Master send the Nack_bit(1) to the Slave
 */
void Master_Nack();

/**
 * @brief Master send the Ack_bit(0) to the Slave
 */
void Master_Ack();

/**
 * @brief Checks if the Slave received the Data from Master succsesfully
 * @return See @ref PMI_RC_E
 */
int8_t Slave_Ack();

/**
 * @brief Initialises software i2c for pmi purposes
 * @return See @ref PMI_RC_E
 */
int32_t i2c_sw_init();

/**
 * @brief Transmits an amount of data over i2c
 * @param addr 7 bit device address
 * @param Buffer Data buffer
 * @return Checks if the Master received the Ack_bit from Slave
 */
int32_t i2c_sw_tx(uint8_t Buffer);

void i2c_sw_write(uint8_t Adresse,uint8_t Register,uint8_t Data);

int32_t i2c_sw_rx();

uint32_t i2c_sw_read();

#endif /* I2C_SW_H */
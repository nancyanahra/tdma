#ifndef NRF24l01_H
#define NRF24l01_H

#include "main.h"      // pull in HAL types
#include "stm32f4xx_hal_def.h"
#include <stdint.h>

// SPI commands (nRF24L01 datasheet)
#define NRF_R_REGISTER          0x00u   // OR with register address 
#define NRF_W_REGISTER          0x20u   // OR with register address 
#define NRF_R_RX_PAYLOAD        0x61u
#define NRF_W_TX_PAYLOAD        0xA0u
#define NRF_FLUSH_TX            0xE1u
#define NRF_FLUSH_RX            0xE2u
#define NRF_NOP                 0xFFu   

// reg addresses
#define NRF_REG_CONFIG          0x00u
#define NRF_REG_EN_AA           0x01u
#define NRF_REG_EN_RXADDR       0x02u
// setup address widths
#define NRF_REG_SETUP_AW        0x03u
#define NRF_REG_SETUP_RETR      0x04u
#define NRF_REG_RF_CH           0x05u
#define NRF_REG_RF_SETUP        0x06u
#define NRF_REG_STATUS          0x07u
#define NRF_REG_RX_ADDR_P0      0x0Au
#define NRF_REG_TX_ADDR         0x10u
#define NRF_REG_RX_PW_P0        0x11u
#define NRF_REG_FIFO_STATUS     0x17u

// config
#define NRF_CONFIG_EN_CRC       (1u << 3)
#define NRF_CONFIG_PWR_UP       (1u << 1)
#define NRF_CONFIG_PRIM_RX      (1u << 0)

// status bits
#define NRF_STATUS_RX_DR        (1u << 6)
#define NRF_STATUS_TX_DS        (1u << 5)
#define NRF_STATUS_MAX_RT       (1u << 4)

// FIFO
#define NRF_FIFO_TX_EMPTY       (1u << 4)
#define NRF_FIFO_RX_EMPTY       (1u << 0)

#define NRF24_MAX_PAYLOAD       32u

//public api
// return HAL_OK if success
HAL_StatusTypeDef nrf_init     (SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef nrf_read_reg (uint8_t reg, uint8_t *val);
HAL_StatusTypeDef nrf_write_reg(uint8_t reg, uint8_t val);
HAL_StatusTypeDef nrf_write_buf(uint8_t reg, const uint8_t *vals, uint8_t n);
HAL_StatusTypeDef nrf_cmd      (uint8_t cmd);

HAL_StatusTypeDef nrf_read_payload(uint8_t *data, uint8_t n);
HAL_StatusTypeDef nrf_write_payload(const uint8_t *data, uint8_t n);

#endif 
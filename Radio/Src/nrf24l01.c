#include "nrf24l01.h"
#include "stm32f4xx_hal_def.h"
#include <stdint.h>

#define NRF_POR_DELAY_MS     100u   // power-on reset time

static SPI_HandleTypeDef *nrf_hspi = NULL;


HAL_StatusTypeDef nrf_init(SPI_HandleTypeDef *hspi){
  if (hspi == NULL) {
    return HAL_ERROR;
  }
  nrf_hspi = hspi;
    
  HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(CE_GPIO_Port,  CE_Pin,  GPIO_PIN_RESET);

  HAL_Delay(NRF_POR_DELAY_MS);

  return HAL_OK;
}

static HAL_StatusTypeDef nrf_xfer(uint8_t *tx, uint8_t *rx, uint16_t len){
  if(nrf_hspi == NULL){
    return HAL_ERROR;
  }
  HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);
  HAL_StatusTypeDef status_type = HAL_SPI_TransmitReceive(nrf_hspi, tx, rx, len, 100);
  HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);
  return status_type;
}

//   uint8_t tx[2], rx[2], result[3];
HAL_StatusTypeDef nrf_read_reg(uint8_t reg, uint8_t *val){

  uint8_t tx[2], rx[2];
  tx[0] = NRF_R_REGISTER | reg; // 0x00 is the READ register
  tx[1] = NRF_NOP; // no data to transmit when reading
  HAL_StatusTypeDef status = nrf_xfer(tx, rx, 2);
  if (status == HAL_OK) {
    *val = rx[1];
  }
  return status;
}

// write to just 1 register
HAL_StatusTypeDef nrf_write_reg(uint8_t reg, uint8_t val){
  
  uint8_t tx[2], rx[2];
  tx[0] = NRF_W_REGISTER | reg;
  tx[1] = val;
  return nrf_xfer(tx, rx, 2);
}

// write a multi-byte register (address)
HAL_StatusTypeDef nrf_write_buf(uint8_t reg, const uint8_t *vals, uint8_t n){
  if( n > NRF24_MAX_PAYLOAD){
    return HAL_ERROR;
  }
  uint8_t tx[NRF24_MAX_PAYLOAD+1], rx[NRF24_MAX_PAYLOAD+1];
  tx[0] = NRF_W_REGISTER | reg;
  for (int i = 0; i < n; i ++){
    tx[i+1] = vals[i];
  }
  return nrf_xfer(tx, rx, n+1);
}

HAL_StatusTypeDef nrf_read_payload(uint8_t *data, uint8_t n){
  if( n > NRF24_MAX_PAYLOAD){
    return HAL_ERROR;
  }
  uint8_t tx[NRF24_MAX_PAYLOAD+1], rx[NRF24_MAX_PAYLOAD+1];

  tx[0]=NRF_R_RX_PAYLOAD; // R_RX_PAYLOAD cmd
  for (int i = 0; i<n; i++){
    tx[i+1] =NRF_NOP;
  }
  HAL_StatusTypeDef stat = nrf_xfer(tx,rx,n+1);
  if (stat == HAL_OK){
    for (int i = 0; i<n; i++){
      data[i]=rx[i+1];
    }
  }
  return stat;
  
}


HAL_StatusTypeDef nrf_write_payload(const uint8_t *data, uint8_t n){
  if( n > NRF24_MAX_PAYLOAD){
    return HAL_ERROR;
  }
  uint8_t tx[NRF24_MAX_PAYLOAD+1], rx[NRF24_MAX_PAYLOAD+1];
  tx[0] = NRF_W_TX_PAYLOAD;
  for (int i = 0; i < n; i ++){
    tx[i+1] = data[i];
  }
  return nrf_xfer(tx, rx, n+1);
}

// for single byte radio commands, lik flushing TX and RX
HAL_StatusTypeDef nrf_cmd(uint8_t cmd){
  uint8_t tx[1], rx[1];
  tx[0] = cmd; 
  return nrf_xfer(tx,rx,1); // rx will return the STATUS byte
}
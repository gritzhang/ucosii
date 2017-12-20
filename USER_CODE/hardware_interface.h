#ifndef __HARDWARE_INTERFACE_H__
#define __HARDWARE_INTERFACE_H__
#include <stdint.h>

#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_gpio.h"

#define SSP1_SSP_PORT    (0)
#define SSP1_SS_PIN     (1<<6)
#define SSP1_SCLK_PIN     (1<<7)
#define SSP1_MOSI_PIN     (1<<9)

#define SSP1_MISO_PIN     (1<<8)


// rf_FM1702 & FM25Cl64 SPI接口
void SSP0_Init(void);

// W5100 SPI接口	
void SSP1_Init(void);
void SSP1_IO_Init(void);
    
// 初始化I2C1
void i2c1Init(uint32_t budl);

void SSP_Hardeare_SendData(LPC_SSP_TypeDef* SSPx, uint16_t Data);

uint16_t SSP_Hardeare_ReceiveData(LPC_SSP_TypeDef* SSPx);

uint8_t I2C_ReadNByte (uint8_t sla, uint32_t suba_type, uint32_t suba, uint8_t *s, uint32_t num);

uint8_t I2C_WriteNByte(uint8_t sla, uint8_t suba_type, uint32_t suba, uint8_t *s, uint32_t num);

#endif



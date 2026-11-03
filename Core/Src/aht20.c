#include "aht20.h"
#include "stm32f1xx_hal_i2c.h"

#define AHT20_ADDRESS 0x70 // AHT20 I2C address shifted for STM32 HAL

uint8_t data[6] = {0};

void AHT20_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t readbuffer = 0;
    HAL_Delay(40); // Wait for the sensor to power up
    HAL_I2C_Master_Receive(hi2c, AHT20_ADDRESS , &readbuffer, 1, HAL_MAX_DELAY);
    if((readbuffer & 0x08)==0x00)
    {
      uint8_t init_cmd[3] = {0xBE, 0x08, 0x00}; // Initialization command
      HAL_I2C_Master_Transmit(hi2c, AHT20_ADDRESS , init_cmd, 3, HAL_MAX_DELAY);
    }

}

void AHT20_Read(I2C_HandleTypeDef *hi2c, float *temperature, float *humidity) {
    uint8_t trigger_cmd[3] = {0xAC, 0x33, 0x00}; // Trigger measurement command
    HAL_I2C_Master_Transmit(hi2c, AHT20_ADDRESS , trigger_cmd, 3, HAL_MAX_DELAY);
    HAL_Delay(75); // Wait for the measurement to complete

    HAL_I2C_Master_Receive(hi2c, AHT20_ADDRESS , data, 6, HAL_MAX_DELAY);
     if((data[0] & 0x80)==0x80) {
        // Measurement not completed, handle error
        *temperature = 0.0f;
        *humidity = 0.0f;
        return;
    }

    // Convert the received data to temperature and humidity
    uint32_t raw_humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((data[3] >> 4) & 0x0F);
    uint32_t raw_temperature = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

    *humidity = (raw_humidity * 100.0f) / 1048576.0f; // Convert to percentage
    *temperature = (raw_temperature * 200.0f / 1048576.0f) - 50.0f; // Convert to Celsius
}
    




void AHT20_Measure(I2C_HandleTypeDef *hi2c) {
    static uint8_t trigger_cmd[3] = {0xAC, 0x33, 0x00}; // Trigger measurement command
    HAL_I2C_Master_Transmit_DMA(hi2c, AHT20_ADDRESS , trigger_cmd, 3);
}

void AHT20_Get(I2C_HandleTypeDef *hi2c)
{

    HAL_I2C_Master_Receive_DMA(hi2c, AHT20_ADDRESS , data, 6);
}

void AHT20_Analysis(float *temperature, float *humidity)
{
    if((data[0] & 0x80)==0x80) {
        // Measurement not completed, handle error
        *temperature = 0.0f;
        *humidity = 0.0f;
        return;
    }

    // Convert the received data to temperature and humidity
    uint32_t raw_humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((data[3] >> 4) & 0x0F);
    uint32_t raw_temperature = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

    *humidity = (raw_humidity * 100.0f) / 1048576.0f; // Convert to percentage
    *temperature = (raw_temperature * 200.0f / 1048576.0f) - 50.0f; // Convert to Celsius
}
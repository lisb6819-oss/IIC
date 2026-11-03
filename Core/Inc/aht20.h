#ifndef AHT20_H
#define AHT20_H

#include "i2c.h"

void AHT20_Init(I2C_HandleTypeDef *hi2c) ;
void AHT20_Read(I2C_HandleTypeDef *hi2c, float *temperature, float *humidity) ;
void AHT20_Measure(I2C_HandleTypeDef *hi2c);
void AHT20_Get(I2C_HandleTypeDef *hi2c);
void AHT20_Analysis(float *temperature, float *humidity);

#endif // AHT20_H
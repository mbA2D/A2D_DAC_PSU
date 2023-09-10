//Board Description File for A2D_DAC_PSU_V1.0 Board

#include <DACX0502.h>
#include "Arduino.h"

//Valid I2C Address
#define A2D_DAC_PSU_MIN_I2C_ADDR	0x48
#define A2D_DAC_PSU_MAX_I2C_ADDR	0x4B

#define A2D_DAC_PSU_PSU_EN_PIN		4
#define A2D_DAC_PSU_ELOAD_EN_PIN	3
#define A2D_DAC_PSU_ELOAD_EN		1
#define A2D_DAC_PSU_PSU_EN			0

#define A2D_DAC_PSU_LED_PIN         LED_BUILTIN

//calibration
#define A2D_DAC_PSU_V_DEFAULT_SCALING       1.0 // V/V
#define A2D_DAC_PSU_I_DEFAULT_SCALING       2.0 // A/V
#define A2D_DAC_PSU_V_DEFAULT_OFFSET        0.0 // V
#define A2D_DAC_PSU_I_DEFAULT_OFFSET        0.0 // A

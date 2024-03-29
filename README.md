NAU881x audio codec library for STM32
======================================

**Work in progress and untested**

Compatible with Nuvoton NAU8810, NAU88C10, NAU8814, and NAU88C14 mono audio codecs. Specific NAU8814 (and NAU88C14) features are prefixed by `NAU8814_`. It might be compatible with NAU8812

## Useful links:
- Product page: [NAU8810](https://www.nuvoton.com/products/smart-home-audio/audio-converters/audio-codec-series/nau8810yg/) [NAU88C10](https://www.nuvoton.com/products/smart-home-audio/audio-converters/audio-codec-series/nau88c10yg/) [NAU8814](https://www.nuvoton.com/products/smart-home-audio/audio-converters/audio-codec-series/nau8814yg/) [NAU88C14](https://www.nuvoton.com/products/smart-home-audio/audio-converters/audio-codec-series/nau88c14yg/)
- Datasheet: [NAU8810](https://www.nuvoton.com/export/resource-files/NAU8810_Datasheet_Rev_2.8.pdf) [NAU8812](https://www.nuvoton.com/export/resource-files/DS_NAU8812_Datasheet_EN_Rev2.9.pdf) [NAU8814](https://www.nuvoton.com/export/resource-files/en-us--DS_NAU8814_DataSheet_EN_Rev3.2.pdf)

## How to use
Write your implementation to write/read register from the device. For example:
```C
// --- main.c ---

void MyI2C_Write(I2C_HandleTypeDef* i2c_handle, uint8_t i2c_address, uint8_t register, uint16_t value)
{
    uint8_t data[] = {register, value >> 8, value & 0xFF};
    HAL_I2C_Master_Transmit(i2c_handle, i2c_address, data, sizeof(data), HAL_MAX_DELAY);
}

uint16_t MyI2C_Read(I2C_HandleTypeDef* i2c_handle, uint8_t i2c_address, uint8_t register)
{
    uint16_t value = 0;
    HAL_I2C_Master_Transmit(i2c_handle, i2c_address, &register, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(i2c_handle, i2c_address, &value, 2, HAL_MAX_DELAY);
    return value;
}
```
```C
// --- main.h ---

void MyI2C_Write(I2C_HandleTypeDef* i2c_handle, uint8_t i2c_address, uint8_t register, uint16_t value);
uint16_t MyI2C_Read(I2C_HandleTypeDef* i2c_handle, uint8_t i2c_address, uint8_t register);
```

Edit the `nau881x.h` to call your implementation
```C
// --- nau881x.h ---

#include "main.h"    // The location of the function prototype for the implementation

#define NAU881X_REG_WRITE(handle, reg, val) MyI2C_Write(handle, NAU8810_I2C_ADDRESS, reg, val)
#define NAU881X_REG_READ(handle, reg) MyI2C_Read(handle, NAU8810_I2C_ADDRESS, reg)
```

Now you can initialize the audio codec, configure the Digital Audio Interface to transmit/receive audio data, and follow the audio path from the Block Diagram section in the datasheet.
```C
// --- main.c ---

#include "nau881x.h"

NAU881x_t nau8810;

int main(void)
{
    // ...

    nau8810.comm_handle = &hi2c1;
    NAU881x_Init(&nau8810);

    // STM32 as I2S Master
    // TODO

    // Make audio codec functional
    // Code below will route the audio from MICN to Speaker via Bypass (refer to General Block Diagram)
    NAU881x_Set_PGA_Input(&nau8810, NAU881X_INPUT_MICN);
    NAU881x_Set_Speaker_Source(&nau8810, NAU881X_OUTPUT_FROM_BYPASS);
    NAU881x_Set_Output_Enable(&nau8810, NAU881X_OUTPUT_SPK);

    // ...
}
```

## Implemented (based on functional description):
- [X] Input path
    - [X] Single ended auxiliary input (AUX) (NAU8814)
    - [X] Differential microphone input
    - [X] PGA boost/Mixer stage
- [X] Microphone biasing
- [ ] ADC digital filter
    - [X] ADC
    - [X] High pass filter
    - [ ] Notch filter
    - [X] ADC gain
- [X] Programmable gain amplifier (PGA)
    - [X] Automatic level control (ALC)
- [X] DAC digital filter
    - [X] DAC
    - [X] 5-band equalizer
- [X] Analog outputs
    - [X] Speaker mixer
    - [X] Mono mixer
- [X] General purpose control
    - [X] Slow timer clock
    - [X] Jack detect (NAU8814)
    - [X] Thermal shutdown (NAU8814)
- [X] Clock generation
- [X] Control interface
    - [X] Control interface selection (NAU8814)
- [X] Digital audio interface
    - [X] Interface modes
    - [X] Companding
- [X] Power supply
    - [X] Power up sequence

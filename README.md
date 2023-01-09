NAU881x audio codec library for STM32
======================================

**Work in progress and untested**

Compatible with Nuvoton NAU8810, NAU88C10, NAU8814, and NAU88C14 mono audio codecs. Specific NAU8814 (and NAU88C14) features are prefixed by `NAU8814_`

## Useful links:
- Product page: [NAU8810](https://www.nuvoton.com/products/smart-home-audio/audio-converters/audio-codec-series/nau8810yg/) [NAU88C10](https://www.nuvoton.com/products/smart-home-audio/audio-converters/audio-codec-series/nau88c10yg/) [NAU8814](https://www.nuvoton.com/products/smart-home-audio/audio-converters/audio-codec-series/nau8814yg/) [NAU88C14](https://www.nuvoton.com/products/smart-home-audio/audio-converters/audio-codec-series/nau88c14yg/)
- Datasheet: [NAU8810](https://datasheet.lcsc.com/lcsc/2202132130_Nuvoton-Tech-NAU8810YG_C2613464.pdf) [NAU8814](https://datasheet.lcsc.com/lcsc/1810221243_Nuvoton-Tech-NAU8814YG_C94774.pdf)

## How to use
Write your implementation to write/read register from the device. For example:
```C
// --- main.c ---

void MyI2C_Write(I2C_HandleTypeDef* i2c_handle, uint8_t i2c_address, uint8_t register, uint16_t value)
{
    // TODO
}

uint16_t MyI2C_Read(I2C_HandleTypeDef* i2c_handle, uint8_t i2c_address, uint8_t register)
{
    // TODO
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
    // TODO

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
- [ ] Digital audio interface
- [ ] Power supply
    - [ ] Power up sequence
    - [ ] Power down sequence
    - [ ] Power saving

#ifndef MR_NAU881X_H
#define MR_NAU881X_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "nau881x_regs.h"

#define NAU881X_REG_WRITE(handle, reg, val) MyI2C_Write(handle, NAU881X_I2C_ADDRESS, reg, val)
#define NAU881X_REG_READ(handle, reg) MyI2C_Read(handle, NAU881X_I2C_ADDRESS, reg)


typedef struct _NAU881x
{
    uint32_t comm_handle;
    uint16_t _register[80];
} NAU881x_t;


typedef enum _nau881x_status
{
    NAU881X_STATUS_OK = 0,
    NAU881X_STATUS_ERROR = 1,
    NAU881X_STATUS_INVALID = 2
} nau881x_status_t;


nau881x_status_t NAU881x_Init(NAU881x_t* nau881x);

// Input path
nau881x_status_t NAU881x_Get_PGA_Input(NAU881x_t* nau881x, nau881x_input_t* input);
nau881x_status_t NAU881x_Set_PGA_Input(NAU881x_t* nau881x, nau881x_input_t input);
uint8_t NAU881x_Get_PGA_Gain(NAU881x_t* nau881x);
nau881x_status_t NAU881x_Set_PGA_Gain(NAU881x_t* nau881x, uint8_t vol);
nau881x_status_t NAU881x_Set_PGA_Gain_db(NAU881x_t* nau881x, float vol_db);
nau881x_status_t NAU881x_Set_PGA_Mute(NAU881x_t* nau881x, uint8_t state);
nau881x_status_t NAU881x_Set_PGA_ZeroCross(NAU881x_t* nau881x, uint8_t state);
nau881x_status_t NAU881x_Set_PGA_Enable(NAU881x_t* nau881x, uint8_t enable);
nau881x_status_t NAU881x_Set_Aux_Enable(NAU881x_t* nau881x, uint8_t enable);
nau881x_status_t NAU881x_Set_Aux_Mode(NAU881x_t* nau881x, nau881x_aux_mode_t mode);
nau881x_status_t NAU881x_Set_PGA_Boost(NAU881x_t* nau881x, uint8_t state);
nau881x_status_t NAU881x_Set_Boost_Volume(NAU881x_t* nau881x, nau881x_input_t input, uint8_t vol);
nau881x_status_t NAU881x_Set_Boost_Enable(NAU881x_t* nau881x, uint8_t enable);
nau881x_status_t NAU881x_Set_MicBias_Enable(NAU881x_t* nau881x, uint8_t enable);
nau881x_status_t NAU881x_Set_MicBias_Voltage(NAU881x_t* nau881x, uint8_t val);
nau881x_status_t NAU881x_Set_MicBiasMode_Enable(NAU881x_t* nau881x, uint8_t enable);

// ADC digital filter
nau881x_status_t NAU881x_Set_ADC_Enable(NAU881x_t* nau881x, uint8_t enable);
nau881x_status_t NAU881x_Set_ADC_Polarity(NAU881x_t* nau881x, uint8_t invert);
nau881x_status_t NAU881x_Set_ADC_OverSampleRate(NAU881x_t* nau881x, nau881x_adc_oversamplerate_t rate);
nau881x_status_t NAU881x_Set_ADC_HighPassFilter(NAU881x_t* nau881x, uint8_t enable, nau881x_hpf_mode_t mode, uint8_t freq_regval);
nau881x_status_t NAU881x_Set_ADC_Gain(NAU881x_t* nau881x, uint8_t regval);

// ALC
nau881x_status_t NAU881x_Set_ALC_Enable(NAU881x_t* nau881x, uint8_t enable);
nau881x_status_t NAU881x_Set_ALC_Gain(NAU881x_t* nau881x, uint8_t minval, uint8_t maxval);
nau881x_status_t NAU881x_Set_ALC_TargetLevel(NAU881x_t* nau881x, uint8_t val);
nau881x_status_t NAU881x_Set_ALC_Hold(NAU881x_t* nau881x, uint8_t val);
nau881x_status_t NAU881x_Set_ALC_Mode(NAU881x_t* nau881x, nau881x_alc_mode_t mode);
nau881x_status_t NAU881x_Set_ALC_AttackTime(NAU881x_t* nau881x, uint8_t val);
nau881x_status_t NAU881x_Set_ALC_DecayTime(NAU881x_t* nau881x, uint8_t val);
nau881x_status_t NAU881x_Set_ALC_ZeroCross(NAU881x_t* nau881x, uint8_t state);
nau881x_status_t NAU881x_Set_ALC_NoiseGate_Threshold(NAU881x_t* nau881x, uint8_t val);
nau881x_status_t NAU881x_Set_ALC_NoiseGate_Enable(NAU881x_t* nau881x, uint8_t enable);

// DAC digital filter
nau881x_status_t NAU881x_Set_ADC_DAC_Passthrough(NAU881x_t* nau881x, uint8_t enable);
nau881x_status_t NAU881x_Set_DAC_Enable(NAU881x_t* nau881x, uint8_t enable);
nau881x_status_t NAU881x_Set_DAC_Polarity(NAU881x_t* nau881x, uint8_t invert);
nau881x_status_t NAU881x_Set_DAC_Gain(NAU881x_t* nau8810, uint8_t val);
nau881x_status_t NAU881x_Set_DAC_SoftMute(NAU881x_t* nau881x, uint8_t state);
nau881x_status_t NAU881x_Set_DAC_AutoMute(NAU881x_t* nau881x, uint8_t state);
nau881x_status_t NAU881x_Set_DAC_SampleRate(NAU881x_t* nau881x, nau881x_dac_samplerate_t rate);
nau881x_status_t NAU881x_Set_DAC_Limiter_Enable(NAU881x_t* nau881x, uint8_t enable);
nau881x_status_t NAU881x_Set_DAC_Limiter_AttackTime(NAU881x_t* nau881x, uint8_t val);
nau881x_status_t NAU881x_Set_DAC_Limiter_DecayTime(NAU881x_t* nau881x, uint8_t val);
nau881x_status_t NAU881x_Set_DAC_Limiter_VolumeBoost(NAU881x_t* nau881x, uint8_t value);
nau881x_status_t NAU881x_Set_DAC_Limiter_Threshold(NAU881x_t* nau881x, int8_t value);
nau881x_status_t NAU881x_Set_Equalizer_Path(NAU881x_t* nau881x, nau881x_eq_path_t path);
nau881x_status_t NAU881x_Set_Equalizer_Bandwidth(NAU881x_t* nau881x, uint8_t equalizer_no, nau881x_eq_bandwidth_t bandwidth);
nau881x_status_t NAU881x_Set_Equalizer_Gain(NAU881x_t* nau881x, uint8_t equalizer_no, int8_t value);
nau881x_status_t NAU881x_Set_Equalizer1_Frequency(NAU881x_t* nau881x, nau881x_eq1_cutoff_freq_t cutoff_freq);
nau881x_status_t NAU881x_Set_Equalizer2_Frequency(NAU881x_t* nau881x, nau881x_eq2_center_freq_t center_freq);
nau881x_status_t NAU881x_Set_Equalizer3_Frequency(NAU881x_t* nau881x, nau881x_eq3_center_freq_t center_freq);
nau881x_status_t NAU881x_Set_Equalizer4_Frequency(NAU881x_t* nau881x, nau881x_eq4_center_freq_t center_freq);
nau881x_status_t NAU881x_Set_Equalizer5_Frequency(NAU881x_t* nau881x, nau881x_eq5_cutoff_freq_t cutoff_freq);

#ifdef __cplusplus
}
#endif

#endif // MR_NAU881X_H

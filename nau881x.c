#include "nau881x.h"
#include "nau881x_regs.h"


nau881x_status_t NAU881x_Init(NAU881x_t* nau881x)
{
    // Datasheet page 64
    NAU881x_SoftwareReset(nau881x);

    // TODO: Power up sequence

    return NAU881X_STATUS_OK;
}

/* ----- Input path ----- */
nau881x_status_t NAU881x_Get_PGA_Input(NAU881x_t* nau881x, nau881x_input_t* input)
{
    uint16_t val = nau881x->_register[NAU881X_REG_INPUT_CTRL];
    *input = val & 0x0003;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_PGA_Input(NAU881x_t* nau881x, nau881x_input_t input)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_INPUT_CTRL];
    regval &= ~(0x0007);            // AUX bits in NAU8810 always zero
    regval |= (uint16_t)input;      // TODO: Return invalid for AUX if NAU8810
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_INPUT_CTRL, regval);
    nau881x->_register[NAU881X_REG_INPUT_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_PGA_Gain(NAU881x_t* nau881x, uint8_t val)
{
    if (val > 0x3F)
        return NAU881X_STATUS_INVALID;

    uint16_t regval = nau881x->_register[NAU881X_REG_PGA_GAIN_CTRL];
    regval &= ~(0x003F);
    regval |= (val & 0x003F);
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_PGA_GAIN_CTRL, regval);
    nau881x->_register[NAU881X_REG_PGA_GAIN_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_PGA_Gain_db(NAU881x_t* nau881x, float vol_db)
{
    if (vol_db < -12)
        return NAU881X_STATUS_INVALID;
    else if (vol_db >= 35.25)
        return NAU881X_STATUS_INVALID;

    // Round to floor by 0.75
    int16_t rounding = vol_db * 100;
    rounding %= 75;
    if (rounding < 0)
        rounding += 75;
    vol_db -= rounding / 100.0;
    // Convert to val for register
    uint8_t val = (vol_db + 12) / 0.75;
    return NAU881x_Set_PGA_Volume(nau881x, val);
}

uint8_t NAU881x_Get_PGA_Gain(NAU881x_t* nau881x)
{
    return nau881x->_register[NAU881X_REG_PGA_GAIN_CTRL] & 0x003F;
}

nau881x_status_t NAU881x_Set_PGA_Mute(NAU881x_t* nau881x, uint8_t state)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_PGA_GAIN_CTRL];
    regval &= ~(1 << 6);
    regval |= (state ? 1 : 0) << 6;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_PGA_GAIN_CTRL, regval);
    nau881x->_register[NAU881X_REG_PGA_GAIN_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_PGA_ZeroCross(NAU881x_t* nau881x, uint8_t state)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_PGA_GAIN_CTRL];
    regval &= ~(1 << 7);
    regval |= (state ? 1 : 0) << 7;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_PGA_GAIN_CTRL, regval);
    nau881x->_register[NAU881X_REG_PGA_GAIN_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_PGA_Enable(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_2];
    regval &= ~(1 << 2);
    regval |= (enable ? 1 : 0) << 2;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_POWER_MANAGEMENT_2, regval);
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_2] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU8814_Set_Aux_Enable(NAU881x_t* nau8814, uint8_t enable)
{
    uint16_t regval = nau8814->_register[NAU881X_REG_POWER_MANAGEMENT_1];
    regval &= ~(1 << 6);
    regval |= (enable ? 1 : 0) << 6;
    NAU881X_REG_WRITE(nau8814->comm_handle, NAU881X_REG_POWER_MANAGEMENT_1, regval);
    nau8814->_register[NAU881X_REG_POWER_MANAGEMENT_1] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU8814_Set_Aux_Mode(NAU881x_t* nau8814, nau881x_aux_mode_t mode)
{
    uint16_t regval = nau8814->_register[NAU881X_REG_INPUT_CTRL];
    regval &= ~(1 << 3);
    regval |= mode << 3;
    NAU881X_REG_WRITE(nau8814->comm_handle, NAU881X_REG_INPUT_CTRL, regval);
    nau8814->_register[NAU881X_REG_INPUT_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_PGA_Boost(NAU881x_t* nau881x, uint8_t state)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_ADC_BOOST_CTRL];
    regval &= ~(1 << 8);
    regval |= (state ? 1 : 0) << 8;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ADC_BOOST_CTRL, regval);
    nau881x->_register[NAU881X_REG_ADC_BOOST_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Boost_Volume(NAU881x_t* nau881x, nau881x_input_t input, uint8_t vol)
{
    if (vol > 0x07)
        return NAU881X_STATUS_INVALID;

    // TODO: Check if NAU8810 without #if macro because NAU8810 does not have AUX
// #if NAU881X_PART == NAU881X_PART_NAU8810
//     if (input != NAU881X_INPUT_MICP)
//         return NAU881X_STATUS_INVALID;
// #endif

    int8_t shift = -1;
    switch (input)
    {
        case NAU8814_INPUT_AUX: shift = 0; break;
        case NAU881X_INPUT_MICP: shift = 4; break;
        default: break;
    }
    if (shift < 0) return NAU881X_STATUS_INVALID;

    uint16_t regval = nau881x->_register[NAU881X_REG_ADC_BOOST_CTRL];
    regval &= ~(0x07 << shift);
    regval |= (vol & 0x07) << shift;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ADC_BOOST_CTRL, regval);
    nau881x->_register[NAU881X_REG_ADC_BOOST_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Boost_Enable(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_2];
    regval &= ~(1 << 4);
    regval |= (enable ? 1 : 0) << 4;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_POWER_MANAGEMENT_2, regval);
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_2] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_MicBias_Enable(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_1];
    regval &= ~(1 << 4);
    regval |= (enable ? 1 : 0) << 4;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_POWER_MANAGEMENT_1, regval);
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_1] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_MicBias_Voltage(NAU881x_t* nau881x, uint8_t val)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_INPUT_CTRL];
    regval &= ~(0x03 << 7);
    regval |= val << 7;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_INPUT_CTRL, regval);
    nau881x->_register[NAU881X_REG_INPUT_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_MicBiasMode_Enable(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_4];
    regval &= ~(1 << 4);
    regval |= (enable ? 1 : 0) << 4;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_POWER_MANAGEMENT_4, regval);
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_4] = regval;
    return NAU881X_STATUS_OK;
}

/* ----- ADC digital filter ----- */
nau881x_status_t NAU881x_Set_ADC_Enable(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_2];
    regval &= ~(1 << 0);
    regval |= (enable ? 1 : 0) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_POWER_MANAGEMENT_2, regval);
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_2] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ADC_Polarity(NAU881x_t* nau881x, uint8_t invert)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_ADC_CTRL];
    regval &= ~(1 << 0);
    regval |= (invert ? 1 : 0) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ADC_CTRL, regval);
    nau881x->_register[NAU881X_REG_ADC_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ADC_OverSampleRate(NAU881x_t* nau881x, nau881x_adc_oversamplerate_t rate)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_ADC_CTRL];
    regval &= ~(1 << 3);
    regval |= rate << 3;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ADC_CTRL, regval);
    nau881x->_register[NAU881X_REG_ADC_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ADC_HighPassFilter(NAU881x_t* nau881x, uint8_t enable, nau881x_hpf_mode_t mode, uint8_t freq_regval)
{
    if (freq_regval > 0x07)
        return NAU881X_STATUS_INVALID;

    uint16_t regval = nau881x->_register[NAU881X_REG_ADC_CTRL];
    regval &= ~(0x1F << 4);
    regval |= (enable ? 1 : 0) << 8;
    regval |= (mode ? 1 : 0) << 7;
    regval |= (freq_regval & 0x07) << 4;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ADC_CTRL, regval);
    nau881x->_register[NAU881X_REG_ADC_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ADC_Gain(NAU881x_t* nau881x, uint8_t regval)
{
    if (regval > 0xFF)
        return NAU881X_STATUS_INVALID;

    uint16_t val = 0;
    val |= (regval & 0xFF) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ADC_VOL, val);
    uint16_t regval = nau881x->_register[NAU881X_REG_ADC_VOL];
    return NAU881X_STATUS_OK;
}

/* ----- ALC ----- */
nau881x_status_t NAU881x_Set_ALC_Enable(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_ALC_CTRL_1];
    regval &= ~(1 << 8);
    regval |= (enable ? 1 : 0) << 8;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ALC_CTRL_1, regval);
    nau881x->_register[NAU881X_REG_ALC_CTRL_1] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ALC_Gain(NAU881x_t* nau881x, uint8_t minval, uint8_t maxval)
{
    if ((minval > 0x07) || (maxval > 0x07))
        return NAU881X_STATUS_INVALID;

    uint16_t regval = nau881x->_register[NAU881X_REG_ALC_CTRL_1];
    regval &= ~(0x3F << 0);
    regval |= (minval & 0x07) << 0;
    regval |= (maxval & 0x07) << 3;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ALC_CTRL_1, regval);
    nau881x->_register[NAU881X_REG_ALC_CTRL_1] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ALC_TargetLevel(NAU881x_t* nau881x, uint8_t val)
{
    if (val > 0x0F)
        return NAU881X_STATUS_INVALID;
    
    uint16_t regval = nau881x->_register[NAU881X_REG_ALC_CTRL_2];
    regval &= ~(0x0F << 0);
    regval |= (val & 0x0F) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ALC_CTRL_2, regval);
    nau881x->_register[NAU881X_REG_ALC_CTRL_2] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ALC_Hold(NAU881x_t* nau881x, uint8_t val)
{
    if (val > 0x0F)
        return NAU881X_STATUS_INVALID;
    
    uint16_t regval = nau881x->_register[NAU881X_REG_ALC_CTRL_2];
    regval &= ~(0x0F << 4);
    regval |= (val & 0x0F) << 4;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ALC_CTRL_2, regval);
    nau881x->_register[NAU881X_REG_ALC_CTRL_2] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ALC_Mode(NAU881x_t* nau881x, nau881x_alc_mode_t mode)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_ALC_CTRL_3];
    regval &= ~(1 << 8);
    regval |= (mode & 0x01) << 8;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ALC_CTRL_3, regval);
    nau881x->_register[NAU881X_REG_ALC_CTRL_3] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ALC_AttackTime(NAU881x_t* nau881x, uint8_t val)
{
    if (val > 0x0F)
        return NAU881X_STATUS_INVALID;
    
    uint16_t regval = nau881x->_register[NAU881X_REG_ALC_CTRL_3];
    regval &= ~(0x0F << 0);
    regval |= (val & 0x0F) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ALC_CTRL_3, regval);
    nau881x->_register[NAU881X_REG_ALC_CTRL_3] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ALC_DecayTime(NAU881x_t* nau881x, uint8_t val)
{
    if (val > 0x0F)
        return NAU881X_STATUS_INVALID;
    
    uint16_t regval = nau881x->_register[NAU881X_REG_ALC_CTRL_3];
    regval &= ~(0x0F << 4);
    regval |= (val & 0x0F) << 4;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ALC_CTRL_3, regval);
    nau881x->_register[NAU881X_REG_ALC_CTRL_3] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ALC_ZeroCross(NAU881x_t* nau881x, uint8_t state)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_ALC_CTRL_2];
    regval &= ~(1 << 8);
    regval |= (state ? 1 : 0) << 8;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ALC_CTRL_2, regval);
    nau881x->_register[NAU881X_REG_ALC_CTRL_2] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ALC_NoiseGate_Threshold(NAU881x_t* nau881x, uint8_t val)
{
    if (val > 7)
        return NAU881X_STATUS_INVALID;

    uint16_t regval = nau881x->_register[NAU881X_REG_NOISE_GATE];
    regval &= ~(0x07 << 0);
    regval |= (val & 0x07) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_NOISE_GATE, regval);
    nau881x->_register[NAU881X_REG_NOISE_GATE] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_ALC_NoiseGate_Enable(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_NOISE_GATE];
    regval &= ~(1 << 3);
    regval |= (enable ? 1 : 0) << 3;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_NOISE_GATE, regval);
    nau881x->_register[NAU881X_REG_NOISE_GATE] = regval;
    return NAU881X_STATUS_OK;
}

/* ----- DAC digital filter ----- */
nau881x_status_t NAU881x_Set_ADC_DAC_Passtrough(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_COMPANDING_CTRL];
    regval &= ~(1 << 0);
    regval |= (enable ? 1 : 0) << 1;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_COMPANDING_CTRL, regval);
    nau881x->_register[NAU881X_REG_COMPANDING_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_Enable(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_3];
    regval &= ~(1 << 0);
    regval |= (enable ? 1 : 0) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_POWER_MANAGEMENT_3, regval);
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_3] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_Polarity(NAU881x_t* nau881x, uint8_t invert)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_DAC_CTRL];
    regval &= ~(1 << 0);
    regval |= (invert ? 1 : 0) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_DAC_CTRL, regval);
    nau881x->_register[NAU881X_REG_DAC_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_Gain(NAU881x_t* nau881x, uint8_t val)
{
    uint16_t regval = 0;
    regval |= (val & 0xFF) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_DAC_VOL, regval);
    nau881x->_register[NAU881X_REG_DAC_VOL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_SoftMute(NAU881x_t* nau881x, uint8_t state)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_DAC_CTRL];
    regval &= ~(1 << 6);
    regval |= (state ? 1 : 0) << 6;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_DAC_CTRL, regval);
    nau881x->_register[NAU881X_REG_DAC_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_AutoMute(NAU881x_t* nau881x, uint8_t state)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_DAC_CTRL];
    regval &= ~(1 << 2);
    regval |= (state ? 1 : 0) << 2;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_DAC_CTRL, regval);
    nau881x->_register[NAU881X_REG_DAC_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_SampleRate(NAU881x_t* nau881x, nau881x_dac_samplerate_t rate)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_DAC_CTRL];
    regval &= ~(0x03 << 4);
    regval |= rate << 4;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_DAC_CTRL, regval);
    nau881x->_register[NAU881X_REG_DAC_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_Limiter_Enable(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_DAC_LIMITER_1];
    regval &= ~(1 << 8);
    regval |= (enable ? 1 : 0) << 8;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_DAC_LIMITER_1, regval);
    nau881x->_register[NAU881X_REG_DAC_LIMITER_1] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_Limiter_AttackTime(NAU881x_t* nau881x, uint8_t val)
{
    if (val > 0x0F)
        return NAU881X_STATUS_INVALID;
    
    uint16_t regval = nau881x->_register[NAU881X_REG_DAC_LIMITER_1];
    regval &= ~(0x0F << 0);
    regval |= (val & 0x0F) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_DAC_LIMITER_1, regval);
    nau881x->_register[NAU881X_REG_DAC_LIMITER_1] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_Limiter_DecayTime(NAU881x_t* nau881x, uint8_t val)
{
    if (val > 0x0F)
        return NAU881X_STATUS_INVALID;
    
    uint16_t regval = nau881x->_register[NAU881X_REG_DAC_LIMITER_1];
    regval &= ~(0x0F << 4);
    regval |= (val & 0x0F) << 4;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_DAC_LIMITER_1, regval);
    nau881x->_register[NAU881X_REG_DAC_LIMITER_1] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_Limiter_VolumeBoost(NAU881x_t* nau881x, uint8_t value)
{
    if (value > 12)
        return NAU881X_STATUS_INVALID;
    uint16_t regval = nau881x->_register[NAU881X_REG_DAC_LIMITER_2];
    regval &= ~(0x0F << 0);
    regval |= (value) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_DAC_LIMITER_2, regval);
    nau881x->_register[NAU881X_REG_DAC_LIMITER_2] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_DAC_Limiter_VolumeBoost(NAU881x_t* nau881x, int8_t value)
{
    if ((value >= 0) || (value < -6))
        return NAU881X_STATUS_INVALID;
    uint16_t regval = nau881x->_register[NAU881X_REG_DAC_LIMITER_2];
    value = value * (-1) - 1;
    regval &= ~(0x07 << 4);
    regval |= (value) << 4;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_DAC_LIMITER_2, regval);
    nau881x->_register[NAU881X_REG_DAC_LIMITER_2] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Equalizer_Path(NAU881x_t* nau881x, nau881x_eq_path_t path)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_EQ_1];
    regval &= ~(1 << 8);
    regval |= path << 8;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_EQ_1, regval);
    nau881x->_register[NAU881X_REG_EQ_1] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Equalizer_Bandwidth(NAU881x_t* nau881x, uint8_t equalizer_no, nau881x_eq_bandwidth_t bandwidth)
{
    if ((equalizer_no < 2) || (equalizer_no >= 5))
        return NAU881X_STATUS_INVALID;
    
    uint8_t regnum = NAU881X_REG_EQ_1 + equalizer_no - 1;
    uint16_t regval = nau881x->_register[regnum];
    regval &= ~(1 << 8);
    regval |= bandwidth << 8;
    NAU881X_REG_WRITE(nau881x->comm_handle, regnum, regval);
    nau881x->_register[regnum] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Equalizer_Gain(NAU881x_t* nau881x, uint8_t equalizer_no, int8_t value)
{
    if ((value < -12) || (value > 12))
        return NAU881X_STATUS_INVALID;
    
    uint8_t val = (value * (-1) + 12) & 0x1F;
    uint8_t regnum = NAU881X_REG_EQ_1 + equalizer_no - 1;
    uint16_t regval = nau881x->_register[regnum];
    regval &= ~(0x1F << 0);
    regval |= val << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, regnum, regval);
    nau881x->_register[regnum] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Equalizer1_Frequency(NAU881x_t* nau881x, nau881x_eq1_cutoff_freq_t cutoff_freq)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_EQ_1];
    regval &= ~(0x03 << 5);
    regval |= cutoff_freq << 5;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_EQ_1, regval);
    nau881x->_register[NAU881X_REG_EQ_1] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Equalizer2_Frequency(NAU881x_t* nau881x, nau881x_eq2_center_freq_t center_freq)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_EQ_2];
    regval &= ~(0x03 << 5);
    regval |= center_freq << 5;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_EQ_2, regval);
    nau881x->_register[NAU881X_REG_EQ_2] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Equalizer3_Frequency(NAU881x_t* nau881x, nau881x_eq3_center_freq_t center_freq)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_EQ_3];
    regval &= ~(0x03 << 5);
    regval |= center_freq << 5;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_EQ_3, regval);
    nau881x->_register[NAU881X_REG_EQ_3] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Equalizer4_Frequency(NAU881x_t* nau881x, nau881x_eq4_center_freq_t center_freq)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_EQ_4];
    regval &= ~(0x03 << 5);
    regval |= center_freq << 5;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_EQ_4, regval);
    nau881x->_register[NAU881X_REG_EQ_4] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Equalizer5_Frequency(NAU881x_t* nau881x, nau881x_eq5_cutoff_freq_t cutoff_freq)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_EQ_5];
    regval &= ~(0x03 << 5);
    regval |= cutoff_freq << 5;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_EQ_5, regval);
    nau881x->_register[NAU881X_REG_EQ_5] = regval;
    return NAU881X_STATUS_OK;
}

/* ----- Analog outputs ----- */
nau881x_status_t NAU881x_Set_Output_Enable(NAU881x_t* nau881x, nau881x_output_t output)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_3];
    regval &= ~(NAU881X_PM3_SPKMIX | NAU881X_PM3_SPKP | NAU881X_PM3_SPKN);
    regval &= ~(NAU881X_PM3_MOUTMIX | NAU881X_PM3_MOUT);

    if (output & NAU881X_OUTPUT_SPK)
        regval |= NAU881X_PM3_SPKMIX | NAU881X_PM3_SPKP | NAU881X_PM3_SPKN;
    if (output & NAU881X_OUTPUT_MOUT)
        regval |= NAU881X_PM3_MOUTMIX | NAU881X_PM3_MOUT;

    if (output & NAU881X_OUTPUT_MOUT_DIFFERENTIAL)
    {
        // TODO: SPKMOUT
        regval |= NAU881X_PM3_MOUTMIX | NAU881X_PM3_MOUT | NAU881X_PM3_SPKN;
    }

    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_POWER_MANAGEMENT_3, regval);
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_3] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Speaker_Source(NAU881x_t* nau881x, nau881x_output_source_t source)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_SPK_MIXER_CTRL];
    regval &= ~(0x03 << 0);
    regval &= ~(1 << 5);
    if (source & NAU8814_OUTPUT_FROM_AUX) source = (source & NAU8814_OUTPUT_FROM_AUX) | (1 << 5);
    regval |= source;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_SPK_MIXER_CTRL, regval);
    nau881x->_register[NAU881X_REG_SPK_MIXER_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Speaker_FromBypass_Attenuation(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_ATTN_CTRL];
    regval &= ~(1 << 1);
    regval |= (enable ? 1 : 0) << 1;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ATTN_CTRL, regval);
    nau881x->_register[NAU881X_REG_ATTN_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Speaker_Boost(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_OUTPUT_CTRL];
    regval &= ~(1 << 2);
    regval |= (enable ? 1 : 0) << 2;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_OUTPUT_CTRL, regval);
    nau881x->_register[NAU881X_REG_OUTPUT_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Speaker_ZeroCross(NAU881x_t* nau881x, uint8_t state)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_SPK_VOL_CTRL];
    regval &= ~(1 << 7);
    regval |= (state ? 1 : 0) << 7;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_SPK_VOL_CTRL, regval);
    nau881x->_register[NAU881X_REG_SPK_VOL_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Speaker_Mute(NAU881x_t* nau881x, uint8_t state)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_SPK_VOL_CTRL];
    regval &= ~(1 << 6);
    regval |= (state ? 1 : 0) << 6;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_SPK_VOL_CTRL, regval);
    nau881x->_register[NAU881X_REG_SPK_VOL_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Speaker_Volume(NAU881x_t* nau881x, uint8_t val)
{
    if (val > 0x3F)
        return NAU881X_STATUS_INVALID;

    uint16_t regval = nau881x->_register[NAU881X_REG_SPK_VOL_CTRL];
    regval &= ~(0x3F << 0);
    regval |= (val & 0x3F) << 0;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_SPK_VOL_CTRL, regval);
    nau881x->_register[NAU881X_REG_SPK_VOL_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Speaker_Volume_db(NAU881x_t* nau881x, int8_t vol_db)
{
    if ((vol_db < -57) || (vol_db >= 6))
        return NAU881X_STATUS_INVALID;

    return NAU881x_Set_Speaker_Volume(nau881x, NAU881X_SPKVOL_DB_TO_REG_VALUE(vol_db));
}

uint8_t NAU881x_Get_Speaker_Volume(NAU881x_t* nau881x)
{
    return nau881x->_register[NAU881X_REG_SPK_VOL_CTRL] & 0x3F;
}

uint8_t NAU881x_Get_Speaker_Volume_db(NAU881x_t* nau881x)
{
    return NAU881X_SPKVOL_REG_VALUE_TO_DB(NAU881x_Get_Speaker_Volume(nau881x));
}

nau881x_status_t NAU881X_Set_Mono_Source(NAU881x_t* nau881x, nau881x_output_source_t source)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_MONO_MIXER_CTRL];
    regval &= ~(0x07 << 0);
    regval |= source;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_MONO_MIXER_CTRL, regval);
    nau881x->_register[NAU881X_REG_MONO_MIXER_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Mono_FromBypass_Attenuation(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_ATTN_CTRL];
    regval &= ~(1 << 2);
    regval |= (enable ? 1 : 0) << 2;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_ATTN_CTRL, regval);
    nau881x->_register[NAU881X_REG_ATTN_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Mono_Boost(NAU881x_t* nau881x, uint8_t enable)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_OUTPUT_CTRL];
    regval &= ~(1 << 3);
    regval |= (enable ? 1 : 0) << 3;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_OUTPUT_CTRL, regval);
    nau881x->_register[NAU881X_REG_OUTPUT_CTRL] = regval;
    return NAU881X_STATUS_OK;
}

nau881x_status_t NAU881x_Set_Mono_Mute(NAU881x_t* nau881x, uint8_t state)
{
    uint16_t regval = nau881x->_register[NAU881X_REG_MONO_MIXER_CTRL];
    regval &= ~(1 << 6);
    regval |= (state ? 1 : 0) << 6;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_MONO_MIXER_CTRL, regval);
    nau881x->_register[NAU881X_REG_MONO_MIXER_CTRL] = regval;

    regval = nau881x->_register[NAU881X_REG_HIGH_VOLTAGE_CTRL];
    regval &= ~(1 << 4);
    regval |= (state ? 1 : 0) << 4;
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_HIGH_VOLTAGE_CTRL, regval);
    nau881x->_register[NAU881X_REG_HIGH_VOLTAGE_CTRL] = regval;

    return NAU881X_STATUS_OK;
}

/* ----- Reset ----- */
nau881x_status_t NAU881x_SoftwareReset(NAU881x_t* nau881x)
{
    NAU881X_REG_WRITE(nau881x->comm_handle, NAU881X_REG_SOFTWARE_RESET, 1);

    // Set register default values based on datasheet
    // Software reset register does not need to be set
    // Power management
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_1] = 0x0000;
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_2] = 0x0000;
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_3] = 0x0000;
    // Audio control
    nau881x->_register[NAU881X_REG_AUDIO_INTERFACE] = 0x0050;
    nau881x->_register[NAU881X_REG_COMPANDING_CTRL] = 0x0000;
    nau881x->_register[NAU881X_REG_CLOCK_CTRL_1] = 0x0140;
    nau881x->_register[NAU881X_REG_CLOCK_CTRL_2] = 0x0000;
    nau881x->_register[NAU8814_REG_GPIO_CTRL] = 0x0000;
    nau881x->_register[NAU881X_REG_DAC_CTRL] = 0x0000;
    nau881x->_register[NAU881X_REG_DAC_VOL] = 0x00FF;
    nau881x->_register[NAU881X_REG_ADC_CTRL] = 0x0100;
    nau881x->_register[NAU881X_REG_ADC_VOL] = 0x00FF;
    // Equalizer
    nau881x->_register[NAU881X_REG_EQ_1] = 0x012C;
    nau881x->_register[NAU881X_REG_EQ_2] = 0x002C;
    nau881x->_register[NAU881X_REG_EQ_3] = 0x002C;
    nau881x->_register[NAU881X_REG_EQ_4] = 0x002C;
    nau881x->_register[NAU881X_REG_EQ_5] = 0x002C;
    // DAC limiter
    nau881x->_register[NAU881X_REG_DAC_LIMITER_1] = 0x0032;
    nau881x->_register[NAU881X_REG_DAC_LIMITER_2] = 0x0000;
    // Notch filter
    nau881x->_register[NAU881X_REG_NOTCH_FILTER_0_H] = 0x0000;
    nau881x->_register[NAU881X_REG_NOTCH_FILTER_0_L] = 0x0000;
    nau881x->_register[NAU881X_REG_NOTCH_FILTER_1_H] = 0x0000;
    nau881x->_register[NAU881X_REG_NOTCH_FILTER_1_L] = 0x0000;
    // ALC control
    nau881x->_register[NAU881X_REG_ALC_CTRL_1] = 0x0038;
    nau881x->_register[NAU881X_REG_ALC_CTRL_2] = 0x000B;
    nau881x->_register[NAU881X_REG_ALC_CTRL_3] = 0x0032;
    nau881x->_register[NAU881X_REG_NOISE_GATE] = 0x0000;
    // PLL control
    nau881x->_register[NAU881X_REG_PLL_N] = 0x0008;
    nau881x->_register[NAU881X_REG_PLL_K1] = 0x000C;
    nau881x->_register[NAU881X_REG_PLL_K2] = 0x0093;
    nau881x->_register[NAU881X_REG_PLL_K3] = 0x00E9;
    // Input, putput & mixer control
    nau881x->_register[NAU881X_REG_ATTN_CTRL] = 0x0000;
    nau881x->_register[NAU881X_REG_INPUT_CTRL] = 0x0003;
    nau881x->_register[NAU881X_REG_PGA_GAIN_CTRL] = 0x0010;
    nau881x->_register[NAU881X_REG_ADC_BOOST_CTRL] = 0x0100;
    nau881x->_register[NAU881X_REG_OUTPUT_CTRL] = 0x0002;
    nau881x->_register[NAU881X_REG_SPK_MIXER_CTRL] = 0x0001;
    nau881x->_register[NAU881X_REG_SPK_VOL_CTRL] = 0x0039;
    nau881x->_register[NAU881X_REG_MONO_MIXER_CTRL] = 0x0001;
    // Low power control
    nau881x->_register[NAU881X_REG_POWER_MANAGEMENT_4] = 0x0000;
    // PCM time slot & ADCOUT impedance option control
    nau881x->_register[NAU881X_REG_TIMESLOT] = 0x0000;
    nau881x->_register[NAU881X_REG_ADCOUT_DRIVE] = 0x0020;
    // Register ID
    nau881x->_register[NAU881X_REG_SILICON_REV] = 0x00EF;
    nau881x->_register[NAU881X_REG_2WIRE_ID] = 0x001A;
    nau881x->_register[NAU881X_REG_ADDITIONAL_ID] = 0x00CA;
    // reserved reg does not need to be set
    nau881x->_register[NAU881X_REG_HIGH_VOLTAGE_CTRL] = 0x0001;
    nau881x->_register[NAU881X_REG_ALC_ENHANCEMENTS_1] = 0x0000;
    nau881x->_register[NAU881X_REG_ALC_ENHANCEMENTS_2] = 0x0039;
    nau881x->_register[NAU881X_REG_ADDITIONAL_IF_CTRL] = 0x0000;
    nau881x->_register[NAU881X_REG_POWER_TIE_OFF_CTRL] = 0x0000;
    nau881x->_register[NAU881X_REG_AGC_P2P_DET] = 0x0000;
    nau881x->_register[NAU881X_REG_AGC_PEAK_DET] = 0x0000;
    nau881x->_register[NAU881X_REG_CTRL_AND_STATUS] = 0x0000;
    nau881x->_register[NAU881X_REG_OUTPUT_TIE_OFF_CTRL] = 0x0000;

    
    return NAU881X_STATUS_OK;
}

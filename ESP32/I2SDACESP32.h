#pragma once   // 预处理指令，能够保证cpp只会被编译一次

#include "./driver/ledc.h"
#include "./driver/i2s.h"
#include "./freertos/portmacro.h"

#ifndef _I2SDACESP32_
#define _I2SDACESP32_

static i2s_config_t i2s_config_dac;
static i2s_port_t i2s_num_dac=(i2s_port_t)0;

// --------- Make A CLass ----------
class I2SDAC{
// Public Variables & Methdods(Inlcuding Building Function)
public:
    unsigned long samplerate;
    unsigned char chn=0;//0:right 1:left

    // The Building Function
    I2SDAC(unsigned char chn_){
        chn=chn_;
    }

    ~I2SDAC(){
        i2s_driver_uninstall(i2s_num_dac);
    }


    void begin(unsigned long samplerate_){
        samplerate=samplerate_;

        i2s_config_dac.mode=i2s_mode_t(I2S_MODE_MASTER|I2S_MODE_TX|I2S_MODE_DAC_BUILT_IN);
        i2s_config_dac.sample_rate=samplerate;
        i2s_config_dac.bits_per_sample = (i2s_bits_per_sample_t)16;
        if(chn==0)
            i2s_config_dac.channel_format=I2S_CHANNEL_FMT_ONLY_RIGHT;
        else
            i2s_config_dac.channel_format=I2S_CHANNEL_FMT_ONLY_LEFT;
        i2s_config_dac.communication_format=(i2s_comm_format_t)(I2S_COMM_FORMAT_I2S_MSB);
        i2s_config_dac.intr_alloc_flags=0;
        i2s_config_dac.dma_buf_count=8;
        i2s_config_dac.dma_buf_len=128;
        i2s_config_dac.use_apll=false;

        i2s_driver_install(i2s_num_dac,&i2s_config_dac,0,NULL);
        i2s_set_pin(i2s_num_dac,NULL);
        if(chn==0)
            i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);
        else
            i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN);
        i2s_set_sample_rates(i2s_num_dac,samplerate);
    }

    void write(unsigned char in){
        unsigned short dat;
        dat=(unsigned short)in<<8;
        i2s_write_bytes(i2s_num_dac,(char*)(&dat),2,(TickType_t)portMAX_DELAY);
    }

};

#endif


// ----------------- Usage -------------------
// dac0 = I2SDAC(0)
// dac0.begin(samplerate)
// dac0.write(vol)
// -------------------------------------------

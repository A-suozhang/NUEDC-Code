#pragma once

#include "./driver/ledc.h"
#include "./driver/i2s.h"
#include "./freertos/portmacro.h"

#ifndef _CS5343HEADER_
#define _CS5343HEADER_

static ledc_timer_config_t ledc_timer;
static ledc_channel_config_t ledc_channel;
static i2s_config_t i2s_config;
static i2s_pin_config_t i2s_pins;
static i2s_port_t i2s_num=(i2s_port_t)0;

class CS5343{
    public:
    unsigned char SDOUT_PIN,SCLK_PIN,LRCK_PIN,MCLK_PIN;
    unsigned int samplerate;
    CS5343(unsigned char sdout,unsigned char sclk,unsigned char lrck,unsigned char mclk){
        SDOUT_PIN=sdout;SCLK_PIN=sclk;LRCK_PIN=lrck;MCLK_PIN=mclk;
    }
    ~CS5343(){

    }
    
    void begin(unsigned int samplerate_){
        samplerate=samplerate_;

        //config MCLK
        ledc_timer.bit_num=(ledc_timer_bit_t)2;
        ledc_timer.freq_hz=/*4608000*/4000000;
        ledc_timer.speed_mode=LEDC_HIGH_SPEED_MODE;
        ledc_timer.timer_num=LEDC_TIMER_0;
        ledc_channel.channel    = LEDC_CHANNEL_0;
        ledc_channel.gpio_num   = MCLK_PIN;
        ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
        ledc_channel.timer_sel  = LEDC_TIMER_0;
        ledc_channel.duty       = 2;

        ledc_timer_config(&ledc_timer);
        ledc_channel_config(&ledc_channel);

        //config I2S
        i2s_config.mode=i2s_mode_t(I2S_MODE_MASTER|I2S_MODE_RX);
        i2s_config.sample_rate=samplerate_;
        i2s_config.bits_per_sample = (i2s_bits_per_sample_t)32;
        i2s_config.channel_format=I2S_CHANNEL_FMT_RIGHT_LEFT;
        i2s_config.communication_format=(i2s_comm_format_t)(I2S_COMM_FORMAT_I2S|I2S_COMM_FORMAT_I2S_MSB);
        i2s_config.intr_alloc_flags=0;
        i2s_config.dma_buf_count=8;
        i2s_config.dma_buf_len=128;
        i2s_config.use_apll=false;
        i2s_pins.bck_io_num=SCLK_PIN;
        i2s_pins.ws_io_num=LRCK_PIN;
        i2s_pins.data_in_num=SDOUT_PIN;
        i2s_pins.data_out_num=I2S_PIN_NO_CHANGE;

        i2s_driver_install(i2s_num,&i2s_config,0,NULL);
        i2s_set_pin(i2s_num,&i2s_pins);
        i2s_set_sample_rates(i2s_num,samplerate);
    }

    int getbytes(unsigned char* dest,unsigned int len){
        return i2s_read_bytes(i2s_num,(char*)dest,(size_t)len,(TickType_t)portMAX_DELAY);
    }

    long read(unsigned char ch){
        long inbuf[2];
        i2s_read_bytes(i2s_num,(char*)inbuf,(size_t)(2*sizeof(long)),(TickType_t)portMAX_DELAY);
        return inbuf[ch];   //dangerous
    }
};

#endif

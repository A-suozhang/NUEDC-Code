#include <stdio.h>
#include <Arduino.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_system.h"
#include <HardwareSerial.h>
#include <String.h>
#include <SPI.h>

#define SAMPLE_RATE1     (3125)
#define NUM_DATA          (32)
#define OVER_SAMPLE       (1)
#define I2S_BCK_IO      (GPIO_NUM_26)
#define I2S_WS_IO       (GPIO_NUM_25)
#define I2S_DO_IO       (GPIO_NUM_12)


char clk_out[8] = {0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF};

void i2sconfig(){
    i2s_config_t i2s_config0;
    i2s_config0.mode = (i2s_mode_t)(I2S_MODE_SLAVE | I2S_MODE_RX);                                 
    i2s_config0.sample_rate = SAMPLE_RATE1;
    i2s_config0.bits_per_sample = (i2s_bits_per_sample_t)(16);
    i2s_config0.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;                           //2-channels
    i2s_config0.communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);
    i2s_config0.dma_buf_count = 2;
    i2s_config0.dma_buf_len = 60;
    i2s_config0.use_apll = false;
    i2s_config0.intr_alloc_flags = 0;    

    i2s_config_t i2s_config1;
    i2s_config1.mode = (i2s_mode_t)(I2S_MODE_SLAVE | I2S_MODE_TX);                                 
    i2s_config1.sample_rate = SAMPLE_RATE1;
    i2s_config1.bits_per_sample = (i2s_bits_per_sample_t)(16);
    i2s_config1.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;                           //2-channels
    i2s_config1.communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);
    i2s_config1.dma_buf_count = 10;
    i2s_config1.dma_buf_len = 128;
    i2s_config1.use_apll = false;
    i2s_config1.intr_alloc_flags = 0;   

    i2s_pin_config_t i2s_config_pin0;
    i2s_config_pin0.bck_io_num = I2S_BCK_IO;
    i2s_config_pin0.ws_io_num = I2S_WS_IO;
    i2s_config_pin0.data_out_num = I2S_PIN_NO_CHANGE;
    i2s_config_pin0.data_in_num = I2S_DO_IO;

    i2s_pin_config_t i2s_config_pin1;
    i2s_config_pin1.bck_io_num = I2S_BCK_IO;
    i2s_config_pin1.ws_io_num = I2S_WS_IO;
    i2s_config_pin1.data_out_num = 13;
    i2s_config_pin1.data_in_num = I2S_PIN_NO_CHANGE;


    i2s_driver_install((i2s_port_t)(I2S_NUM_0),&(i2s_config0), 0, NULL);
    i2s_set_pin((i2s_port_t)(I2S_NUM_0),&i2s_config_pin0);

    i2s_driver_install((i2s_port_t)(I2S_NUM_1),&(i2s_config1), 0, NULL);
    i2s_set_pin((i2s_port_t)(I2S_NUM_1),&i2s_config_pin1);

}

byte data2recive2[NUM_DATA*OVER_SAMPLE];
char dataOverSample[NUM_DATA*OVER_SAMPLE*8];

size_t i2s_byte_write0 = 0;
size_t i2s_byte_write1 = 0;

unsigned long temp_millisl;
unsigned long lasttime;

void setup(){
    Serial.begin(115200);
    i2sconfig();
    
}
void loop(){
    // * ------------- Read Data Through I2S Into data2recive ----------------------------

    i2s_read(I2S_NUM_0,data2recive2,32*OVER_SAMPLE,&i2s_byte_write1,100);
    i2s_write(I2S_NUM_1,clk_out,8,&i2s_byte_write0,100);

    temp_millisl = millis();
    if (temp_millisl - lasttime > 1000){
        lasttime = temp_millisl;
        for (int i=0;i<32;i++){
            Serial.print(data2recive2[i]);
            Serial.print(" ");
        }
        Serial.println(" ");
    }
    // * --------------- Convert The Byte Field To BInary Char Array --------------------- //
    // for (int i = 0;i<NUM_DATA*OVER_SAMPLE;i++){
    //     char s[9];
    //     itoa((int)(data2recive2[i]),s,2);
    // }
    


    // * --------------- Find The Max Pulse Of Data & Determine Data Rate ---------------- //

    // * --------------- Create & Sync The CLK ------------------------------------------ //

}   

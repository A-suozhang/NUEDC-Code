#include <stdio.h>
#include <Arduino.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_system.h"

#define SAMPLE_RATE0     (312500)
#define SAMPLE_RATE1     (3125)
#define I2S_BCK_IO      (GPIO_NUM_26)
#define I2S_WS_IO       (GPIO_NUM_25)
#define I2S_DO_IO       (GPIO_NUM_15)

void i2sconfig(){
    i2s_config_t i2s_config0;
    i2s_config0.mode = (i2s_mode_t)(I2S_MODE_SLAVE | I2S_MODE_RX);                                 
    i2s_config0.sample_rate = SAMPLE_RATE0;
    i2s_config0.bits_per_sample = (i2s_bits_per_sample_t)(16);
    i2s_config0.channel_format = I2S_CHANNEL_FMT_ALL_RIGHT;                           //2-channels
    i2s_config0.communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);
    i2s_config0.dma_buf_count = 10;
    i2s_config0.dma_buf_len = 60;
    i2s_config0.use_apll = false;
    i2s_config0.intr_alloc_flags = 0;    

    i2s_pin_config_t i2s_config_pin0;
    i2s_config_pin0.bck_io_num = I2S_BCK_IO;
    i2s_config_pin0.ws_io_num = I2S_WS_IO;
    i2s_config_pin0.data_out_num = -1;
    i2s_config_pin0.data_in_num = I2S_DO_IO;


    i2s_driver_install((i2s_port_t)(I2S_NUM_0),&(i2s_config0), 0, NULL);
    i2s_set_pin((i2s_port_t)(I2S_NUM_0),&i2s_config_pin0);

}

byte data2recive2[32];
char 

size_t i2s_byte_write0 = 0;
size_t i2s_byte_write1 = 0;

unsigned long temp_millisl;
unsigned long lasttime;

void setup(){
    Serial.begin(115200);
    i2sconfig();
    
}
void loop(){
    // i2s_write(I2S_NUM_0,data2send,512,&i2s_byte_write,100);
    
    // Serial.print("Next!");
    // i2s_write(I2S_NUM_1,data2send2,32,&i2s_byte_write1,100);
    // i2s_write(I2S_NUM_0,data2send2,32,&i2s_byte_write0,100);
    
    i2s_read(I2S_NUM_0,data2recive2,32,&i2s_byte_write1,100);

    temp_millisl = millis();
    if (temp_millisl - lasttime > 1000){
        lasttime = temp_millisl;
        for (int i=0;i<32;i++){
            Serial.print(data2recive2[i]);
            Serial.print(" ");
        }
        Serial.println(" ");
    }

    
    // Serial.println(i2s_byte_write1);

    // for (int i = 0;i<10;i++){
    //     i2s_set_sample_rates(I2S_NUM_0,(uint32_t)(3125));    // CLk_DIV TOO LARGE
    //     delay(2000);
    // }
    // i2s_set_sample_rates(I2S_NUM_0, 625000);
    
    // Serial.println(i2s_byte_write);
}   

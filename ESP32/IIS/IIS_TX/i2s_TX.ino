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
#define I2S_DO_IO       (GPIO_NUM_12)

void i2sconfig(){
    i2s_config_t i2s_config0;
    i2s_config0.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);                                 
    i2s_config0.sample_rate = SAMPLE_RATE1;
    i2s_config0.bits_per_sample = (i2s_bits_per_sample_t)(16);
    i2s_config0.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;                           //2-channels
    i2s_config0.communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);
    i2s_config0.dma_buf_count = 6;
    i2s_config0.dma_buf_len = 60;
    i2s_config0.use_apll = false;
    i2s_config0.intr_alloc_flags = 0;    

    // * I2S1 Also Writes Data
    // i2s_config_t i2s_config1;
    // i2s_config1.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);                                 
    // i2s_config1.sample_rate = SAMPLE_RATE1;
    // i2s_config1.bits_per_sample = (i2s_bits_per_sample_t)(16);
    // i2s_config1.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;                           //2-channels
    // i2s_config1.communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);
    // i2s_config1.dma_buf_count = 6;
    // i2s_config1.dma_buf_len = 60;
    // i2s_config1.use_apll = false;
    // i2s_config1.intr_alloc_flags = 0;   

    // *I2S1 Reads Data From I2S0
    // i2s_config_t i2s_config1;
    // i2s_config1.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX);                                 
    // i2s_config1.sample_rate = SAMPLE_RATE0;
    // i2s_config1.bits_per_sample = (i2s_bits_per_sample_t)(16);
    // i2s_config1.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;                           //2-channels
    // i2s_config1.communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);
    // i2s_config1.dma_buf_count = 10;
    // i2s_config1.dma_buf_len = 60;
    // i2s_config1.use_apll = false;
    // i2s_config1.intr_alloc_flags = 0; 

    i2s_pin_config_t i2s_config_pin0;
    i2s_config_pin0.bck_io_num = I2S_BCK_IO;
    i2s_config_pin0.ws_io_num = I2S_WS_IO;
    i2s_config_pin0.data_out_num = I2S_DO_IO;
    i2s_config_pin0.data_in_num = I2S_PIN_NO_CHANGE;

    // i2s_pin_config_t i2s_config_pin1;
    // i2s_config_pin1.bck_io_num = GPIO_NUM_15;
    // i2s_config_pin1.ws_io_num = GPIO_NUM_2;
    // i2s_config_pin1.data_out_num = GPIO_NUM_13;
    // i2s_config_pin1.data_in_num = I2S_PIN_NO_CHANGE;

    // i2s_pin_config_t i2s_config_pin1;
    // i2s_config_pin1.bck_io_num = GPIO_NUM_15;
    // i2s_config_pin1.ws_io_num = GPIO_NUM_2;
    // i2s_config_pin1.data_out_num = I2S_PIN_NO_CHANGE;
    // i2s_config_pin1.data_in_num = GPIO_NUM_4;


    i2s_driver_install((i2s_port_t)(I2S_NUM_0),&(i2s_config0), 0, NULL);
    i2s_set_pin((i2s_port_t)(I2S_NUM_0),&i2s_config_pin0);

    // i2s_driver_install((i2s_port_t)(I2S_NUM_1),&(i2s_config1), 0, NULL);
    // i2s_set_pin((i2s_port_t)(I2S_NUM_1),&i2s_config_pin1);
}


byte data2send2[32] = {1, 28, 75, 129, 146, 110, 65, 91, 89, 97, 246, 245, 209, 13, 143, 
57, 139, 72, 165, 78, 236, 247, 233, 154, 140, 29, 87, 202, 19, 252, 
47, 26};

byte data2send[512] = {0, 16, 51, 80, 174, 236, 7, 73, 70, 118, 51, 241, 82, 200, 45, 241, \
114, 174, 140, 172, 170, 160, 30, 32, 70, 199, 252, 133, 214, 156, \
30, 96, 139, 133, 71, 53, 203, 185, 7, 184, 68, 64, 12, 21, 124, 124, \
205, 2, 118, 242, 166, 149, 4, 253, 214, 29, 132, 229, 252, 229, 125, \
127, 248, 8, 152, 50, 210, 1, 99, 158, 233, 8, 168, 103, 35, 242, 87, \
151, 50, 194, 50, 51, 48, 5, 15, 225, 33, 85, 193, 166, 197, 250, \
239, 195, 65, 158, 137, 163, 75, 129, 75, 225, 224, 2, 6, 106, 21, \
221, 128, 233, 40, 206, 198, 126, 42, 89, 5, 190, 46, 85, 209, 149, \
149, 84, 3, 196, 8, 216, 255, 144, 186, 211, 131, 204, 17, 112, 168, \
230, 185, 119, 32, 247, 8, 136, 1, 130, 175, 143, 153, 160, 78, 222, \
84, 210, 160, 159, 186, 195, 176, 156, 191, 156, 175, 175, 255, 1, \
19, 6, 90, 64, 44, 115, 221, 33, 21, 12, 228, 126, 74, 242, 230, 88, \
70, 70, 102, 0, 161, 252, 36, 42, 184, 52, 216, 191, 93, 248, 104, \
51, 209, 52, 105, 112, 41, 124, 60, 0, 64, 205, 66, 187, 176, 29, 37, \
25, 216, 207, 197, 75, 32, 183, 197, 202, 186, 50, 178, 170, 128, \
120, 129, 27, 31, 242, 23, 90, 112, 121, 130, 46, 21, 28, 215, 46, \
228, 30, 225, 17, 0, 48, 85, 241, 243, 52, 9, 219, 202, 154, 84, 19, \
247, 88, 118, 19, 151, 243, 149, 245, 255, 224, 34, 96, 203, 72, 5, \
142, 123, 164, 34, 161, 156, 143, 201, 94, 92, 203, 8, 200, 204, 192, \
20, 63, 132, 133, 87, 6, 155, 23, 235, 191, 13, 6, 122, 38, 141, 46, \
5, 47, 135, 128, 8, 25, 168, 87, 118, 3, 164, 163, 59, 25, 248, 169, \
100, 22, 248, 185, 87, 70, 86, 85, 80, 15, 16, 35, 99, 254, 66, 235, \
78, 15, 48, 69, 194, 163, 154, 229, 220, 131, 220, 34, 32, 6, 10, \
190, 62, 102, 129, 59, 121, 83, 74, 130, 126, 235, 14, 194, 114, 254, \
114, 190, 191, 252, 4, 76, 25, 105, 0, 177, 207, 116, 132, 84, 51, \
145, 249, 43, 203, 153, 97, 25, 25, 152, 2, 135, 240, 144, 170, 224, \
211, 98, 253, 119, 225, 160, 207, 68, 209, 165, 192, 165, 240, 240, \
1, 3, 53, 10, 238, 192, 116, 148, 103, 99, 63, 21, 44, 130, 223, 23, \
42, 232, 202, 202, 170, 1, 226, 4, 108, 127, 200, 93, 105, 193, 230, \
8, 184, 84, 115, 92, 187, 144, 123, 132, 68, 0, 193, 87, 199, 204, \
208, 39, 111, 42, 105, 80, 79, 221, 97, 216, 78, 95, 206, 87, 215, \
255, 128, 137, 131, 45, 32, 22, 57, 238, 144, 138, 134, 114, 63, 37, \
121, 115, 44, 35, 35, 51, 0, 80, 254, 18, 21, 92, 26, 108, 95, 174, \
252, 52, 25, 232, 154, 52, 184, 20, 190, 30};

byte data2recive2[32];

size_t i2s_byte_write0 = 0;
size_t i2s_byte_write1 = 0;

void setup(){
    Serial.begin(115200);
    i2sconfig();
    
}
void loop(){
    // i2s_write(I2S_NUM_0,data2send,512,&i2s_byte_write,100);
    
    // Serial.print("Next!");
    // i2s_write(I2S_NUM_1,data2send2,32,&i2s_byte_write1,100);
    i2s_write(I2S_NUM_0,data2send2,32,&i2s_byte_write0,100);
    
    // i2s_read(I2S_NUM_1,data2recive2,32,&i2s_byte_write1,100);
    // for (int i=0;i<32;i++){
    //     Serial.print(data2recive2[i]);
    //     Serial.print(" ");
    // }
    // Serial.println(" ");
    // // Serial.println(i2s_byte_write1);
    // delay(500);
    

    // for (int i = 0;i<10;i++){
    //     i2s_set_sample_rates(I2S_NUM_0,(uint32_t)(3125));    // CLk_DIV TOO LARGE
    //     delay(2000);
    // }
    // i2s_set_sample_rates(I2S_NUM_0, 625000);
    
    // Serial.println(i2s_byte_write);
}   

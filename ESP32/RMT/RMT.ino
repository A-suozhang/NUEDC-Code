#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt.h"
#include <Arduino.h>

static const char* TAG = "RMT";

#define RMT_TX_GPIO 12

rmt_item32_t item0[] = {
    {{{4,0,4,1}}},
    {{{4,1,4,1}}}
    // {{{ 0, 1, 0, 0 }}}
};

rmt_item32_t item1[] = {
    {{{4,1,4,1}}}
};

// int num_items = sizeof(items)/sizeof(items[0]);   

void rmt_tx_init(){
    rmt_config_t config0;
    config0.rmt_mode = RMT_MODE_TX;
    config0.channel = RMT_CHANNEL_0;
    config0.gpio_num = (gpio_num_t)(RMT_TX_GPIO);
    config0.mem_block_num = 1;
    config0.tx_config.loop_en = 1;
    // The Carrier Settings
    config0.tx_config.carrier_en = 0;
    config0.tx_config.idle_output_en = 0;   // ?
    config0.tx_config.idle_level = (rmt_idle_level_t)(1);
    // config0.tx_config.carrier_duty_percent = 0;  
    // config0.tx_config.carrier_freq_hz = 1000;
    // config0.tx_config.carrier_level = 0;
    config0.clk_div = 32;

    rmt_config(&config0);
    rmt_driver_install(config0.channel,0,0);

}

void setup(){
    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
    // inMode(12,OUTPUT);
    rmt_tx_init();
    ESP_LOGI(TAG, "Initializing RMT...");
    // ESP_LOGI(TAG,"The Size Of Items Is %d", num_items);
}

void loop(){
    digitalWrite(25,HIGH);
    rmt_write_items(RMT_CHANNEL_0, item0, 3, true);
    delay(1000);
    // rmt_write_items(RMT_CHANNEL_0, item1, 1, true);
}

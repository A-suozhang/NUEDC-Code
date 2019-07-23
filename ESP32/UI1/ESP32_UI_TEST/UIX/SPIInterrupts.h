#ifndef __SPIINTERRPUTS_H__
#define __SPIINTERRPUTS_H__

#include "esp_attr.h"
#include "esp_log.h"

static volatile unsigned char spitransmitting;
static volatile bool spifinishedtransmitting=false;

void IRAM_ATTR spipretrans(void* arg){
    spitransmitting=true;
    //ESP_LOGI(TAG,"spipretrans");
}

void IRAM_ATTR spiposttrans(void* arg){
    spitransmitting=false;
    spifinishedtransmitting=true;
    //ESP_LOGI(TAG,"spiposttrans");
}

#endif 
#include <Arduino.h>
#include <HardwareSerial.h>
#include "src/UIX/UIX.h"
#include <String.h>
#include <SPI.h>
#include <driver/adc.h>
#include <driver/dac.h>
#include <esp_log.h>
#include "src/AD9910.h"
static const char* TAG = "My_ESP32";

# define CLOCKSPEED 1000000
# define uchar unsigned char

//SCLK = 14, MISO = 12, MOSI = 13, SS = 15
# define cs 15
# define cs2 25
# define miso 12                                     
# define mosi 13
# define sclk 14
# define update 26

SPIClass * hspi = NULL;
AD9910 * DDS = NULL;
UIX uix;

bool fc_shift_en;

char sendstr[100]="";
char recvstr[100]="";

void btnsendclick0(int tag,UIXButton* obj);
void btnsendclick1(int tag,UIXButton* obj);
void btnsendclick2(int tag,UIXButton* obj);

unsigned long last_time;

UIXInputBox inptsend0(20,120,20,50,COLVSBLUE,COLCYAN,sendstr);   //input box
UIXButton btnsend0(130,180,20,50,COLVSBLUE,COLCYAN,"Set",btnsendclick0);
UIXButton btnsend1(10,160,60,90,COLVSBLUE,COLCYAN,"Enable fc Shift",btnsendclick1);
UIXButton btnsend2(10,160,100,130,COLVSBLUE,COLVSORANGE,"Disable fc Shift",btnsendclick2);

int16_t adc_read(){
    signed long adc_sum = 0;
    int16_t adc_result = 0;
    int16_t adc_temp = 0;
    for (int i = 0; i<1600;i++){
        adc_temp = adc1_get_raw(ADC1_CHANNEL_6);
        adc_sum = adc_sum + adc_temp;
        // Serial.println(adc_sum);
    }
    adc_result = adc_sum/100;
    return adc_result;
}


void btnsendclick0(int tag,UIXButton* obj){
    // int val = adc_read();
    // Serial.println(val);
    int calib;
    // sscanf(sendstr,"%d",calib);
    calib = atoi(sendstr);
    
    (hspi)->beginTransaction(SPISettings(CLOCKSPEED, MSBFIRST, SPI_MODE0)); 
    digitalWrite(cs , LOW);

    Serial.print(" ");
    Serial.print((calib & 0xFF00)>>8,HEX);
    Serial.print(" ");
    Serial.println((calib & 0x00FF),HEX);
    (hspi)->transfer((calib & 0XFF00) >> 8);
    (hspi)->transfer((calib & 0X00FF));
    // Serial.println(temp_read);
    digitalWrite(cs , HIGH);
    (hspi)->endTransaction();
    // ESP_LOGI(TAG,"Button Pressed");

};

void btnsendclick1(int tag,UIXButton* obj){
    fc_shift_en = true;
}

void btnsendclick2(int tag,UIXButton* obj){
    fc_shift_en = false;

    (hspi)->beginTransaction(SPISettings(CLOCKSPEED, MSBFIRST, SPI_MODE0)); 
    digitalWrite(cs , LOW);
    // uchar temp = atoi(sendstr);
    (hspi)->transfer(0XFF);
    (hspi)->transfer(0XFF);
    // Serial.println(temp_read);
    digitalWrite(cs , HIGH);
    (hspi)->endTransaction();
}

void setup(){
    pinMode(15,OUTPUT);
    Serial.begin(115200);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_6);  // 6dB: Dynamic Range 2.2V

    hspi = new SPIClass(HSPI);
    if(hspi==NULL)
        Serial.print("SPI INITIALIZATION FAILED.");
    //SCLK = 14, MISO = 12, MOSI = 13, SS = 15
    hspi->begin();

    DDS = new AD9910(cs2,update,mosi,sclk,hspi);
    DDS->begin();

    uixuserpanelnum=1;  //no more than 46
    uixpanels[0].label="2019 G";
    uixpanels[0].uixobjects+=inptsend0;
    uixpanels[0].uixobjects+=btnsend0;
    uixpanels[0].uixobjects+=btnsend1;
    uixpanels[0].uixobjects+=btnsend2;
    uix.begin();

    bool fc_shift_en = true;

}

signed short last_val = 0;
signed short temp_val;


void loop(){
    uix.tick();

    if (fc_shift_en){
        unsigned long temp_millis = millis();
        if (temp_millis - last_time > 10){
            last_time = temp_millis;
            signed short val = adc_read();
            val = val - 0x8000;
            // Serial.print(val);
            // Serial.print(" ");

            if (val>last_val+25){
                temp_val = val;
                last_val = val;
            }
            else if(val<last_val-25){
                temp_val = val;
                last_val = val;
            }
            else{
                temp_val = last_val;
            }

            // Serial.println(temp_val);

            (hspi)->beginTransaction(SPISettings(CLOCKSPEED, MSBFIRST, SPI_MODE0)); 
            digitalWrite(cs , LOW);
            // uchar temp = atoi(sendstr);
            // Serial.print((val & 0xFF00)>>8,HEX);
            // Serial.print(" ");
            // Serial.println((val & 0x00FF),HEX);
            (hspi)->transfer((temp_val & 0XFF00) >> 8);
            (hspi)->transfer((temp_val & 0X00FF));
            // Serial.println(temp_read);
            digitalWrite(cs , HIGH);
            (hspi)->endTransaction();

        }

    }
    // else{
    //     Serial.println("Disabled...");
    // }

    // (hspi)->beginTransaction(SPISettings(CLOCKSPEED, MSBFIRST, SPI_MODE0)); 
    // digitalWrite(cs , LOW);
    // (hspi)->transfer(0xAA);
    // digitalWrite(cs , HIGH);
    // (hspi)->endTransaction();
    // delay(100);
}

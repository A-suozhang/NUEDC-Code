#include <Arduino.h>
#include <HardwareSerial.h>
# include "AD9910.h"
# include <SPI.h>

# define cs 5
# define rst 16
# define update 17
# define sdio 23
# define sclk 18

# define CLOCKSPEED 1000000
# define uchar unsigned char

uchar temp;

unsigned long temp_millis0;
unsigned long temp_millis1;
unsigned long last_time0;
unsigned long last_time1;

uchar data2send[5] = {0x35,0x34,0x33,0x32,0x31};
uchar dataReceived[5];
double amp = 100.0;

HardwareSerial Serial0(2);
AD9910 DDS(cs, rst,update,sdio,sclk);

void setup(){
    Serial.begin(115200);
    Serial0.begin(115200,SERIAL_8N1,14,12);
    SPI.begin();
    DDS.begin();
    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
}



void loop(){

    // Every One Sec Write Through Serial0 Acquiing The Data Bits
    
    // ------------ Receiving Data ---------------
    temp_millis0 = millis();
    if (temp_millis0 - last_time0 > 1000){
        last_time0 = temp_millis0;
        Serial0.write(0x30);
    }

    if (Serial0.available()){
        Serial0.readBytes(dataReceived, 5);
        // Serial.write(dataReceived, 5);
        Serial.print(dataReceived[0],HEX);
        Serial.print(" ");
        Serial.print(dataReceived[1],HEX);
        Serial.print(" ");
        Serial.print(dataReceived[2],HEX);
        Serial.print(" ");
        Serial.print(dataReceived[3],HEX);
        Serial.print(" ");
        Serial.print(dataReceived[4],HEX);
        Serial.println(" ");
    }

    // --------- Decode The Data -----------
    // ---- Original Data ----
    // DataPack[0] <= {3'b0, data[24:20]};
    // DataPack[1] <= data[19:12];
    // DataPack[2] <= data[11:4];
    // DataPack[3] <= {data[3:0],addr[11:8]};
    // DataPack[4] <= addr[7:0];
    // // ---- Fake Data -----
    // dataReceived[0] = 0x1F;
    // dataReceived[1] = 0x33;
    // dataReceived[2] = 0x27;
    // dataReceived[3] = 0x6F;
    // dataReceived[4] = 0xFA;

    unsigned long Amp;
    signed short freq;
    double frequency;

    Amp = ((dataReceived[0] & 0x1F) << 20)|((dataReceived[1]) << 12)|((dataReceived[2]) << 4)|((dataReceived[3] & 0xF0) >> 4);

    freq = ((dataReceived[3] & 0x0F) << 12)|(dataReceived[4] << 4); // Fiest Move 12 bit Data Left 4, to decode negative value
    freq = freq >> 5;   // then right shift 4 to restore value
    if (freq <= 0) {
        freq = -1*freq;
    }
    frequency = (float(freq)/2048.0)*781250;


    temp_millis1 = millis();
    if (temp_millis1 - last_time1 > 1000){
        last_time1 = temp_millis1;
        Serial.print("Amp: ");
        Serial.print(double(Amp/6710.0));
        Serial.println(" ");
        Serial.print("Freq: ");
        Serial.println(frequency);
        Serial.println(" ");
    }


    // while(Serial.read()>0){} 
    // while(Serial0.read()>0){}   // Empty Input BUffer


    // ------------ Dirve AD9910 -----------------

    // for (int i = 0;i<10;i++){
    //     DDS.set_Amp(amp*i);
    //     delay(50);
    // }
    // for (int j = 10; j>0;j--){
    //     DDS.set_Amp(amp*j);
    //     delay(50);
    // }

    // DDS.set_freq(100000000.0);
   


    // ------- Lit The Light --------
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);

}
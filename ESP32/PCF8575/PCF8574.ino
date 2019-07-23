#include <Arduino.h>
#include "src/PCF8574.h"
#include "Wire.h"

PCF8574 IO_Expand0(0x00);   // The ADDR Here Is Of 3 BIT (0x00 ~ 0x07)
PCF8574 IO_Expand1(0x01);

void setup(){
    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
    Serial.begin(115200);

    Wire.begin();
    IO_Expand0.begin();
    IO_Expand1.begin();
    
}

void loop(){
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);

    uchar temp;
    uchar data_rx;
    if (Serial.available()){
        temp = Serial.read();
        if (temp == 0x30){  // The ASC For '0'
            IO_Expand0.WriteByte(0xAA);
        }
        if (temp == 0x31){
            data_rx = IO_Expand0.ReadByte();
            Serial.print(data_rx,HEX);
        }
        if (temp == 0x40){  // The ASC For '0'
            IO_Expand1.WriteByte(0xBB);
        }
        if (temp == 0x41){
            data_rx = IO_Expand1.ReadByte();
            Serial.print(data_rx,HEX);
        }
        if (temp == 0x32){
            if (Serial.available()){
                uchar shift0;
                shift0 = Serial.read();
                IO_Expand0.digitalWrite(shift0,LOW);
            }
        }
        if (temp == 0x33){
            if (Serial.available()){
                uchar shift1;
                shift1 = Serial.read();
                IO_Expand0.digitalWrite(shift1,HIGH);
            }
        }
        if (temp == 0x34){
            if (Serial.available()){
                uchar i;
                i = Serial.read();
                IO_Expand0.digitalRead(i);
            }
        }
        if (temp == 0x35){
            uchar temp_data;
            temp_data = IO_Expand0.get_temp_data();
            Serial.println(temp_data,HEX);
        }
    }
    // Serial.println("Working..");
}
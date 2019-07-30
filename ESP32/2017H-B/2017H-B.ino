# include "HardwareSerial.h"
HardwareSerial Serial0(2);


void setup(){
    Serial.begin(115200);
    Serial0.begin(115200,SERIAL_8N1,14,12);
}

void loop(){
    while (Serial0.available()){
        char c = Serial0.read();
        Serial.printf("Got: %d\n",c);
    }
}
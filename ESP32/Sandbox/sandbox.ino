# include<Arduino.h>
# define uchar unsigned char

uchar data_buffer[100];

void setup(){
    pinMode(25, OUTPUT);
    Serial.begin(115200);
}

void loop(){
    int i = 1000;

    
    // Serial.write(((unsigned short)10000 & 0xFF00) >> 8);
    // Serial.write((unsigned short)10000 & 0x00FF);

    // Serial.print(256*sin(2*PI*0.1));
    // Serial.print(" ");
    // Serial.print((uchar)(int)(256*sin(2*PI*0.1)));
    // Serial.print(" ");
    // Serial.println(" ");

    Serial.println(float(100));

    digitalWrite(25,HIGH);
    // for (int j=0;j<100;j++){
    //     data_buffer[j] = (unsigned short)j;
    //     Serial.print(data_buffer[j],HEX);
    //     Serial.print(" ");
    // }
    delay(500);
}
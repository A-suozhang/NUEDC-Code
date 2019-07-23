# include<Arduino.h>
# define uchar unsigned char
#include <SPI.h>
SPIClass * hspi = NULL;
# define CLOCKSPEED 1000000
# define cs 15

uchar data_buffer[100];

void setup(){
    pinMode(25, OUTPUT);
    hspi = new SPIClass(HSPI);
    hspi->begin();
    Serial.begin(115200);
}

void loop(){
    int i = 1000;

    (hspi)->beginTransaction(SPISettings(CLOCKSPEED, MSBFIRST, SPI_MODE0)); // 开始SPI传输,其中CLOCKSPEED是SCLK传输,选用MODE0(经测试除了MODE1之外其他MODE都能操作)
    digitalWrite(cs , LOW);    // 手动拉低CS
    // delay(1);
    for (int i = 0; i < 2; i++){
        (hspi)->transfer(0xaa);
        (hspi)->transfer(0xbb);
        (hspi)->transfer(0xcc);
    }
    digitalWrite(cs , HIGH);
    (hspi)->endTransaction();

    
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
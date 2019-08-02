# include<Arduino.h>
# define uchar unsigned char
// #include <SPI.h>
// SPIClass * hspi = NULL;
// # define CLOCKSPEED 1000000
// # define cs 15

uchar data_buffer[100];

void setup(){
    pinMode(25, OUTPUT);
    // hspi = new SPIClass(HSPI);
    // hspi->begin();
    Serial.begin(115200);
}

void loop(){

    int i = 129;
    char s[9];
    
    itoa(i,s,2);
    for (int i = 0;i<9;i++){
        if (s[i] == NULL){
            // Serial.printf("There Is NULL at %d",i);
            s[i] = 0;
        }
        if (s[i] == 49){    // ASCII for '1'
            s[i] = 1;
            // Serial.print('1');
        }
        if (s[i] == 48){
            s[i] = 0;
        }
        // else{
        //     Serial.printf("The %dth is %d",i,s[i]);
        // }
    }

    for (int i = 0;i<8;i++){
        if (s[i] == 1){
            Serial.print('1');
            Serial.print(' ');
        }
        else if (s[i] == 0){
            Serial.print('0');
            Serial.print(' ');
        }
        else {
            Serial.print('2');
            Serial.print(' ');
        }
    }

    Serial.println(" ");
    
    

    // (hspi)->beginTransaction(SPISettings(CLOCKSPEED, MSBFIRST, SPI_MODE0)); // 开始SPI传输,其中CLOCKSPEED是SCLK传输,选用MODE0(经测试除了MODE1之外其他MODE都能操作)
    // digitalWrite(cs , LOW);    // 手动拉低CS
    // // delay(1);
    // for (int i = 0; i < 2; i++){
    //     (hspi)->transfer(0xaa);
    //     (hspi)->transfer(0xbb);
    //     (hspi)->transfer(0xcc);
    // }
    // digitalWrite(cs , HIGH);
    // (hspi)->endTransaction();

    
    // Serial.write(((unsigned short)10000 & 0xFF00) >> 8);
    // Serial.write((unsigned short)10000 & 0x00FF);

    // Serial.print(256*sin(2*PI*0.1));
    // Serial.print(" ");
    // Serial.print((uchar)(int)(256*sin(2*PI*0.1)));
    // Serial.print(" ");
    // Serial.println(" ");

    digitalWrite(25,HIGH);
    // for (int j=0;j<100;j++){
    //     data_buffer[j] = (unsigned short)j;
    //     Serial.print(data_buffer[j],HEX);
    //     Serial.print(" ");
    // }
    delay(500);
}
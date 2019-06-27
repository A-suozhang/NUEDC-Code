#include <Arduino.h>
#line 1 "c:\\Users\\A_suozhang\\Documents\\Arduino\\ESP32\\ESP32_Test\\esp32_test.ino"
#line 1 "c:\\Users\\A_suozhang\\Documents\\Arduino\\ESP32\\ESP32_Test\\esp32_test.ino"
int i = 0;

#line 3 "c:\\Users\\A_suozhang\\Documents\\Arduino\\ESP32\\ESP32_Test\\esp32_test.ino"
void setup();
#line 8 "c:\\Users\\A_suozhang\\Documents\\Arduino\\ESP32\\ESP32_Test\\esp32_test.ino"
void loop();
#line 3 "c:\\Users\\A_suozhang\\Documents\\Arduino\\ESP32\\ESP32_Test\\esp32_test.ino"
void setup(){
    pinMode(25,OUTPUT);
    Serial.begin(115200);
}

void loop(){
    Serial.println(i);
    i++;
    digitalWrite(25,HIGH);
    delay(200);
    digitalWrite(25,LOW);
    delay(200);

}

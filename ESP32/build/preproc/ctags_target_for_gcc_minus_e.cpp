# 1 "c:\\Users\\A_suozhang\\Documents\\Arduino\\ESP32\\ESP32_Test\\esp32_test.ino"
# 1 "c:\\Users\\A_suozhang\\Documents\\Arduino\\ESP32\\ESP32_Test\\esp32_test.ino"
int i = 0;

void setup(){
    pinMode(25,0x02);
    Serial.begin(115200);
}

void loop(){
    Serial.println(i);
    i++;
    digitalWrite(25,0x1);
    delay(200);
    digitalWrite(25,0x0);
    delay(200);

}

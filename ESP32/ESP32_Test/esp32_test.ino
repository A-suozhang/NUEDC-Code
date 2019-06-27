int i = 0;

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
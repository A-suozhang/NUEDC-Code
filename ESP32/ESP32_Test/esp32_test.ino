#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(2);

char input;
float input_float;
double input_double = 1.23;

void setup() {
  

  // The first is RX!!!
  mySerial.begin(115200,SERIAL_8N1,14,12);

  pinMode(25,OUTPUT);

	Serial.begin(115200); 
	delay(2000);  

	mySerial.println("Type something!");
}

void loop() {

  digitalWrite(25,HIGH); // Lit A Light 
  delay(200);

  /*--------- Serial Input & Print Test ------------- */

	if(Serial.available()){

		input_float = Serial.parseFloat();
		// Serial.print("You typed: ");
    if (input_float == 1.0){
		  Serial.println(input_double);  
      Serial.println((double)input_double);  
      mySerial.print(63,HEX);
      Serial.print(64,HEX);

    }

    if (input_float == 2.0){
      Serial.println(255,HEX);
      Serial.println(255,BIN);
    }

    if (Serial.findUntil("aaa","bc")){
      Serial.println("Received aaa");
    }

    while(Serial.read()>0){}  //Clear Serial Buffer
    
	}
  // Serial.println((float)3.2,1);
 
}
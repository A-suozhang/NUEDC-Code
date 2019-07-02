char input;
float input_float;
double input_double = 1.23;

void setup() {

  pinMode(25,OUTPUT);

	Serial.begin(115200); 
	delay(2000);  

	Serial.println("Type something!");
}

void loop() {

  digitalWrite(25,HIGH); // Lit A Light 
  /*--------- Serial Input & Print Test ------------- */

	if(Serial.available()){

		input_float = Serial.parseFloat();
		// Serial.print("You typed: ");
    if (input_float == 1.0){
		  Serial.println(input_double);  
      Serial.println((double)input_double);  
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
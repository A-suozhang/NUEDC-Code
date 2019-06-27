// the number of the LED pin
const int ledPin = 2;  

// setting PWM properties
const int freq = 500000;
const int ledChannel = 0;
const int resolution = 8;
 
void setup(){
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}
 
void loop(){

  
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
  }
//
//  // decrease the LED brightness
//  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
//    // changing the LED brightness with PWM
//    ledcWrite(ledChannel, dutyCycle);  
//  }
 
}

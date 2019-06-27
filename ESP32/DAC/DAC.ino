volatile int DACFlag;
int totalInterrupts;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
int t;

void IRAM_ATTR DACTimer(){

    portENTER_CRITICAL_ISR(&timerMux);
    DACFlag ++;
    portEXIT_CRITICAL_ISR(&timerMux);
    
  }

void setup() {

  pinMode(25,OUTPUT);

  Serial.begin(115200);
  timer = timerBegin(0, 10, true);
  timerAttachInterrupt(timer, &DACTimer, true);
  timerAlarmWrite(timer,5 0 ,true);    //80_000_000/8000/10
  timerAlarmEnable(timer);
  
}

void loop() { // Generate a Sine wave

  digitalWrite(25,HIGH);
  
  if (DACFlag>0){
      portENTER_CRITICAL_ISR(&timerMux);
      DACFlag --;
      portEXIT_CRITICAL_ISR(&timerMux);

      totalInterrupts ++;

      if (totalInterrupts % 2 == 1){
          dacWrite(25,0);
        }
      else{
          dacWrite(25,255);
        }
      //dacWrite(25, 128 + 127*(sin(2000*PI/8000*t))); 
    }

  
    
  
}

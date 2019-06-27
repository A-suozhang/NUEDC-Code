volatile int interruptCounter;    // The Interrupt_Count, More Like A Flag, being ++ and -- , range from 1 and 0
int totalInterruptCounter;        // The Total Num Of Interrupts  
 
hw_timer_t * timer = NULL;        // Create a Pointer Towards HardWare Timer(Which Timer Will Be Specified In timerBegin)
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // The TimerMux is like a Lock, to prevent write to variable in different tasks
                                                      // When ENTER_CRITICAL_ISR: Locked
                                                      // When The Lock Is Locked, Other Task Call portENTER_CRITICAL_ISR, blocking
 
void IRAM_ATTR onTimer() {
  
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;                 // Flag is 1
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(115200);
 
  timer = timerBegin(0, 80, true);                         // timeBegin(Hardware_timer_0-3,prescaler_freq_divide_rate ,true)
  timerAttachInterrupt(timer, &onTimer, true);             // AttachInterrupt(timer_obj, The_ISR_function, true)
  timerAlarmWrite(timer, 1000000, true);                   // AlarmWrite(timer_obj, factor2, true);    when 1/80_000_000*(prescaler_factor*factor2) s, call ISR
  timerAlarmEnable(timer);
 
}
 
void loop() {
 
  if (interruptCounter > 0) {         // Check interrupt_flag
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;               //flag back to 0
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;          // Num_Of_Interruption ++
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
 
  }
}

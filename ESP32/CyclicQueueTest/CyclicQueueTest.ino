# include "CyclicQueue.h"

volatile int interruptCounter;  
int totalInterruptCounter;  

hw_timer_t * timer = NULL; 
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; 

// Build The BUFFER
int buffer[100];  //
int element;      //Contain Elements Poped From BUFFER
CyclicQueue <int> cq0(buffer, 100);   //Instantiation The QUEUE(FIFO)

//Interrupt 0
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;                 // Flag is 1
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);

  //Set Up Interrupt
  timer = timerBegin(0, 80, true);                         // timeBegin(Hardware_timer_0-3,prescaler_freq_divide_rate ,true)
  timerAttachInterrupt(timer, &onTimer, true);             // AttachInterrupt(timer_obj, The_ISR_function, true)
  timerAlarmWrite(timer, 100000, true);                   // AlarmWrite(timer_obj, factor2, true);    when 1/80_000_000*(prescaler_factor*factor2) s, call ISR
  timerAlarmEnable(timer);

}

void loop() {

  // Call The Interrupt 
   if (interruptCounter > 0) {         // Check interrupt_flag
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;               //flag back to 0
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;          // Num_Of_Interruption ++
    cq0.push(totalInterruptCounter);
 
    Serial.print(totalInterruptCounter);
    Serial.print("  ");
    Serial.println(cq0.len);

    if (cq0.len > 80){
        cq0.clear();
      }
 
  }
  
   // cq0.pop(&element);
   

}

#include <driver/adc.h>
#include <Wire.h>        // Wire header file for I2C and 2 wire

// volatile unsigned long interruptCounter0;   // Timer0 - Interrupt0 For ADC

//  Set Up Time Measure
unsigned int sps = 0;
unsigned long temp_micros;
unsigned long temp_millis;
unsigned long lasttime;
unsigned long lasttime_for_adc;
int i = 0;
int j = 0;
int serial_start = 0;
unsigned char data2send[8000];

// Set Up Timer
//hw_timer_t * timer0 = NULL;
//portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;

//// Interrupt 0
//void IRAM_ATTR onTimer0() {
//  portENTER_CRITICAL_ISR(&timerMux0);
//  interruptCounter0++;
//  portEXIT_CRITICAL_ISR(&timerMux0);
//}



void setup() {
  // put your setup code here, to run once:
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);

  Serial.begin(115200);

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_6);

  //  timer0 = timerBegin(0, 100, true);
  //  timerAttachInterrupt(timer0, &onTimer0, true);
  //  timerAlarmWrite(timer0, 100, true);          // 100 For 8K Sample Rate
  //  timerAlarmEnable(timer0);
  //  lasttime=millis();

}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned char val;

  //Lit Light
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);


  temp_micros = micros();
  if (temp_micros - lasttime_for_adc > 83) {
    lasttime_for_adc = temp_micros;

    if (serial_start == 0) {
      //    portENTER_CRITICAL_ISR(&timerMux0);
      //    interruptCounter0--;
      //    portEXIT_CRITICAL_ISR(&timerMux0);
      val = (adc1_get_raw(ADC1_CHANNEL_6) >> 4);
      // val=BinaryToGray((char)(val>>4));
      // Serial2.write((char)((fake_data++)%255));
      // Serial.println(fake_data,HEX);
      data2send[i] = val;
      i++;
      i = i % 8000;
      sps++;
    }
  }

  if (serial_start == 1){
    for (j = 0; j < 7999; j++) {
      Serial.print (data2send[j]);
      Serial.print (" ");
      if (j % 24 == 0) {
        Serial.println(" ");
      }
    }
    serial_start = 0;
    temp_millis = millis();
    lasttime = temp_millis;
    Serial.println(" ");
    Serial.println(sps);
    Serial.println(" ");
    sps=0;
  }
  
  temp_millis = millis();
  if (temp_millis - lasttime > 1000) {
    lasttime = temp_millis;
    serial_start = 1;
  }

}

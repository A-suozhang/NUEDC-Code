#include <driver/adc.h>
#include <Wire.h>        // Wire header file for I2C and 2 wire
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include "esp_task_wdt.h"
#include "CS5343ESP32.h"
#include "AGCAmplifier.h"

#define PEEKBUFLEN 3906

CS5343 adc(27, 26, 25, 4);
AGCAmplifier agc(0.001, 10);
long res[PEEKBUFLEN];



// volatile unsigned long interruptCounter0;   // Timer0 - Interrupt0 For ADC

//  Set Up Time Measure
unsigned int sps = 0;
unsigned long temp_micros;
unsigned long temp_millis;
unsigned long temp_millis2;
unsigned long lasttime;
unsigned long lasttime2;
unsigned long lasttime_for_adc;
int i = 0;
int j = 0;
int serial_start = 0;
int read_temperature_start = 0;
unsigned char data2send[8000];
unsigned char temp2send;

// Set Up Temperature Sensor
int TMP75_Address = 0x48; // Apple iMac Temp Sensor Circular PCB
int numOfBytes = 2;
int baudRate = 9600;
int columns = 16;   // LCD is 16 Column 2 Row Hitachi LCD
int rows = 2;
byte configReg = 0x01;  // Address of Pointer Register
byte bitConv = B01100000;   // Set to 1aaaaaaaaaaaaaaaaaaaaaaaaaaaaqr52 bit conversion
byte rdWr = 0x01;       // Set to read write
byte rdOnly = 0x00;    // Set to Read
String temperature;

TaskHandle_t Task1;   // Creating Task
//TaskHandle_t Task2;

float readTemp() {
  // Now take a Temerature Reading
  Wire.requestFrom(TMP75_Address, numOfBytes); // Address the TMP75 and set number of bytes to receive
  byte MostSigByte = Wire.read();              // Read the first byte this is the MSB
  byte LeastSigByte = Wire.read();             // Now Read the second byte this is the LSB

  // Being a 12 bit integer use 2's compliment for negative temperature values
  int TempSum = (((MostSigByte << 8) | LeastSigByte) >> 4);
  // From Datasheet the TMP75 has a quantisation value of 0.0625 degreesC per bit
  float temp = (TempSum * 0.0625);
  // Serial.println(MostSigByte, BIN);   // Uncomment for debug of binary data from Sensor
  // Serial.println(LeastSigByte, BIN);  // Uncomment for debug  of Binary data from Sensor
  return temp;                           // Return the temperature value
}



void setup() {
  // put your setup code here, to run once:
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);

  Serial.begin(230400);                       // Serial Used For Print Log
  Serial2.begin(230400, SERIAL_8N1, 16, 17);  // HW_SERIAL

  Wire.begin();                                // Join the I2C bus as a master
  Wire.beginTransmission(TMP75_Address);       // Address the TMP75 sensor
  Wire.write(configReg);                       // Address the Configuration register
  Wire.write(bitConv);                         // Set the temperature resolution
  Wire.endTransmission();                      // Stop transmitting
  Wire.beginTransmission(TMP75_Address);       // Address the TMP75 sensor
  Wire.write(rdOnly);                          // Address the Temperature register
  Wire.endTransmission();                      // Stop transmitting

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_6);

  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */

  delay(500);

  //  xTaskCreatePinnedToCore(
  //    Task2code,   /* Task function. */
  //    "Task2",     /* name of task. */
  //    10000,       /* Stack size of task */
  //    NULL,        /* parameter of the task */
  //    1,           /* priority of the task */
  //    &Task2,      /* Task handle to keep track of created task */
  //    1);          /* pin task to core 1 */
  //}
}

void Task1code( void * pvParameters ) {

  unsigned char val;
  int adc_per_sec = 0;

  adc.begin(15625);

  for (;;) {
    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;

    unsigned char val;
    unsigned char fake_val;

    //Lit Light
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);


    // ------------------ The Old Built In ADC Way -------------------------
    //    temp_micros = micros();
    //    if (temp_micros - lasttime_for_adc > 83) {
    //      lasttime_for_adc = temp_micros;
    //
    //      // if (serial_start == 0) {
    //      val = (adc1_get_raw(ADC1_CHANNEL_6) >> 4);
    //
    //      // Fake Data
    //      // fake_val++;
    //      fake_val = fake_val % 255;
    //
    //      // val=BinaryToGray((char)(val>>4));
    //      // Serial2.write((char)((fake_data++)%255));
    //      // Serial.println(fake_data,HEX);
    //      // data2send[i] = val;
    //      if (val == 255) {
    //        val = val - 1;
    //      }
    //      Serial2.write(val);
    //      // Serial2.write(fake_val);
    //      //  Serial.print(val);
    //      // i++;
    //      // i = i % 8000;
    //      // sps++;
    //    }
    //    // }
    //
    //    //    if (serial_start == 1) {
    //    //      for (j = 0; j < 7999; j++) {
    //    //        // Serial.print (data2send[j]);
    //    //        // Serial.print (" ");
    //    //        if (j % 24 == 0) {
    //    //          // Serial.println(" ");
    //    //        }
    //    //      }
    //    //      serial_start = 0;
    //    //      temp_millis = millis();
    //    //      lasttime = temp_millis;
    //    //      Serial.println(" ");
    //    //      Serial.println(sps);
    //    //      Serial.println(temp2send);
    //    //      Serial.println(" ");
    //    //      sps = 0;
    //    //    }
    //
    //    temp_millis = millis();
    //    if (temp_millis - lasttime > 1000) {
    //      lasttime = temp_millis;
    //      Serial2.write((char)255);
    //      Serial2.write((char)255);
    //      Serial2.write(temp2send);
    //      // Serial.println(temp2send);
    //      // serial_start = 1;
    //    }
    //
    //  }
    //  esp_task_wdt_feed();

    // ------------- The New ADC --------------
    unsigned long timestart = millis();
    long adcres = adc.read(0);
    val = agc.feed(adcres);
    adc_per_sec++;
    if (val == 255) {
      val = val - 1;
    }
    Serial2.write(val);

    temp_millis = millis();
    if (temp_millis - lasttime > 1000) {
      lasttime = temp_millis;
      Serial2.write((char)255); 
      Serial2.write((char)255);
      Serial2.write(temp2send);
      // Serial.println(adc_per_sec);
      adc_per_sec = 0;
      // Serial.println(temp2send);
      // serial_start = 1;
    }

    // --------The  New ADC Debug -----------

    //    for (int i = 0; i < 15625; i++) {
    //      long adcres = adc.read(0);
    //      res[i % PEEKBUFLEN] = agc.feed(adcres);
    //    }
    //    unsigned long timeend = millis();
    //    for (int i = 0; i < PEEKBUFLEN; i++) {
    //      Serial.print(res[i]);
    //      Serial.print(",");
    //    }
    //    Serial.println();
    //    Serial.println(timeend - timestart);
    //    Serial.println(agc.ampnowfilt);
    //    Serial.println();
    //    delay(1000);
  }
}

//void Task2code( void * pvParameters ) {
//  for (;;) {
//
//  }
//}

void loop() {
  TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed = 1;
  TIMERG0.wdt_wprotect = 0;

  // put your main code here, to run repeatedly:
  temp_millis2 = millis();
  if (temp_millis2 - lasttime2 > 1000) {
    lasttime2 = temp_millis2;32
    read_temperature_start = 1;
  }

  if (read_temperature_start == 1) {
    float temp = readTemp();             // Read the temperature now
    //unsigned char temp2send = BinaryToGray((char)(int)temp);
    temp2send = ((char)(int)temp);
    // Serial.print("Temperature   ");
    // Serial.println(temp2send,HEX);        // Temperature value to 1 Decimal place no newline to serial
    read_temperature_start = 0;
  }
  esp_task_wdt_feed();

}

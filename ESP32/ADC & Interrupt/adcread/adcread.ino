#include <driver/adc.h>
#include <Wire.h>        // Wire header file for I2C and 2 wire

volatile unsigned long interruptCounter0;   // Timer0 - Interrupt0 For ADC 
volatile unsigned long interruptCounter1;   // Timer1 - Interrupt1 For Temperature

//  Set Up Time Measure
unsigned int sps=0;
unsigned long lasttime;

unsigned char fake_data = 0;

// Set Up Timer 
hw_timer_t * timer0 = NULL;
hw_timer_t * timer1 = NULL;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;

// Set Up Temperature Sensor
int TMP75_Address = 0x48; // Apple iMac Temp Sensor Circular PCB
int numOfBytes = 2;
int baudRate = 9600;
int columns = 16;   // LCD is 16 Column 2 Row Hitachi LCD
int rows = 2;
byte configReg = 0x01;  // Address of Pointer Register
byte bitConv = B01100000;   // Set to 12 bit conversion
byte rdWr = 0x01;       // Set to read write
byte rdOnly = 0x00;    // Set to Read
String temperature;


// Binary <-> Gray Code
unsigned short BinaryToGray(unsigned char num)
{
    return num ^ (num >> 1);
}

unsigned short GrayToBinary(unsigned char num)
{
    unsigned int mask = num >> 1;
    while (mask != 0)
    {
        num = num ^ mask;
        mask = mask >> 1;
    }
    return num;
}

// Interrupt 0
void IRAM_ATTR onTimer0() {
  portENTER_CRITICAL_ISR(&timerMux0);
  interruptCounter0++;
  portEXIT_CRITICAL_ISR(&timerMux0);
}

// Interruption 1
void IRAM_ATTR onTimer1() {
  portENTER_CRITICAL_ISR(&timerMux1);
  interruptCounter1++;
  portEXIT_CRITICAL_ISR(&timerMux1);
}

float readTemp(){
  // Now take a Temerature Reading
  Wire.requestFrom(TMP75_Address,numOfBytes);  // Address the TMP75 and set number of bytes to receive
  byte MostSigByte = Wire.read();              // Read the first byte this is the MSB
  byte LeastSigByte = Wire.read();             // Now Read the second byte this is the LSB

  // Being a 12 bit integer use 2's compliment for negative temperature values
  int TempSum = (((MostSigByte << 8) | LeastSigByte) >> 4); 
  // From Datasheet the TMP75 has a quantisation value of 0.0625 degreesC per bit
  float temp = (TempSum*0.0625);
  // Serial.println(MostSigByte, BIN);   // Uncomment for debug of binary data from Sensor
  // Serial.println(LeastSigByte, BIN);  // Uncomment for debug  of Binary data from Sensor
  return temp;                           // Return the temperature value
}


void setup() {
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);

   
  Serial.begin(115200);                       // Serial Used For Print Log
  Serial2.begin(115200,SERIAL_8N1,16,17);     // HW_SERIAL
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);
  
  timer0 = timerBegin(0, 100, true);
  timer1 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer0, &onTimer0, true);
  timerAttachInterrupt(timer1, &onTimer1, true);
  timerAlarmWrite(timer0, 67, true);          // 100 For 8K Sample Rate
  timerAlarmWrite(timer1, 1000000, true);
  timerAlarmEnable(timer0);
  timerAlarmEnable(timer1);
  lasttime=millis();

  Wire.begin();                                // Join the I2C bus as a master
  Wire.beginTransmission(TMP75_Address);       // Address the TMP75 sensor
  Wire.write(configReg);                       // Address the Configuration register 
  Wire.write(bitConv);                         // Set the temperature resolution 
  Wire.endTransmission();                      // Stop transmitting
  Wire.beginTransmission(TMP75_Address);       // Address the TMP75 sensor
  Wire.write(rdOnly);                          // Address the Temperature register 
  Wire.endTransmission();                      // Stop transmitting 
}

void loop() {
  
  unsigned char val;
 
  //Lit Light
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);

  // Read ADC
  if(interruptCounter0>0){
    portENTER_CRITICAL_ISR(&timerMux0);
    interruptCounter0--;
    portEXIT_CRITICAL_ISR(&timerMux0);
    val=adc1_get_raw(ADC1_CHANNEL_6);
    val=BinaryToGray((char)(val>>4));
    Serial2.write((char)val);
    // Serial2.write((char)((fake_data++)%255));
    // Serial.println(fake_data,HEX);
    sps++;
  }

// Read The temperature and Serial Print
  if(interruptCounter1>0){
    portENTER_CRITICAL_ISR(&timerMux1);
    interruptCounter1--;
    portEXIT_CRITICAL_ISR(&timerMux1);
    float temp = readTemp();             // Read the temperature now
    //unsigned char temp2send = BinaryToGray((char)(int)temp);
    unsigned char temp2send = ((char)(int)temp);
    // Serial.print("Temperature   "); 
    // Serial.println(temp2send,HEX);        // Temperature value to 1 Decimal place no newline to serial
    
    Serial2.write((char)255);
    Serial2.write((char)255);
    Serial2.write(temp2send);
    // Serial.println(temp2send);
  }
  
  
  if(millis()-lasttime>1000){
    lasttime=millis();
    Serial.println(sps);
    sps=0;
  }
  
}

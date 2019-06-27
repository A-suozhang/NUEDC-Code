#include <Wire.h>        // Wire header file for I2C and 2 wire

// I used a Sensor PCB from a scrapped White Intel based iMac
// Note there are two sensor boards both with a TMP75 sensor in the iMac.

// TMP75 Address is 0x48 and from its Datasheet = A0,A1,A2 are all grounded.
int TMP75_Address = 0x48; // Apple iMac Temp Sensor Circular PCB
//int TMP75_Address = 0x49; // Apple iMac Temp Sensor Square PCB

// Declare some nice variables
int numOfBytes = 2;
int baudRate = 9600;
int columns = 16;   // LCD is 16 Column 2 Row Hitachi LCD
int rows = 2;
byte configReg = 0x01;  // Address of Pointer Register
byte bitConv = B01100000;   // Set to 12 bit conversion
byte rdWr = 0x01;       // Set to read write
byte rdOnly = 0x00;    // Set to Read
String temperature;

void setup(){
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  Serial.begin(baudRate);            // Set Serial Port speed
  Wire.begin();                      // Join the I2C bus as a master
  Wire.beginTransmission(TMP75_Address);       // Address the TMP75 sensor
  Wire.write(configReg);                       // Address the Configuration register 
  Wire.write(bitConv);                         // Set the temperature resolution 
  Wire.endTransmission();                      // Stop transmitting
  Wire.beginTransmission(TMP75_Address);       // Address the TMP75 sensor
  Wire.write(rdOnly);                          // Address the Temperature register 
  Wire.endTransmission();                      // Stop transmitting 
}

// Main Program Infinite loop 
void loop(){
  digitalWrite(25,HIGH);
  digitalWrite(26,HIGH);
  float temp = readTemp();             // Read the temperature now
  Serial.print("Temperature   "); 
  Serial.println(temp);        // Temperature value to 1 Decimal place no newline to serial
//  delay(50);
//  digitalWrite(25,LOW);
//  digitalWrite(26,LOW);
//  delay(50);
  
}

// Begin the reading the TMP75 Sensor 
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

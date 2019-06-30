#include <SPI.h>
#include "AD9914.h"
#include <ADF4350.h>

#define MAX_PARAM_NUM 11

#include "SetListArduino.h"

//Define pin mappings:
#define SYNCIO_PIN 8    

#define CSPIN 14                 // DDS chip select pin. Digital input (active low). Bringing this pin low enables detection of serial clock edges.
#define OSKPIN 15 //not actually connected               // DDS Output Shift Keying. Digital input.
#define PS0PIN 5               // DDS PROFILE[0] pin. Profile Select Pins. Digital input. Use these pins to select one of eight profiles for the DDS.
#define PS1PIN 6               // DDS PROFILE[1] pin. Profile Select Pins. Digital input. Use these pins to select one of eight profiles for the DDS.
#define PS2PIN 7               // DDS PROFILE[2] pin. Profile Select Pins. Digital input. Use these pins to select one of eight profiles for the DDS.
#define IO_UPDATEPIN  9        // DDS I/O_UPDATE pin. Digital input. A high on this pin transfers the contents of the buffers to the internal registers.
#define RESETPIN 10      // DDS MASTER_RESET pin. Digital input. Clears all memory elements and sets registers to default values.

#define SYNCIO_PIN 8
#define DROVER 2
#define DRHOLD 3
#define DRCTL 4

#define PLL_CS_PIN 1

#define SETLIST_TRIGGER 33 //we don't actually want to trigger this device, so specify an unused pin




//Declare the DDS object:
AD9914 DDS(CSPIN, RESETPIN, IO_UPDATEPIN, PS0PIN, PS1PIN, PS2PIN, OSKPIN);

//Declare the PLL object:
ADF4350 PLL(PLL_CS_PIN);

//Declare the setlist arduino object:
SetListArduino SetListImage(SETLIST_TRIGGER);

double amplitudeCorrdB(unsigned long freq){ //freq is in Hz
  if (freq > 1500000000) {
    return 0.0;
  } else {
    //return (-20.25 + 0.013*(freq/1000000.0)); //linear version
    //return (5.613e-12 * pow(freq/1000000.0,4) - 1.341e-8 * pow(freq/1000000.0,3) + 6.154e-6 * pow(freq/1000000.0,2) + 0.016*freq/1000000.0 - 21.035); //4th order polynomial version, smoothes output after AOM driver board
    return (-3.288e-12 * pow(freq/1000000.0,4) + 1.262e-8 * pow(freq/1000000.0,3) - 1.51e-5 * pow(freq/1000000.0,2) + 7.847e-3*freq/1000000.0 - 4.717); //4th order polynomial version, smoothes output after directional coupler and amplifier to about +28 dBm

  }
}

int maxFreqs;
unsigned long freq[8];
unsigned long delayMicros;


void setup() {
  delay(100);
  SPI.begin();
  SPI.setClockDivider(4);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  
  Serial.begin(115200);
  
  PLL.initialize(3500,10); //max PLL setting may be 3 GHz...
  
  delay(10);
  
  DDS.initialize(3500000000);
  DDS.enableProfileMode();
  DDS.enableOSK();
  //freq[0] = 1190000000;
  freq[0] = 1500000000;
  freq[1] = 620000000;
  freq[2] = 1190000000;
  freq[3] = 1435000000;
  maxFreqs = 1;
  delayMicros = 150;
  
  //unsigned long freq[8] = {1190000000, 620000000, 1190000000, 1435000000, 1700000000, 1000000000, 1200000000, 1500000000};
  //unsigned long freq[8] = {1190000000, 1435000000, 1190000000, 620000000, 1700000000, 1000000000, 1200000000, 1500000000};
  for (int i = 0 ; i < 8 ; i++) {
    DDS.setFreq(freq[i],i);
    DDS.setAmpdB(amplitudeCorrdB(freq[i]),i);
    //DDS.setAmpdB(-4.05,i);
  }
//  DDS.setFreq(900000000,0);
//  DDS.setAmpdB(0.0,0);
  //DDS.setAmp(0.1,0);
  //DDS.setFreq(91000000,1);
  //DDS.setFreq(92000000,2);
  //DDS.setFreq(93000000,3);
  //DDS.setFreq(94000000,4);
  //DDS.setFreq(95000000,5);
  //DDS.setFreq(96000000,6);
  //DDS.setFreq(5000000,7);
  DDS.selectProfile(0);
  //DDS.disableSyncClck();
  //power usage: 370 mA at 1.8V, 600mA at 3.3V
  
  SetListImage.registerDevice(DDS, 0);
  SetListImage.registerCommand("SF",0,setSF);
  SetListImage.registerCommand("MF",0,setMultiFreq);
}

void loop() {
  // put your main code here, to run repeatedly:
  SetListImage.readSerial();
  
 for (int i = 0 ; i < maxFreqs ; i++) {
   DDS.selectProfile(i);
   //delay(1);
   delayMicroseconds(delayMicros);
 } 
 
 //DDS.selectProfile(0);
 //DDS.setAmpdB(-3,0);
 //Serial.println(amplitudeCorrdB(25000000));


 //delay(5000);

}

void setSF(AD9914 * DDS, int * params){ //this mode just talks to profile 0
   unsigned long comFreq = params[0]; //input is desired frequency in MHz
   comFreq = comFreq*1000000; //convert to Hz
   DDS->setAmpdB(amplitudeCorrdB(comFreq));
   DDS->setFreq(comFreq);
}

void setMultiFreq(AD9914 * DDS, int * params){
  //Need 11 parameters max:
  //0 - 7; 8 frequencies
  //8: number of frequencies to use
  //9: delay time
  //10: amplitude correction
  
  //Define 8 frequencies:
  //unsigned long freqs[8];
  for (int i = 0 ; i < 8 ; i++) {
    freq[i] = params[i]*1000000;
  }
  
  maxFreqs = params[8];
  delayMicros = params[9];
  double ampCorr = params[10];
  
  for (int i = 0 ; i < 8 ; i++) {
    DDS->setAmpdB(amplitudeCorrdB(freq[i])-ampCorr,i);
    DDS->setFreq(freq[i],i);
 }
 

  
}

#include <SPI.h>
#include "lib/ADF4351.h"

#include <Arduino.h>

#define CLOCKSPEED 1000000
#define CS 15
#define MOSI 13
#define SCLK 14
#define CE 25

SPIClass * hspi = NULL;
ADF4351 * PLL = NULL; // declares object PLL of type ADF4351. Will initialize it below.
// mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION); // set before init() so that you can see startup messages
void setup(){
    double test = 0;
    hspi = new SPIClass(HSPI);
    PLL = new ADF4351(CS, CE, hspi);
    Serial.begin(115200);
    delay(500); // give it a sec to warm up
    hspi->begin();          // for communicating with DDS/PLLs

    // PLL->initialize(360.7, 100); // initialize the PLL to output 400 Mhz, using an
    // test = PLL->getFreq();
                            // onboard reference of 10Mhz
}

void loop(){

}
# The ESP32 PINOUT
* [LINK](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)

## Peripherals
* 18 ADC
* 3 SPI
* 3 UART
* 16 PWM
* 2 DAC
* 2 I2S

## 
* GPIO 6~11 Connected To SPI FLASH (Don't Use)
* 34,35,36,39 Only Inoput
* When Using WIFI ADC2(0,4,2..) Not Used
* DAC (GPIO25,26)
* PWM: All Pins Output Except 34-39 Could Do PWM
* IIC (21(SDA),22(SCL)) 
    * Binded To The Default Wire.begin();
    * If Want To Use Other Pins, Could Wire.begin(SCL,SDA);
* SPI
     * Default Setting
    * |SPI|MOSI|MISO|SCK|CS|
      |--|--|--|--|--|
      |VSPI|23|19|18|5| 
      |HSPI|13|12|14|15|
* INTR: All GPIO Could Be Configured As INTR
* HIGH When Reboot:
    * GPIO 1,3,5,6-11,14,15
/* 
   ADF4351.h - ADF4351 PLL Communication Library
   Created by Neal Pisenti, 2013.
   JQI - Strontium - UMD

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   aunsigned long with this program.  If not, see <http://www.gnu.org/licenses/>.


 */

#ifndef ADF4351_h
#define ADF4351_h

#include "Arduino.h"

class ADF4351
{
    public: 
        // Constructor function. 
        // Creates PLL object, with given SS pin
        ADF4351(byte, byte, SPIClass *);

        // Initialize with initial frequency, refClk (defaults to 10Mhz); 
        void initialize(double, double);


        // powers down the PLL/VCO
        void powerDown(bool);
        void setRfPower(int);
        void setAuxPower(int);
        void auxEnable(bool);
        void rfEnable(bool);

        // Gets current frequency
        double getFreq();

        // Sets frequency
        void setFreq(double);

        void setInt(int);

        void setFeedbackType(bool);

        void update();

        unsigned long _phase;
        unsigned long _int, _mod, _frac, _divider, _auxPower, _rfPower;
        double _freq, _refFreq, _PFDFreq, _n, _channelSpacing;
    private:
        // Instance variables that hold pinout mapping
        byte _CS, _CE;
        SPIClass * _hspi;
        unsigned long _rcounter;
        // Instance variables for ...
        bool _powerdown, _auxEnabled, _rfEnabled, _feedbackType, _prescaler;

        // register values
        byte _r0[4], _r1[4], _r2[4], _r3[4], _r4[4], _r5[4];

        unsigned gcd_iter(unsigned long int, unsigned long int);

        // function to write data to register.
        void setR0();
        void setR1();
        void setR2();
        void setR3();
        void setR4();
        void setR5();
        void writeRegister(byte[4]);



};
 

#endif

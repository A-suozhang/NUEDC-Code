/* 
   AD9914.h - AD9914 DDS communication library
   Created by Ben Reschovsky, 2016.
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

#ifndef AD9914_h
#define AD9914_h

#include "Arduino.h"

class AD9914
{
    public: 
        // Constructor function. 
        AD9914(byte ssPin, byte resetPin, byte updatePin, byte ps0, byte ps1, byte ps2, byte osk);

        // Initialize with refClk frequency
        void initialize(unsigned long);

        // Initialize with refIn frequency, and clock multiplier value
        //void initialize(unsigned long, byte);

        // Reset the DDS
        void reset();

        // Update to load newly written settings
        void update();
        
        // Gets current frequency
        unsigned long getFreq(byte);
        unsigned long getFreq();
        
        // Sets frequency
        void setFreq(unsigned long, byte);
        // Sets frequency
        void setFreq(unsigned long); //default to profile 0

        // Gets current frequency tuning word
        unsigned long getFTW(byte);
        unsigned long getFTW();
        
        // Sets frequency tuning word
        void setFTW(unsigned long, byte);
        void setFTW(unsigned long);
        
        //write scaled amplitude for the selected profile
        void setAmp(double scaledAmp, byte profile);
        void setAmp(double scaledAmp);
        void setAmpdB(double scaledAmpdB, byte profile);
        void setAmpdB(double scaledAmpdB);
        
        // Gets current amplitude
        double getAmp(byte);
        double getAmp();
        
        // Gets current amplitude in dB
        double getAmpdB(byte);
        double getAmpdB();
        
        // Gets current amplitude tuning word
        unsigned long getASF(byte);
        unsigned long getASF();
        

        // places DDS in linear sweep mode
        //void linearSweep(unsigned long, unsigned long, unsigned long, byte, unsigned long, byte);

        //enable profile mode
        void enableProfileMode();

        //disable profile mode
        void disableProfileMode();
        
        //enable OSK
        void enableOSK();
        
        //disable OSK
        void disableOSK();
        
        //Get profile mode status
        boolean getProfileSelectMode();
        
        //Get OSK mode status
        boolean getOSKMode();

        //enable the Sync Clck output
        void enableSyncClck();

        //disable the Sync Clck output
        void disableSyncClck();
        
        //Change active profile mode:
        void selectProfile(byte);
        
        //Get currently active profile
        byte getProfile();
        
        
        


    private:
        // Instance variables that hold pinout mapping
        // from arduino to DDS pins.
        byte _ssPin, _resetPin, _updatePin, _ps0, _ps1, _ps2, _osk;

        // Instance variables for frequency _freq, frequency tuning word _ftw,
        // reference clock frequency _refClk, etc.
        unsigned long _freq[8], _ftw[8], _refClk, _refIn, _asf[8];
        double _scaledAmp[8], _scaledAmpdB[8];
        
        byte _activeProfile;
        
        // Instance variables to keep track of the DDS mode:
        boolean _profileModeOn, _OSKon;


        // function to write data to register. 
        void writeRegister(byte[2], byte[1024]);
        
        // function to toggle the DAC CAL register bit and calibrate the DAC
        void dacCalibrate();
        
        // write amplitude tuning word to device
        void writeAmp(long ampScaleFactor, byte profile);

        // DDS frequency resolution
         double RESOLUTION;// = 4294967296; // sets resolution to 2^32 = 32 bits. Using type double to avoid confusion with integer division...

};

#endif

#pragma once

#include <math.h>

#ifndef _AGCAMPLIFIER_
#define _AGCAMPLIFIER_

class AGCAmplifier{
public:
    double adjspd=0.001;
    double maxgain=10;

    double ampnow=0;
    double ampnowfilt=0.5;
    AGCAmplifier(double adjspd_,double maxgain_){
        adjspd=adjspd_;
        maxgain=maxgain_;
    }
    unsigned char feed(long in){
        double inputnorm=(double)in/2147483647.0;
        double inputabs=fabs(inputnorm);
        if(inputabs>ampnow){
            ampnow=inputabs;
        }else{
            ampnow=0.95*ampnow;
        }
        ampnowfilt=(1.0-adjspd)*ampnowfilt+adjspd*ampnow;
        double gain=1.0;
        if(ampnowfilt>0.0){
            gain=1.0/ampnowfilt;
        }
        double inputamp=inputnorm*0.6;
        if(gain>maxgain){
            inputamp*=maxgain;
        }else{
            inputamp*=gain;
        }
        if(inputamp>0.95)inputamp=0.95;
        if(inputamp<-0.95)inputamp=-0.95;
        unsigned char quan=127+(unsigned char)(127.0*inputamp);
        return quan;
    }
};


#endif
#include <Arduino.h>
#include <HardwareSerial.h>
#include "UIX/UIX.h"


UIX uix;

//button click callbacks
void btnsendclick(int tag,UIXButton* obj);
void btnclearrecvclick(int tag,UIXButton* obj);

char sendstr[100]="";
char recvstr[100]="";
UIXInputBox inptsend(20,120,20,50,COLVSBLUE,COLCYAN,sendstr);   //input box
UIXButton btnsend(20,90,60,90,COLVSBLUE,COLCYAN,"Fuck",btnsendclick);


//called when btnsend is clicked
void btnsendclick(int tag,UIXButton* obj){
    Serial.print(sendstr);
}


void setup(){
    Serial.begin(115200);

    uixuserpanelnum=1;  //no more than 4
    uixpanels[0].label="SerialTest";
    //add objects to panel 0
    uixpanels[0].uixobjects+=inptsend;
    uixpanels[0].uixobjects+=btnsend;

    uix.begin();
}

void loop(){
    uix.tick();


}


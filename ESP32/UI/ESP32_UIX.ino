#include <Arduino.h>
#include <HardwareSerial.h>
#include "UIX/UIX.h"

HardwareSerial SerialTest(2);

UIX uix;

//button click callbacks
void btnsendclick(int tag,UIXButton* obj);
void btnclearrecvclick(int tag,UIXButton* obj);

char sendstr[100]="";
char recvstr[100]="";
UIXInputBox inptsend(20,120,20,50,COLVSBLUE,COLCYAN,sendstr);   //input box
UIXButton btnsend(20,90,60,90,COLVSBLUE,COLCYAN,"Send",btnsendclick);
UIXDynamicText txtrecv(recvstr,COLWHITE,20,100,1,0);
UIXButton btnclearrecv(20,90,118,148,COLVSORANGE,COLORANGE,"Clear",btnclearrecvclick);

//called when btnsend is clicked
void btnsendclick(int tag,UIXButton* obj){
    SerialTest.print(sendstr);
}

//called when btnclearrecv is clicked
void btnclearrecvclick(int tag,UIXButton* obj){
    recvstr[0]='\0';    //clear received string
    txtrecv.invalidate();   //refresh dynamictext
}

void setup(){
    Serial.begin(115200);
    SerialTest.begin(115200,SERIAL_8N1,2,15);   //RX pin 2, TX pin 15

    uixuserpanelnum=1;  //no more than 4
    uixpanels[0].label="SerialTest";
    //add objects to panel 0
    uixpanels[0].uixobjects+=inptsend;
    uixpanels[0].uixobjects+=btnsend;
    uixpanels[0].uixobjects+=txtrecv;
    uixpanels[0].uixobjects+=btnclearrecv;

    uix.begin();
}

void loop(){
    uix.tick();

    //check serial
    bool received=false;
    while(SerialTest.available()){
        received=true;
        int inspos=strlen(recvstr);
        if(inspos<50){
            char incoming=SerialTest.read();
            recvstr[inspos]=incoming;
            Serial.write(incoming);
        }
        recvstr[inspos+1]='\0';
    }
    //refresh dynamictext
    if(received) txtrecv.invalidate();
}


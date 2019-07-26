#include <Arduino.h>
#include <HardwareSerial.h>
#include "src/UIX/UIX.h"
#include "src/AD9910.h"
#include <String.h>
#include <SPI.h>
#include <WiFi.h>

// Set Up The WIFI_ROUTER(AP)  Info
const char* ssid     = "awsl";
const char* password = "1qaz2wsx";
WiFiServer server(100);

// -------- A Few Define ------------
# define cs 15
# define rst 25
# define update 26
# define sdio 13
# define sclk 14

# define CLOCKSPEED 1000000
# define uchar unsigned char



// ------ Setting Up A Few Elements -------------
SPIClass * hspi = NULL;
AD9910 * DDS = NULL;
UIX uix;

double freq = 1000000.0;    // real_freq*100
double amp = 500.0;

//button click callbacks
void btnsendclick0(int tag,UIXButton* obj);
void btnsendclick1(int tag,UIXButton* obj);
void btnsendclick2(int tag,UIXButton* obj);

char sendstr_freq[100]="";
char sendstr_amp[100]="";
char str_freq[100]="Frequency";
char str_amp[100]="Amplitude";

UIXInputBox inptsend0(20,120,20,50,COLVSBLUE,COLCYAN,sendstr_freq);   //input box
UIXInputBox inptsend1(20,120,50,70,COLVSBLUE,COLCYAN,sendstr_amp);   //input box

UIXButton btnsend0(20,120,80,110,COLVSBLUE,COLCYAN,"Sweep",btnsendclick0);
UIXButton btnsend1(130,190,20,50,COLVSBLUE,COLCYAN,"Set Freq",btnsendclick1);
UIXButton btnsend2(130,190,50,70,COLVSBLUE,COLCYAN,"Set Amp",btnsendclick2);


//called when btnsend is clicked
void btnsendclick0(int tag,UIXButton* obj){
    Serial.println("Sweep!");
    for (int i=0;i<40;i++){
        DDS->set_freq(1000000*(i+1));
    }
}

void btnsendclick1(int tag,UIXButton* obj){
    Serial.print("Get Freq:");
    DDS->set_freq(atof(sendstr_freq));
    Serial.println(float(atoi(sendstr_freq)));
}
void btnsendclick2(int tag,UIXButton* obj){
    Serial.print("Get Amp:");
    DDS->set_Amp(atof(sendstr_amp));
    Serial.println(sendstr_amp);
}

void setup(){

    // Set Up Hardware
    Serial.begin(115200);
    // Set Up The SPI
    hspi = new SPIClass(HSPI);
    if(hspi==NULL)
        Serial.print("SPI INITIALIZATION FAILED.");
    hspi->begin();
    //SCLK = 14, MISO = 12, MOSI = 13, SS = 15
    DDS = new AD9910(cs,rst,update,sdio,sclk,hspi);
    DDS->begin();
    // Initial_Value
    DDS->set_Amp(500.0);
    DDS->set_freq(1000000.0);

    // Set Up The WIFi server
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid,password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected."); 
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();

    // Set Up The UI
    uixuserpanelnum=1;  //no more than 46
    uixpanels[0].label="SerialTest";
    //add objects to panel 0
    uixpanels[0].uixobjects+=inptsend0;
    uixpanels[0].uixobjects+=inptsend1;
    uixpanels[0].uixobjects+=btnsend0;
    uixpanels[0].uixobjects+=btnsend1;
    uixpanels[0].uixobjects+=btnsend2;
    uix.begin();
}

void loop(){
    // Dynamically Update The UIX
    uix.tick();
    Serial.println("Looping...");

    // 
    WiFiClient client = server.available();   // Listen for incoming clients
    if (client) {                             
        Serial.println("New Client.");          // print a message out in the serial port
        while (client.connected()) {    // (client.connected() stays true, when there are unread data, even if the client has disconnected)
        if (client.available()) {             // if there's bytes to read to the client,
            // Read Client's Respond
            char c = client.read();  
            client.write(c);
            Serial.write(c);   

        }                 // print it out the serial monitor
        }

    // After The client disconnected
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    }



}


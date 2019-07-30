#include <Arduino.h>
#include <HardwareSerial.h>
#include "src/UIX/UIX.h"
#include "src/AD9910.h"
#include "src/AD8750.h"
#include <String.h>
#include <SPI.h>
#include <WiFi.h>
#include <driver/adc.h>
#include <driver/dac.h>
#include <esp_log.h>

static const char* TAG = "My_ESP32";

// Set Up The WIFI_ROUTER(AP)  Info
// const char* ssid     = "F534";
// const char* password = "nuedc2019";
const char* ssid     = "awsl";
const char* password = "1qaz2wsx";
WiFiServer server(100);
HardwareSerial Serial0(1);

// -------- A Few Define ------------
# define cs 15
# define update 12                                     
# define sdio 13
# define sclk 14

# define adc_pin 36

# define dac0_pin 25
# define dac1_pin 26

# define uart_tx_pin 1
# define uart_rx_pin 3

# define CLOCKSPEED 1000000
# define uchar unsigned char



// ------ Setting Up A Few Elements -------------
SPIClass * hspi = NULL;
AD9910 * DDS = NULL;
AD8750 * VGA = NULL;
UIX uix;

double freq = 1000000.0;    // real_freq*100
double amp = 500.0;

int state = 0;

//button click callbacks
void btnsendclick0(int tag,UIXButton* obj);
void btnsendclick1(int tag,UIXButton* obj);
void btnsendclick2(int tag,UIXButton* obj);
void btnsendclick3(int tag,UIXButton* obj);
void btnsendclick4(int tag,UIXButton* obj);
void btnsendclick5(int tag,UIXButton* obj);
void btnsendclick6(int tag,UIXButton* obj);
void btnsendclick7(int tag,UIXButton* obj);

char sendstr_freq[100]="";
char sendstr_amp[100]="";
char sendstr_gain[100]="";
char str_freq[100]="Frequency";
char str_amp[100]="Amplitude";

bool esp32_data_ready_flag = false;
bool start_uart_transfer = false;

unsigned long last_time;
unsigned long last_time1;
unsigned long last_time2;


UIXInputBox inptsend0(20,120,20,50,COLVSBLUE,COLCYAN,sendstr_freq);   //input box
UIXInputBox inptsend1(20,120,60,90,COLVSBLUE,COLCYAN,sendstr_amp);   //input box
UIXInputBox inptsend2(20,120,100,130,COLVSBLUE,COLCYAN,sendstr_gain);   //input box

UIXButton btnsend0(20,120,150,180,COLVSBLUE,COLCYAN,"Sweep",btnsendclick0);
UIXButton btnsend1(130,190,20,50,COLVSBLUE,COLCYAN,"Set Freq",btnsendclick1);
UIXButton btnsend2(130,190,60,90,COLVSBLUE,COLCYAN,"Set Amp",btnsendclick2);
UIXButton btnsend3(130,190,100,130,COLVSBLUE,COLCYAN,"Set Gain",btnsendclick3);
UIXButton btnsend4(20,70,190,220,COLVSBLUE,COLCYAN,"DAC",btnsendclick4);
UIXButton btnsend5(80,130,190,220,COLVSBLUE,COLCYAN,"UART",btnsendclick5);
UIXButton btnsend6(140,190,190,220,COLVSBLUE,COLCYAN,"WiFi",btnsendclick6);
UIXButton btnsend7(20,180,10,160,COLVSBLUE,COLCYAN,"Start Transfer",btnsendclick7);

uint16_t val;
uint16_t val1;
uint16_t data_buffer[40];

uint16_t adc_read(){
    unsigned long adc_sum = 0;
    uint16_t adc_result = 0;
    uint16_t adc_temp = 0;
    for (int i = 0; i<50;i++){
        adc_temp = adc1_get_raw(ADC1_CHANNEL_6);
        adc_sum = adc_sum + adc_temp;
        // Serial.println(adc_sum);
    }
    adc_result = adc_sum/50;
    return adc_result;
}


//called when btnsend is clicked
void btnsendclick0(int tag,UIXButton* obj){
    esp32_data_ready_flag = 0;
    Serial.println("Sweep!");
    for (int i=0;i<40;i++){
        DDS->set_freq(1000000*(i+1));
        // ADC Read
        delay(100);
        // val = adc1_get_raw(ADC1_CHANNEL_6);
        val1 = 0x0FFF - adc_read();             // Full 12 bit - ADC_Result
        // Serial.print("The ADC Value Is:");
        // Serial.println(val);
        ESP_LOGI(TAG, "At %d Iter, Getting The ADC VALUE %d",i,val1);
        // Fill data_buffer
        data_buffer[i] = val1;
        
    }
    esp32_data_ready_flag = true;
}


// The Call Back For Freq Set
void btnsendclick1(int tag,UIXButton* obj){
    esp32_data_ready_flag = 0;
    Serial.print("Get Freq:");
    DDS->set_freq(atof(sendstr_freq));
    Serial.println(float(atoi(sendstr_freq)));
    delay(5);   // Wait A while
    // val = adc1_get_raw(ADC1_CHANNEL_6);         // 36 is The ADC CH
    val1 = adc_read();
    // Serial.print("The ADC Value Is:");
    // Serial.println(val);
    // Serial.println(val1);
    ESP_LOGI(TAG, "Getting The ADC VALUE %d",val1);
}

void btnsendclick2(int tag,UIXButton* obj){
    esp32_data_ready_flag = 0;
    Serial.print("Get Amp:");
    DDS->set_Amp(atof(sendstr_amp));
    Serial.println(sendstr_amp);
    delay(5);   // Wait A while
    // val = adc1_get_raw(ADC1_CHANNEL_6);
    val1 = adc_read();
    // Serial.print("The ADC Value Is:");
    // Serial.println(val);
    ESP_LOGI(TAG, "Getting The ADC VALUE %d",val1);
}

void btnsendclick3(int tag,UIXButton* obj){
    esp32_data_ready_flag = 0;
    VGA -> AD8750_WriteByte(uint8_t(atoi(sendstr_gain)));
    Serial.print("Get Gain:");
    Serial.println(uint8_t(atoi(sendstr_gain)));
    delay(5);   // Wait A while
}

void btnsendclick4(int tag,UIXButton* obj){
    state = 0;
    ESP_LOGI(TAG, "Entering DAC MODE");
}

void btnsendclick5(int tag,UIXButton* obj){
    state = 1;
    ESP_LOGI(TAG, "Entering UART MODE");
}

void btnsendclick6(int tag,UIXButton* obj){
    state = 2;
    ESP_LOGI(TAG, "Entering WiFi MODE");
}

void btnsendclick7(int tag,UIXButton* obj){
    start_uart_transfer = true;
    ESP_LOGI(TAG, "Starting UART Transfer...");
}


void setup(){

    // Set Up Serial
    Serial.begin(115200);
    Serial0.begin(115200,SERIAL_8N1,35,27);
    ESP_LOGI(TAG, "Serial Initialized");

    // Set Up ADDA
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);  // 6dB: Dynamic Range 2.2V
    // adc1_get_raw(ADC1_CHANNEL_6);
    dac_output_enable(DAC_CHANNEL_1);   // 25
    dac_output_enable(DAC_CHANNEL_2);   // 26
    // dac_output_voltage(DAC_CHANNEL_1, 200);
    ESP_LOGI(TAG, "ADDA Initialized");
    

    // Set Up The SPI
    hspi = new SPIClass(HSPI);
    if(hspi==NULL)
        Serial.print("SPI INITIALIZATION FAILED.");
    hspi->begin();
    //SCLK = 14, MISO = 12, MOSI = 13, SS = 15
    DDS = new AD9910(cs,update,sdio,sclk,hspi);
    DDS->begin();
    // Initial_Value
    DDS->set_Amp(500.0);
    DDS->set_freq(1000000.0);
    ESP_LOGI(TAG, "DDS Initialized");

    VGA = new AD8750(2,sclk,sdio,hspi);
    VGA -> begin();
    ESP_LOGI(TAG, "VGA Initialized");

    // Set Up The UI
    uixuserpanelnum=2;  //no more than 46
    uixpanels[0].label="2017 H";
    uixpanels[1].label="Uart Mode";
    //add objects to panel 0
    uixpanels[0].uixobjects+=inptsend0;
    uixpanels[0].uixobjects+=inptsend1;
    uixpanels[0].uixobjects+=inptsend2;
    uixpanels[0].uixobjects+=btnsend0;
    uixpanels[0].uixobjects+=btnsend1;
    uixpanels[0].uixobjects+=btnsend2;
    uixpanels[0].uixobjects+=btnsend3;
    uixpanels[0].uixobjects+=btnsend4;
    uixpanels[0].uixobjects+=btnsend5;
    uixpanels[0].uixobjects+=btnsend6;
    uixpanels[1].uixobjects+=btnsend7;
    uix.begin();

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
}

void loop(){

    // * ------------------ Dynamically Update The UIX ----------------------
    uix.tick();

    // * ------------------ DAC OUTPUT DATA ------------------------------
    if (state == 0){
        
        if (esp32_data_ready_flag){
            for (int i = 0;i<39;i++){
                for(int j = 0;j<60;j++){
                    dac_output_voltage(DAC_CHANNEL_1,(uint8_t)(i*6+j/10));
                    dac_output_voltage(DAC_CHANNEL_2,(uint8_t)((data_buffer[i] + (data_buffer[i+1] - data_buffer[i])*(j/10+1)/6 ) >> 4));
                }
                
                // ESP_LOGI(TAG, "Writing Through DAC The %d 's Iter: %d", (uint8_t)(i*6), (uint8_t)(data_buffer[i] >> 4));
                // delayMicroseconds(1);   
            }
        }
    }

    // * -------------------- The WIFI Part ------------------------------
    if (state == 2){
        unsigned long temp_millis = millis();
        if (temp_millis - last_time > 2000){
            last_time = temp_millis;
            Serial.println("Waiting For Client");
        }

        bool sending_data = false;
        WiFiClient client = server.available();   // Listen for incoming clients
        if (client) {                             
            Serial.println("New Client.");          // print a message out in the serial port
                // Only If The Data Is Ready
            while(client.connected()){
                // --------------- Check If Client Is Alive ------------------
                // unsigned long temp_millis1 = millis();
                // if (temp_millis1 - last_time1 > 10000){
                //     last_time1 = temp_millis1;
                //     if (!sending_data){
                //         client.write(0xFF);
                //     }
                //     delay(500);

                //     if (client.available()){
                //         char c = client.read();
                //         // Serial.print("Received From Client:");
                //         Serial.print(c);
                //         if (c != 0x00){
                //             Serial.print(c);
                //             client.stop();
                //             Serial.println("Detected Dead Client");
                //         }
                //         else{
                //             Serial.println("The Client Is Alive");
                //         }

                //     }
                //     else{
                //         client.stop();
                //         Serial.println("No Data Back, Detected Dead Client");
                //     }
                // }
                // ---------------------------------------------------------------

                if (Serial.available()){
                    char c = Serial.read();
                    client.print(c);
                }

                if (client.available()){
                    char c = client.read();
                    Serial.print(c);
                    // TODO: Fill The TCP Communication Design
                    if (c == '!'){
                        sending_data = true;
                        // Send Out The Data
                        client.write((uint8_t)0x00);
                        for (uint8_t i = 0;i<40;i++){
                            client.write((uint8_t)(data_buffer[i] >> 4));
                        }
                        sending_data = false;
                    }
                }


                
            }
            
        
            // ---------- Send Data To Client Test ----------------
            // while (client.connected()) {    // (client.connected() stays true, when there are unread data, even if the client has disconnected)
            // if (client.available()) {             // if there's bytes to read to the client,
            //     // Read Client's Respond
            //     char c = client.read();  
            //     client.write(c);
            //     Serial.write(c);   
            //     }                 // print it out the serial monitor
            // }
            // -------------------------------------------------


            // After The client disconnected
            client.stop();
            Serial.println("Client disconnected.");
            Serial.println("");
        }
    }


    // * ------------------- The UART PART ---------------------------

    if (state == 1){

        if (start_uart_transfer){
            // Serial.write(0x00); // Send 2 0x00 To Denote The Start Of A Frame
            // Serial.write(0x00);
            for (int i = 0;i<40;i++){
                if ((uint8_t)(data_buffer[i]) == 0x00){
                    Serial.write(0x01);
                    Serial0.write(0x01);
                }
                else if ((uint8_t)(data_buffer[i]) == 0xFF){
                    Serial.write(0xFE);
                    Serial0.write(0xFE);
                }
                else {
                    Serial.write((uint8_t)(data_buffer[i] >> 4));
                    Serial0.write((uint8_t)(data_buffer[i] >> 4));
                } 
            }
            ESP_LOGI(TAG, "Finished Transmitting A Packet Through UART");
            start_uart_transfer = false;
        }
    }

    
   



}


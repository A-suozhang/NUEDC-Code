# include<Arduino.h>
#include <HardwareSerial.h>

# define uchar unsigned char
# define MAX_LEN 1024

uchar temp;
int i;

// ---- Hand Craft INTR ----------
unsigned long temp_millis0;
unsigned long temp_millis1;
unsigned long last_time0;
unsigned long last_time1;

uchar head = 0;
uchar cnt0 = 0;
uchar true_head = 0xEE;
uchar tails[4] = {0,0,0,0};
uchar true_tail[4] = {0xFF,0xFC,0xFF,0xFF};
uchar temp_frame[MAX_LEN] = {0};
String s;
String frame;
int cnt = 0;    // cnt is length + 1

// ------------------------------
HardwareSerial Serial0(2);

void write_string(uchar ID1,uchar ID2, String s){
    Serial0.write(0XEE);
    Serial0.write(0XB1);
    Serial0.write(0X10);
    Serial0.write(0X00);
    Serial0.write(ID1);
    Serial0.write(0x00);
    Serial0.write(ID2);
    Serial0.printf("%s",s);
    Serial0.write(0xFF);
    Serial0.write(0xFC);
    Serial0.write(0xFF);
    Serial0.write(0xFF);
}

void write_float(uchar ID1,uchar ID2, float f){
    Serial0.write(0XEE);
    Serial0.write(0XB1);
    Serial0.write(0X10);
    Serial0.write(0X00);
    Serial0.write(ID1);
    Serial0.write(0x00);
    Serial0.write(ID2);
    Serial0.printf("%f",f);
    Serial0.write(0xFF);
    Serial0.write(0xFC);
    Serial0.write(0xFF);
    Serial0.write(0xFF);
}

void write_char(uchar ID1,uchar ID2, uchar c){
    Serial0.write(0XEE);
    Serial0.write(0XB1);
    Serial0.write(0X10);
    Serial0.write(0X00);
    Serial0.write(ID1);
    Serial0.write(0x00);
    Serial0.write(ID2);
    Serial0.print(c);
    Serial0.write(0xFF);
    Serial0.write(0xFC);
    Serial0.write(0xFF);
    Serial0.write(0xFF);
}


void change_scene(uchar ID1){
    Serial0.write(0xEE);
    Serial0.write(0xB1);
    Serial0.write(0x00);
    Serial0.write(0x00);
    Serial0.write(ID1);
    Serial0.write(0xFF);
    Serial0.write(0xFC);
    Serial0.write(0xFF);
    Serial0.write(0xFF);
}

void write_chart(uchar ID1,uchar ID2,uchar ID3,unsigned short data_num,uchar data_buffer[]){
    Serial0.write(0XEE);
    Serial0.write(0XB1);
    Serial0.write(0X35);
    Serial0.write(0X00);
    Serial0.write(ID1);
    Serial0.write(0x00);
    Serial0.write(ID2);
    Serial0.write(ID3);
    Serial0.write((data_num & 0xFF00) >> 8);
    Serial0.write(data_num & 0x00FF);
    for(unsigned short i=0;i<data_num;i++){
        Serial0.write(data_buffer[i]);
    }
    Serial0.write(0xFF);
    Serial0.write(0xFC);
    Serial0.write(0xFF);
    Serial0.write(0xFF);
}

void empty_graph(uchar ID1,uchar ID2,uchar ID3){
    Serial0.write(0xEE);
    Serial0.write(0xB1);
    Serial0.write(0x33);
    Serial0.write(0x00);
    Serial0.write(ID1);
    Serial0.write(0x00);
    Serial0.write(ID2);
    Serial0.write(0x00);
    Serial0.write(0xFF);
    Serial0.write(0xFC);
    Serial0.write(0xFF);
    Serial0.write(0xFF);
}



void setup(){
    Serial.begin(921600);
    Serial0.begin(921600,SERIAL_8N1,14,12);

    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
}

void loop(){


    digitalWrite(25,HIGH);
    digitalWrite(26,HIGH);

    // ---------- Receiving Data From The Screen -----------
    // Capture The Frame
    // Capture The Frame
    if(Serial0.available()){
        head = Serial0.read();
        if (head == true_head){
            temp_frame[0] = head;
            // Start Reading 
            // First Read 4 Values
            if (Serial0.available()){
                temp_frame[1] = Serial0.read();
            }
            if (Serial0.available()){
                temp_frame[2] = Serial0.read();
            }
            if (Serial0.available()){
                temp_frame[3] = Serial0.read();
            }
            if (Serial0.available()){
                temp_frame[4] = Serial0.read();
            }
            cnt = 5;    // The temp_frame Has 5 Values

            while(!(temp_frame[cnt-1] == 0xFF && temp_frame[cnt-2] == 0xFF && temp_frame[cnt-3] == 0xFC && temp_frame[cnt-4] == 0xFF)){
                // Detect The Tail, Only When Detecting Tail, Stop Reading
                if (Serial0.available()){
                    temp_frame[cnt] = Serial0.read();
                    cnt = cnt + 1;
                }
            // Print The len
            }

            for (int j=0;j<cnt;j++){
                Serial.print(temp_frame[j],HEX);
                // Serial.print(",");
                // Serial.print(j,DEC);
                Serial.print(" ");
            }
            Serial.println(" ");

            uchar scene_id;
            uchar element_id;
            int where_dot = 0;
            String Data_RX;
            int Data_RX_int;
            float Data_RX_float = 0.0;

            // ------------Decode The Temp Frame ------------      
            if (temp_frame[1] == 0xB1 && temp_frame[2] == 0x11 && temp_frame[7] == 0x11){
                scene_id = temp_frame[4];
                element_id = temp_frame[6];
                // Scan For Float Input
                for(int i=0;i<cnt;i++){
                    if (temp_frame[i] == 46 ){  // The ASCII For '.'
                        where_dot = i;
                    }
                }

                if (where_dot == 0){        // The Input Is An Integer
                    for (int i=8;i<cnt-4;i++){
                        Data_RX += temp_frame[i] - 48;
                    }
                    Data_RX_int = Data_RX.toInt();
                    Serial.print("The Decoded Integer Value: ");
                    Serial.println(Data_RX_int);
                }
                else {
                    for (int i=8;i<where_dot;i++){
                        Data_RX_float += (temp_frame[i] - 48)*pow(10,where_dot-i-1);
                    }
                    for (int i=where_dot+1;i<cnt-4;i++){
                        if (temp_frame[i] > 0){         // Dirty IMplemention, Sometimes The Frame Captured has one more 0
                            Data_RX_float += (temp_frame[i] - 48)*pow(10,where_dot-i);
                        }
                    }
                    Serial.print("The Decoded Float Value: ");
                    Serial.println(Data_RX_float);
                }
            }
            
            temp_frame[MAX_LEN] = {0};
            cnt = 0;
            head = 0;

            while(Serial0.read()>0){};
            
        }
    
    }

    // -------------- The TX Part --------------------
    
    uchar scene_id;
    float temp_float;
    uchar data_buffer[1000];
    uchar data_buffer1[1000];
    if (Serial.available()){
        temp = Serial.read();
        if (temp == 0x00){
            // DO Change Scene
            scene_id = Serial.read();
            change_scene(scene_id);
            while(Serial.read() > 0){};     // Empty BUFFER
        }
        if (temp == 0x01){
            // DO write_float
            // temp_float = Serial.parseFloat();
            write_float(0x00,0x04,3.14);
            while(Serial.read() > 0){};     // Empty BUFFER
        }
        if (temp == 0x02){
            // Write Graph
            // The Input Format Is char
            // Should Quantize
            for (int k=0;k<50;k++){
                data_buffer[k] = (uchar)(int)(255*sin(2*PI*0.01*k));
            }
            write_chart(0x02,0x01,0x00,(unsigned short)50,data_buffer);
            for (int k=0;k<50;k++){
               data_buffer[k] = 0;
            }
            write_chart(0x02,0x01,0x00,(unsigned short)30,data_buffer);
        }
        if (temp == 0x03){
            empty_graph(0x02,0x01,0x00);
        }
        if (temp == 0x04){
            cnt0 = cnt0+1;
            for (int k=0;k<50;k++){
                // data_buffer[k] = (uchar)(int)(127*sin(2*PI*0.1*(k+1+50*cnt0))+128);
                data_buffer[k] = cnt0;
            }
            write_chart(0x02,0x01,0x00,(unsigned short)50,data_buffer);
        }     
    }

    // *** Test Graph Update ***

    // temp_millis0 = millis();

    // if (temp_millis0 - last_time0 > 100){
    //     last_time0 = temp_millis0;

        
    //     cnt0 = cnt0+1;
    //     for (int k=0;k<100;k++){
    //         // data_buffer[k] = (uchar)(int)(127*sin(2*PI*0.1*(k+1+50*cnt0))+128);
    //         data_buffer[k] = (uchar)(int)(127*sin(2*PI*0.075*(k+cnt0))+128);
    //         data_buffer1[k] = (uchar)(int)(127*sin(2*PI*0.15*(k+cnt0))+128);
    //     }
    //     // empty_graph(0x02,0x01,0x00);
    //     write_chart(0x02,0x01,0x00,(unsigned short)100,data_buffer);
    //     write_char(0x02,0x04,cnt0);
    //     write_char(0x02,0x05,cnt0);
    //     write_chart(0x02,0x01,0x01,(unsigned short)100,data_buffer1);
        
    // }

    // *** Testing Serial
    // uchar val;
    // if (Serial0.available()){
    //     val = Serial0.read();
    //     Serial.print(val,HEX);
    // }




}
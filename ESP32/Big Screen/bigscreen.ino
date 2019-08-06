# include<Arduino.h>
# include <HardwareSerial.h>
# include<esp_log.h>

# define uchar unsigned char
# define MAX_LEN 1024

const char* TAG = "ESP32_BIG_SCREEN";

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


// ----------------------------
// Graph Params
float slide_x_value;
float slide_y_value;
int move_x_value = 0;
int move_y_value = 0;

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

void modify_chart(uchar ID1, uchar ID2, float scale_x, float scale_y,int move_x,int move_y){
    Serial0.write(0xEE);
    Serial0.write(0xB1);
    Serial0.write(0x34);
    Serial0.write(0x00);
    Serial0.write(ID1);
    Serial0.write(0x00);
    Serial0.write(ID2);

    signed short temp_move_x = (-1)*move_x;
    Serial0.write((temp_move_x & 0xFF00) >> 8);
    Serial0.write(temp_move_x & 0x00FF);

    unsigned short temp_scale_x = (unsigned short)(1000/(scale_x+0.1));  // Avoid / '0'
    if (temp_scale_x > 10000){
        temp_scale_x = 10000;
    }
    Serial.printf("The Scale X is: %d",temp_scale_x);
    Serial0.write((temp_scale_x & 0xFF00) >> 8);
    Serial0.write(temp_scale_x & 0x00FF);

    
    signed short temp_move_y = move_y;
    Serial0.write((temp_move_y & 0xFF00) >> 8);
    Serial0.write(temp_move_y & 0x00FF);
    
    unsigned short temp_scale_y = (unsigned short)(100/(scale_y+0.1));
    if (temp_scale_y > 1000){
        temp_scale_y = 1000;
    }
    Serial0.write((temp_scale_y & 0xFF00) >> 8);
    Serial0.write(temp_scale_y & 0x00FF);


    Serial0.write(0xFF);
    Serial0.write(0xFC);
    Serial0.write(0xFF);
    Serial0.write(0xFF);
    
}



// Muda Muda
// void advanced_write_chart(uchar ID1,uchar ID2,uchar ID3,unsigned short data_num,uchar data_buffer[],float scale_x=1, float scale_y=1,int move_x=0,int move_y=0 ){
//     Serial0.write(0XEE);
//     Serial0.write(0XB1);
//     Serial0.write(0X35);
//     Serial0.write(0X00);
//     Serial0.write(ID1);
//     Serial0.write(0x00);
//     Serial0.write(ID2);
//     Serial0.write(ID3);
//     Serial0.write((data_num & 0xFF00) >> 8);
//     Serial0.write(data_num & 0x00FF);
//     // Gap = (int)(data_num*scale_x)
//     uchar temp_value = 0;
//     for(unsigned short i=0;i<data_num;i++){
//         // If Meets Gap
//         if (i%((int)(data_num*scale_x)) == 0){
//             Serial0.write((uchar)(data_buffer[i]*scale_y));
//             temp_value = (uchar)(data_buffer[i]*scale_y);
//         }
//         else{
//             Serial.write(temp_value);
//         }       
//     }
//     Serial0.write(0xFF);
//     Serial0.write(0xFC);
//     Serial0.write(0xFF);
//     Serial0.write(0xFF);
// }


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

void set_color(uchar foreground_color, uchar background_color){
    Serial0.write(0xEE);
    Serial0.write(0x42);
    Serial0.write(foreground_color);
    Serial0.write(0xFF);
    Serial0.write(0xFC);
    Serial0.write(0xFF);
    Serial0.write(0xFF);

    Serial0.write(0xEE);
    Serial0.write(0x42);
    Serial0.write(background_color);
    Serial0.write(0xFF);
    Serial0.write(0xFC);
    Serial0.write(0xFF);
    Serial0.write(0xFF);

}

void add_text(int x_position, int y_position, char * text){
    Serial0.write(0xEE);
    Serial0.write(0x20);
    unsigned short temp_x_position = x_position;
    Serial0.write((temp_x_position & 0xFF00) >> 8);
    Serial0.write(temp_x_position & 0x00FF);
    unsigned short temp_y_position = y_position;
    Serial0.write((temp_y_position & 0xFF00) >> 8);
    Serial0.write(temp_y_position & 0x00FF);
    Serial0.write(0x01);
    Serial0.write(0x09);    // Set Max Size
    Serial0.printf("%s",text);
    Serial0.write(0xFF);
    Serial0.write(0xFC);
    Serial0.write(0xFF);
    Serial0.write(0xFF);
}

void add_text_to_graph(int x,int y,int width,int height,int data_num, int data_index,uchar data_value,char* value, float scale_x, float scale_y,int move_x,int move_y){
    set_color(0x00,0xFF);
    Serial.printf("The Modify is : %f, %f, %d, %d\n", scale_x, scale_y, move_x,move_y);
    int temp_x = ((x+(width*scale_y)*((float)data_index/data_num))+(move_x));
    int temp_y = ((y+height*scale_y)*((float)data_value)/255)+(move_y);
    Serial.printf("The Text Is At %d, %d\n",temp_x, temp_y);
    add_text(temp_x,temp_y,value);
}


void setup(){
    Serial.begin(115200);
    Serial0.begin(115200,SERIAL_8N1,14,12);

    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
}

void loop(){


    digitalWrite(25,HIGH);
    digitalWrite(26,HIGH);

    // ---------- Receiving Data From The Screen -----------
    // * ---------------------------------------------------------
    // * ------------------ Capture The Frame --------------------
    // * ---------------------------------------------------------

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

            // * ------- Write The Received Command ------------ 
            // for (int j=0;j<cnt;j++){
            //     Serial.print(temp_frame[j],HEX);
            //     // Serial.print(",");
            //     // Serial.print(j,DEC);
            //     Serial.print(" ");
            // }
            // Serial.println(" ");

            uchar scene_id;
            uchar element_id;
            int where_dot = 0;
            String Data_RX;
            int Data_RX_int;
            float Data_RX_float = 0.0;

            // * ----------------------------------------------------------
            // * ------------Decode The Temp Frame (Input Word)------------
            // * ----------------------------------------------------------      
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
            // ------------Decode The Temp Frame (Input Slide)------------   
            if(temp_frame[1] == 0xB1 && temp_frame[2] == 0x11 && temp_frame[7] == 0x13){
                scene_id = temp_frame[4];
                element_id = temp_frame[6];
                uchar slider_value = temp_frame[11];
                Serial.printf("The Slider Value Of The %d, %d Is:%d\n",scene_id,element_id,slider_value);
                if(scene_id == 2 && element_id == 6){
                    slide_x_value = (float)(temp_frame[11])/100;
                    // Serial.printf("Modify The Graph To %f,%f\n",slide_x_value,slide_y_value);
                    modify_chart(0x02,0x01,slide_x_value,slide_y_value,move_x_value,0);
                    
                }
                else if (scene_id == 2 && element_id == 7){
                    slide_y_value = (float)(temp_frame[11])/100;
                    // Serial.printf("Modify The Graph To %f,%f\n",slide_x_value,slide_y_value);
                    modify_chart(0x02,0x01,slide_x_value,slide_y_value,move_x_value,0); 
                }
                else if(scene_id == 2 && element_id == 8){
                    move_x_value = temp_frame[11] - 50;
                    modify_chart(0x02,0x01,slide_x_value,slide_y_value,move_x_value,0);
                }
            }

            // ------------Decode The Temp Frame (Input Button)------------  
            if (temp_frame[1] == 0xB1 && temp_frame[2] == 0x11 && temp_frame[7] == 0x10 && temp_frame[9] == 0x01){
                scene_id = temp_frame[4];
                element_id = temp_frame[6];
                Serial.printf("The %dth button in %dth scene is pressed\n",scene_id,scene_id);
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
        if (temp == 0x05){
            set_color(0x00,0xCC);
            char text2send[4] = {'F','U','C','K'};
            add_text(100,100,text2send);
        }
        if (temp == 0x06){
            char data_string[10] = {};
            itoa(200, data_string, 10);
            change_scene(0x00);
            change_scene(0x02);
            add_text_to_graph(56,24,910,453,100,30,200,data_string,slide_x_value,slide_y_value,move_x_value,move_y_value);
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
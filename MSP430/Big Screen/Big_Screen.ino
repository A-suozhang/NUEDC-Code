#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <msp430.h>
#include <SoftwareSerial.h>
#define BUTTON    BIT5

unsigned char temp;
int i;

// ---- Hand Craft INTR ----------
unsigned long temp_millis0;
unsigned long temp_millis1;
unsigned long last_time0;
unsigned long last_time1;

unsigned char head = 0;
unsigned char cnt0 = 0;
unsigned char true_head = 0xEE;
unsigned char tails[4] = {0,0,0,0};
unsigned char true_tail[4] = {0xFF,0xFC,0xFF,0xFF};
uint8_t temp_frame[1024] = {0};


int cnt = 0;    // cnt is length + 1

// -------------------- Uart Stuff --------------------------
void uart_print(char * tx_data)                               // Define a function which accepts a character pointer to an array
{
    unsigned int i=0;
    while(tx_data[i])                                                   // Increment through array, look for null pointer (0) at end of string
    {
        while ((UCA3STATW & UCBUSY));                 // Wait if line TX/RX module is busy with data
        UCA3TXBUF = tx_data[i];                                // Send out element i of tx_data array on UART bus
        i++;                                                                   // Increment variable for array address
    }
}

void uart_write(unsigned char data){
    while (!(UCA3IFG & UCTXIFG));
    UCA3TXBUF = data;
}

unsigned char uart_read(){
    while (!(UCA3IFG & UCRXIFG));
    return UCA3RXBUF;
}

// ----------------------------
// Graph Params
float slide_x_value;
float slide_y_value;
int move_x_value = 0;
int move_y_value = 0;

void write_string(unsigned char id1,unsigned char id2,char* s){
    uart_write(0XEE);
    uart_write(0XB1);
    uart_write(0X10);
    uart_write((uint8_t)0x00);
    uart_write(id1);
    uart_write((uint8_t)0x00);
    uart_write(id2);
    uart_print(s);
    uart_write(0xFF);
    uart_write(0xFC);
    uart_write(0xFF);
    uart_write(0xFF);
}

void write_float(unsigned char id1,unsigned char id2, float f){
    uart_write(0XEE);
    uart_write(0XB1);
    uart_write(0X10);
    uart_write((uint8_t)0x00);
    uart_write(id1);
    uart_write((uint8_t)0x00);
    uart_write(id2);
    char temp_s[10];
    String temp_s1 = String(f);
    temp_s1.toCharArray(temp_s,10,0);
    // sprintf(temp_s,"%f",f);
    uart_print(temp_s);
    uart_write(0xFF);
    uart_write(0xFC);
    uart_write(0xFF);
    uart_write(0xFF);
}

void write_char(unsigned char id1,unsigned char id2, unsigned char c){
    uart_write(0XEE);
    uart_write(0XB1);
    uart_write(0X10);
    uart_write((uint8_t)0x00);
    uart_write(id1);
    uart_write((uint8_t)0x00);
    uart_write(id2);
    uart_write(c);
    uart_write(0xFF);
    uart_write(0xFC);
    uart_write(0xFF);
    uart_write(0xFF);
}


void change_scene(unsigned char id1){
    uart_write(0xEE);
    uart_write(0xB1);
    uart_write((uint8_t)0x00);
    uart_write((uint8_t)0x00);
    uart_write(id1);
    uart_write(0xFF);
    uart_write(0xFC);
    uart_write(0xFF);
    uart_write(0xFF);
}

void write_chart(unsigned char id1,unsigned char id2,unsigned char id3,unsigned short data_num,unsigned char data_buffer[]){
    uart_write(0XEE);
    uart_write(0XB1);
    uart_write(0X35);
    uart_write((uint8_t)0x00);
    uart_write(id1);
    uart_write((uint8_t)0x00);
    uart_write(id2);
    uart_write(id3);
    uart_write((data_num & 0xFF00) >> 8);
    uart_write(data_num & (uint8_t)0x00FF);
    for(unsigned short i=0;i<data_num;i++){
        uart_write(data_buffer[i]);
    }
    uart_write(0xFF);
    uart_write(0xFC);
    uart_write(0xFF);
    uart_write(0xFF);
}

void modify_chart(unsigned char id1, unsigned char id2, float scale_x, float scale_y,int move_x,int move_y){
    uart_write(0xEE);
    uart_write(0xB1);
    uart_write(0x34);
    uart_write((uint8_t)0x00);
    uart_write(id1);
    uart_write((uint8_t)0x00);
    uart_write(id2);

    signed short temp_move_x = (-1)*move_x;
    uart_write((temp_move_x & 0xFF00) >> 8);
    uart_write(temp_move_x & (uint8_t)0x00FF);

    unsigned short temp_scale_x = (unsigned short)(1000/(scale_x+0.1));  // Avoid / '0'
    if (temp_scale_x > 10000){
        temp_scale_x = 10000;
    }
    // Serial.printf("The Scale X is: %d",temp_scale_x);
    uart_write((temp_scale_x & 0xFF00) >> 8);
    uart_write(temp_scale_x & (uint8_t)0x00FF);


    signed short temp_move_y = move_y;
    uart_write((temp_move_y & 0xFF00) >> 8);
    uart_write(temp_move_y & (uint8_t)0x00FF);

    unsigned short temp_scale_y = (unsigned short)(100/(scale_y+0.1));
    if (temp_scale_y > 1000){
        temp_scale_y = 1000;
    }
    uart_write((temp_scale_y & 0xFF00) >> 8);
    uart_write(temp_scale_y & (uint8_t)0x00FF);


    uart_write(0xFF);
    uart_write(0xFC);
    uart_write(0xFF);
    uart_write(0xFF);

}



// Muda Muda
// void advanced_write_chart(unsigned char id1,unsigned char id2,unsigned char id3,unsigned short data_num,unsigned char data_buffer[],float scale_x=1, float scale_y=1,int move_x=0,int move_y=0 ){
//     uart_write(0XEE);
//     uart_write(0XB1);
//     uart_write(0X35);
//     uart_write((uint8_t)0x00);
//     uart_write(id1);
//     uart_write((uint8_t)0x00);
//     uart_write(id2);
//     uart_write(id3);
//     uart_write((data_num & 0xFF00) >> 8);
//     uart_write(data_num & (uint8_t)0x00FF);
//     // Gap = (int)(data_num*scale_x)
//     unsigned char temp_value = 0;
//     for(unsigned short i=0;i<data_num;i++){
//         // If Meets Gap
//         if (i%((int)(data_num*scale_x)) == 0){
//             uart_write((unsigned char)(data_buffer[i]*scale_y));
//             temp_value = (unsigned char)(data_buffer[i]*scale_y);
//         }
//         else{
//             Serial.write(temp_value);
//         }
//     }
//     uart_write(0xFF);
//     uart_write(0xFC);
//     uart_write(0xFF);
//     uart_write(0xFF);
// }


void empty_graph(unsigned char id1,unsigned char id2,unsigned char id3){
    uart_write(0xEE);
    uart_write(0xB1);
    uart_write(0x33);
    uart_write((uint8_t)0x00);
    uart_write(id1);
    uart_write((uint8_t)0x00);
    uart_write(id2);
    uart_write((uint8_t)0x00);
    uart_write(0xFF);
    uart_write(0xFC);
    uart_write(0xFF);
    uart_write(0xFF);
}

void set_color(unsigned char foreground_color, unsigned char background_color){
    uart_write(0xEE);
    uart_write(0x42);
    uart_write(foreground_color);
    uart_write(0xFF);
    uart_write(0xFC);
    uart_write(0xFF);
    uart_write(0xFF);

    uart_write(0xEE);
    uart_write(0x42);
    uart_write(background_color);
    uart_write(0xFF);
    uart_write(0xFC);
    uart_write(0xFF);
    uart_write(0xFF);

}

void add_text(int x_position, int y_position, char * text){
    uart_write(0xEE);
    uart_write(0x20);
    unsigned short temp_x_position = x_position;
    uart_write((temp_x_position & 0xFF00) >> 8);
    uart_write(temp_x_position & (uint8_t)0x00FF);
    unsigned short temp_y_position = y_position;
    uart_write((temp_y_position & 0xFF00) >> 8);
    uart_write(temp_y_position & (uint8_t)0x00FF);
    uart_write(0x01);
    uart_write(0x09);    // Set Max Size
    uart_print(text);
    uart_write(0xFF);
    uart_write(0xFC);
    uart_write(0xFF);
    uart_write(0xFF);
}

void add_text_to_graph(int x,int y,int width,int height,int data_num, int data_index,unsigned char data_value,char* value, float scale_x, float scale_y,int move_x,int move_y){
    set_color((uint8_t)0x00,0xFF);
    // Serial.printf("The Modify is : %f, %f, %d, %d\n", scale_x, scale_y, move_x,move_y);
    int temp_x = ((x+(width*scale_y)*((float)data_index/data_num))+(move_x));
    int temp_y = ((y+height*scale_y)*((float)data_value)/255)+(move_y);
    // Serial.printf("The Text Is At %d, %d\n",temp_x, temp_y);
    add_text(temp_x,temp_y,value);
}


void setup(){

      Serial.begin(115200);   // Dont Know Why Actually 4800

      // WDTCTL = WDTPW + WDTHOLD;         // Stop Watch dog timer


      // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
      PM5CTL0 &= ~LOCKLPM5;

      // Clock setup
      CSCTL0_H = CSKEY >> 8;                                                                      // Unlock CS registers
      CSCTL1 = DCOFSEL_3 | DCORSEL;                                                       //Set DCO to 8MHz
      CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;     // Set ACLK = VLOCLK; MCLK = SMCLK = DCO
      CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;                                              // Set all dividers to 1



      // Lock CS registers
      CSCTL0_H = 0;

      P5DIR &=~BUTTON;                          // Ensure button is input

      P1OUT &= ~BIT0;                               // Clear P1.0 output latch
      P1DIR |= BIT0;                                    // For LED

      P5OUT |=  BUTTON;                         // Enables pullup resistor on button
      P5REN |=  BUTTON;

      P6SEL1 &= ~(BIT0 | BIT1);
      P6SEL0 |= (BIT0 | BIT1);                     // USCI_A3 UART operation

      UCA3CTLW0 = UCSWRST;                             // Put eUSCI in reset
      UCA3CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
      // Baud Rate calculation
      // 8000000/(16*9600) = 52.083
      // Fractional portion = 0.083
      // User's Guide Table 21-4: UCBRSx = 0x04
      // UCBRFx = int ( (52.083-52)*16) = 1
      UCA3BRW = 52;                                                           // 8000000/16/9600
      UCA3MCTLW |= UCOS16 | UCBRF_1 | 0x0400;       //
      UCA3CTLW0 &= ~UCSWRST;                                  // Initialize eUSCI

}

void loop(){


    // ---------- Receiving Data From The Screen -----------
    // * ---------------------------------------------------------
    // * ------------------ Capture The Frame --------------------
    // * ---------------------------------------------------------

    if ((UCA3IFG & UCRXIFG)){
        head = uart_read();
            if (head == true_head){

                // Start Reading
                // First Read 4 Values
                temp_frame[1] = uart_read();
                temp_frame[2] = uart_read();
                temp_frame[3] = uart_read();
                temp_frame[4] = uart_read();
                temp_frame[0] = head;
                cnt = 5;    // The temp_frame Has 5 Values

                uint8_t a0 = temp_frame[0];
                uint8_t a1 = temp_frame[1];
                uint8_t a2 = temp_frame[2];
                uint8_t a3 = temp_frame[3];
                uint8_t a4 = temp_frame[4];

                while(!(temp_frame[cnt-1] == 0xFF && temp_frame[cnt-2] == 0xFF && temp_frame[cnt-3] == 0xFC && temp_frame[cnt-4] == 0xFF)){
                    // Detect The Tail, Only When Detecting Tail, Stop Reading
                    temp_frame[cnt] = uart_read();
                    cnt = cnt + 1;
                // Print The len
                }

                  // * ------- Write The Received Command ------------
//                 for (int j=0;j<cnt;j++){
////                     Serial.print(j);
////                     Serial.print(' ');
//                     Serial.print(temp_frame[j],HEX);
//                     // Serial.print(",");
//                     // Serial.print(j,DEC);
//                     Serial.println(" ");
//                 }
//                 Serial.println(" ");

                unsigned char scene_id;
                unsigned char element_id;
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
                        for (int i=8;i<cnt-4;i++){
                            if(temp_frame[i] == 46){
                                Data_RX += '.';
                            }
                            else{
                                Data_RX += temp_frame[i] - 48;
                            }
                        }
        //                    for (int i=8;i<where_dot;i++){
        //                        Data_RX_float += (temp_frame[i] - 48)*pow(10,where_dot-i-1);
        //                    }
        //                    for (int i=where_dot+1;i<cnt-4;i++){
        //                        if (temp_frame[i] > 0){         // Dirty IMplemention, Sometimes The Frame Captured has one more 0
        //                            Data_RX_float += (temp_frame[i] - 48)*pow(10,where_dot-i);
        //                        }
        //                    }
                        Data_RX_float = Data_RX.toFloat();
                        Serial.print("The Decoded Float Value: ");
                        Serial.println(Data_RX_float);
                    }
                  }

                // ------------Decode The Temp Frame (Input Slide)------------
                if(temp_frame[1] == 0xB1 && temp_frame[2] == 0x11 && temp_frame[7] == 0x13){
                    scene_id = temp_frame[4];
                    element_id = temp_frame[6];
                    unsigned char slider_value = temp_frame[11];
                    // Serial.printf("The Slider Value Of The %d, %d Is:%d\n",scene_id,element_id,slider_value);
                    Serial.print("The Slider Value:");
                    Serial.println(slider_value);
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
                if (temp_frame[1] == 0xB1 && temp_frame[2] == 0x11 && temp_frame[7] == 0x10 ){
                    scene_id = temp_frame[4];
                    element_id = temp_frame[6];
                    // Serial.printf("The %dth button in %dth scene is pressed\n",scene_id,scene_id);
                    if (temp_frame[9] == 0x01){
                        Serial.print("The Button Is Pressed:");
                        Serial.print(scene_id);
                        Serial.print(' ');
                        Serial.println(element_id);
                    }
                }


                    temp_frame[1024] = {0};
                    cnt = 0;
                    head = 0;

                    // while(uart_read()>0){};  !!! Fuckin' morone

                }

            }


    // -------------- The TX Part --------------------

    unsigned char scene_id;
    float temp_float;
    unsigned char data_buffer[1000];
    unsigned char data_buffer1[1000];
    if (Serial.available()){
        temp = Serial.read();
        if (temp == (uint8_t)0x00){
            // DO Change Scene
            while(!Serial.available()){};
            scene_id = Serial.read();
            change_scene(scene_id);
            while(Serial.read() > 0){};     // Empty BUFFER
        }
        if (temp == 0x01){
            // DO write_float
            // temp_float = Serial.parseFloat();
            write_float((uint8_t)0x00,0x04,12450.66);
            while(Serial.read() > 0){};     // Empty BUFFER
        }
        if (temp == 0x02){
            // Write Graph
            // The Input Format Is char
            // Should Quantize
            for (int k=0;k<50;k++){
                data_buffer[k] = (unsigned char)(int)(255*sin(2*PI*0.01*k));
            }
            write_chart(0x02,0x01,(uint8_t)0x00,(unsigned short)50,data_buffer);
            for (int k=0;k<50;k++){
               data_buffer[k] = 0;
            }
            write_chart(0x02,0x01,(uint8_t)0x00,(unsigned short)30,data_buffer);
        }
        if (temp == 0x03){
            empty_graph(0x02,0x01,(uint8_t)0x00);
        }
        if (temp == 0x04){
            cnt0 = cnt0+1;
            for (int k=0;k<50;k++){
                // data_buffer[k] = (unsigned char)(int)(127*sin(2*PI*0.1*(k+1+50*cnt0))+128);
                data_buffer[k] = cnt0;
            }
            write_chart(0x02,0x01,(uint8_t)0x00,(unsigned short)50,data_buffer);
        }
        if (temp == 0x05){
            set_color((uint8_t)0x00,0xCC);
            char text2send[4] = {'F','U','C','K'};
            add_text(100,100,text2send);
        }
        if (temp == 0x06){
            char data_string[10] = {};
            itoa(200, data_string, 10);
            change_scene((uint8_t)0x00);
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
    //         // data_buffer[k] = (unsigned char)(int)(127*sin(2*PI*0.1*(k+1+50*cnt0))+128);
    //         data_buffer[k] = (unsigned char)(int)(127*sin(2*PI*0.075*(k+cnt0))+128);
    //         data_buffer1[k] = (unsigned char)(int)(127*sin(2*PI*0.15*(k+cnt0))+128);
    //     }
    //     // empty_graph(0x02,0x01,(uint8_t)0x00);
    //     write_chart(0x02,0x01,(uint8_t)0x00,(unsigned short)100,data_buffer);
    //     write_char(0x02,0x04,cnt0);
    //     write_char(0x02,0x05,cnt0);
    //     write_chart(0x02,0x01,0x01,(unsigned short)100,data_buffer1);

    // }

    // *** Testing Serial
    // unsigned char val;
    // if (Serial0.available()){
    //     val = uart_read();
    //     Serial.print(val,HEX);
    // }




}

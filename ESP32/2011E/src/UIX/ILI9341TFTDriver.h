#ifndef __ILI9341TFTDRIVER_H__
#define __ILI9341TFTDRIVER_H__

#include "SPIWrapper.h"
#include "esp_heap_caps.h"
#include "esp_log.h"

class ILI9341{
    private:
    unsigned char dc_pin,rst_pin;
    unsigned char touch_cs_pin=4; //forced to be pin 4 now
    public:
    
    unsigned char* dispmem;
    unsigned char memorypages=4;
    unsigned char currentpage=0;
    DMASPI* spi;

    ILI9341(DMASPI* spi_,unsigned char dc_pin_,unsigned char rst_pin_,unsigned char memorypages_){
        spi=spi_;
        dc_pin=dc_pin_;rst_pin=rst_pin_;
        memorypages=memorypages_;
    }
    ~ILI9341(){
        free(dispmem);
    }

    void sendcommand(unsigned char cmd){
        digitalWrite(dc_pin,LOW);
        spi->transmit(cmd);
    }

    void senddata(unsigned char dat){
        digitalWrite(dc_pin,HIGH);
        spi->transmit(dat);
    }

    void reset(){
        digitalWrite(rst_pin,LOW);
        delay(200);
        digitalWrite(rst_pin,HIGH);
        delay(200);

        sendcommand(0x01);//soft rst
        delay(200);

        sendcommand(0xC0);
        senddata(0x23);

        sendcommand(0xC1);
        senddata(0x10);

        sendcommand(0xC5);
        senddata(0x3E);
        senddata(0x28);

        sendcommand(0xC7);
        senddata(0x86);

        sendcommand(0x36);
        senddata(0x48);

        sendcommand(0x3A);
        senddata(0x55);

        sendcommand(0xB1);
        senddata(0x00);
        senddata(0x18);

        //display function control
        sendcommand(0xB6);
        senddata(0x08);
        senddata(0x82);
        senddata(0x27);

        //exit sleep
        sendcommand(0x11);
        delay(200);
        //display on
        sendcommand(0x29);

        /*
        digitalWrite(dc_pin,LOW);   //sleep out
        spi->transmit(0x11);
        delay(200);

        digitalWrite(dc_pin,LOW);   //display on (unfreeze)
        spi->transmit(0x29);

        digitalWrite(dc_pin,LOW);   //enable brightness
        spi->transmit(0x53);
        digitalWrite(dc_pin,HIGH);
        spi->transmit(0x28);

        digitalWrite(dc_pin,LOW);   //change brightness
        spi->transmit(0x51);
        digitalWrite(dc_pin,HIGH);
        spi->transmit(0x7F);*/
        ESP_LOGI("Driver","Display Reset");
    }

    void begin(){
        dispmem=(unsigned char*)heap_caps_malloc(10+(320*240*2)/memorypages,MALLOC_CAP_DMA);
        pinMode(dc_pin,OUTPUT);
        pinMode(rst_pin,OUTPUT);
        pinMode(touch_cs_pin,OUTPUT);
        digitalWrite(rst_pin,HIGH);
        digitalWrite(touch_cs_pin,HIGH);
        reset();
    }

    void setdisplay(bool val){
        if(val){
            digitalWrite(dc_pin,LOW);   //display off
            spi->transmit(0x28);
        }else{
            digitalWrite(dc_pin,LOW);   //display on
            spi->transmit(0x29);
        }
    }

    void flushlastpage(){
        spi->flush();
    }

    void sendpage(unsigned short spoffset,unsigned short epoffset){
        //spi->transmit(0x00);    //to be removed
        //spi->flush();
        //ESP_LOGI(TAGDD,"SPI Flushed");
        digitalWrite(dc_pin,LOW);
        spi->transmit(0x2B);    //pas
        digitalWrite(dc_pin,HIGH);
        unsigned short pagesp=(unsigned long)((unsigned long)320/memorypages)*currentpage+spoffset;
        unsigned short pageep=(unsigned long)((unsigned long)320/memorypages)*currentpage+epoffset;
        unsigned char txbuf[4];
        txbuf[0]=pagesp>>8;txbuf[1]=pagesp&0xFF;txbuf[2]=pageep>>8;txbuf[3]=pageep&0xFF;
        spi->transmit(txbuf,4);
        //spi->flush();
        //delay(1000);

        //ESP_LOGI(TAGDD,"Begin Writing Page");
        digitalWrite(dc_pin,LOW);
        spi->transmit(0x2C);    //memory write

        digitalWrite(dc_pin,HIGH);
        spi->transmit(dispmem+240*2*spoffset,240*2*(epoffset-spoffset+1),currentpage!=(memorypages-1));

    }

    void sendpage(){
        sendpage(0,320/memorypages-1);
    }

    bool nextpage(){
        currentpage++;
        if(currentpage>=memorypages){
            currentpage=0;
            return false;
        }else{
            return true;
        }
    }

    void readtouchscreenraw(unsigned short *x,unsigned short *y,unsigned short *z1,unsigned short *z2){
        unsigned char txchars[4]={0xD7,0x97,0xB7,0xC7};
        unsigned char rawa[9],rawb[9];
        digitalWrite(touch_cs_pin,LOW);
        spi->transmitslowdev(txchars,rawa);
        digitalWrite(touch_cs_pin,HIGH);
        digitalWrite(touch_cs_pin,LOW);
        spi->transmitslowdev(txchars,rawb);
        digitalWrite(touch_cs_pin,HIGH);
        *x=(unsigned short)rawa[1]*16+rawa[2]/16;
        *x+=(unsigned short)rawb[1]*16+rawb[2]/16;
        *y=(unsigned short)rawa[3]*16+rawa[4]/16;
        *y+=(unsigned short)rawb[3]*16+rawb[4]/16;
        *z1=(unsigned short)rawa[5]*16+rawa[6]/16;
        *z1+=(unsigned short)rawb[5]*16+rawb[6]/16;
        *z2=(unsigned short)rawa[7]*16+rawa[8]/16;
        *z2+=(unsigned short)rawb[7]*16+rawb[8]/16;
    }

    void readtouchscreen(unsigned short *x,unsigned short *y,unsigned short *z,bool *pressed){
        unsigned short rx,ry,rz1,rz2;
        readtouchscreenraw(&rx,&ry,&rz1,&rz2);
        *z=rz1+(4096-rz2);
        *pressed=*z>800;
        unsigned long indx,indy;
        indx=(unsigned long)rx-300;
        indx=240*indx/3900;
        *x=(unsigned short)indx;
        indy=(unsigned long)ry-430;
        indy=320-(320*indy/3800);
        *y=(unsigned short)indy;
    }
};

#endif

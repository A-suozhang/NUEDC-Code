#ifndef __SPIWRAPPER_H__
#define __SPIWRAPPER_H__

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "SPIInterrupts.h"
#include "esp_log.h"

class DMASPI{
    private:
    unsigned char mosi_pin,miso_pin,sck_pin,cs0_pin,cs1_pin;
    int hosttyp;    //2 HSPI 3 VSPI
    unsigned char dmachan=1;
    spi_device_handle_t spidevhandle,spislowdevhandle;
    spi_transaction_t spitrans,spitransslowdev; //SLOW DEV IS CURRENTLY VERY SPECICALIZED TO TOUCH SCREEN CONTROLLER 
    public:
    DMASPI(int hosttyp_,unsigned char mosi_pin_,unsigned char miso_pin_,unsigned char sck_pin_,unsigned char cs0_pin){
        hosttyp=hosttyp_;mosi_pin=mosi_pin_;miso_pin=miso_pin_;sck_pin=sck_pin_;
    }
    DMASPI(int host){
        if(host==2){
            mosi_pin=13;miso_pin=12;sck_pin=14;cs0_pin=15,cs1_pin=4;
        }else if(host==3){
            mosi_pin=23;miso_pin=19;sck_pin=18;cs0_pin=5,cs1_pin=4;
        }
        hosttyp=host;
    }
    ~DMASPI(){
    }
    void begin(unsigned char mode_,unsigned long clockspd_){
        spitransmitting=false;
        spi_host_device_t spih=HSPI_HOST;
        if(hosttyp==2){
            spih=HSPI_HOST;
        }else if(hosttyp==3){
            spih=VSPI_HOST;
        }
        spi_bus_config_t spiconf;
        spiconf.miso_io_num=miso_pin;spiconf.mosi_io_num=mosi_pin;spiconf.sclk_io_num=sck_pin;
        spiconf.quadhd_io_num=-1;spiconf.quadwp_io_num=-1;
        spiconf.max_transfer_sz=512000;
        spi_bus_initialize(spih,&spiconf,dmachan);  //initialize bus
        spi_device_interface_config_t spidevconf;
        spidevconf.command_bits=0;
        spidevconf.address_bits=0;
        spidevconf.dummy_bits=0;
        spidevconf.mode=mode_;
        spidevconf.duty_cycle_pos=128;
        spidevconf.cs_ena_posttrans=0;
        spidevconf.cs_ena_pretrans=0;
        spidevconf.clock_speed_hz=clockspd_;
        spidevconf.spics_io_num=cs0_pin;
        spidevconf.queue_size=1;    //to be tested
        spidevconf.flags=SPI_DEVICE_HALFDUPLEX;
        spidevconf.pre_cb=(transaction_cb_t)spipretrans;
        spidevconf.post_cb=(transaction_cb_t)spiposttrans;
        spi_bus_add_device(spih,&spidevconf,&spidevhandle);
        spi_device_interface_config_t spislowdevconf;
        spislowdevconf.command_bits=0;
        spislowdevconf.address_bits=0;
        spislowdevconf.dummy_bits=0;
        spislowdevconf.mode=mode_;
        spislowdevconf.duty_cycle_pos=128;
        spislowdevconf.cs_ena_posttrans=0;
        spislowdevconf.cs_ena_pretrans=0;
        spislowdevconf.clock_speed_hz=2000000;
        spislowdevconf.spics_io_num=-1;
        spislowdevconf.queue_size=1;    //to be tested
        spislowdevconf.flags=SPI_DEVICE_NO_DUMMY;
        spislowdevconf.pre_cb=0;
        spislowdevconf.post_cb=0;
        spi_bus_add_device(spih,&spislowdevconf,&spislowdevhandle);
    }

    void transmitslowdev(unsigned char* txchar,unsigned char* rxchar){
        /*
        unsigned char txcharmem[9]={txchar[0],0x00,txchar[1],0x00,txchar[2],0x00,txchar[3],0x00,0x00};
        unsigned char rxcharmem[9];
        spitransslowdev.cmd=0;
        spitransslowdev.addr=0;
        spitransslowdev.flags=0;
        spitransslowdev.length=72;
        spitransslowdev.tx_buffer=txcharmem;
        spitransslowdev.rxlength=72;
        spitransslowdev.rx_buffer=rxchar;
        spi_device_transmit(spislowdevhandle,&spitransslowdev);
        ESP_LOGI("SPIW","touchraw: %d,%d,%d,%d,%d,%d,%d,%d,%d",rxchar[0],rxchar[1],rxchar[2],rxchar[3],rxchar[4],rxchar[5],rxchar[6],rxchar[7],rxchar[8]);
        unsigned short decode=(unsigned short)rxcharmem[1]*32+rxcharmem[2]/8;*/
        unsigned char txcharmem[9]={txchar[0],0x00,txchar[1],0x00,txchar[2],0x00,txchar[3],0x00,0x00};
        for(int i=0;i<9;i++){
            spitransslowdev.cmd=0;
            spitransslowdev.addr=0;
            spitransslowdev.flags=0;
            spitransslowdev.length=8;
            spitransslowdev.tx_buffer=txcharmem+i;
            spitransslowdev.rxlength=8;
            spitransslowdev.rx_buffer=rxchar+i;
            spi_device_transmit(spislowdevhandle,&spitransslowdev);
        }       //WORKAROUND!!!
        //ESP_LOGI("SPIW","touchraw: %d,%d,%d,%d,%d,%d,%d,%d,%d",rxchar[0],rxchar[1],rxchar[2],rxchar[3],rxchar[4],rxchar[5],rxchar[6],rxchar[7],rxchar[8]);
    }

    //len in bytes
    void transmit(unsigned char* txbuf,unsigned long len,bool blocking){
        spitrans.cmd=0;
        spitrans.addr=0;
        spitrans.flags=0;
        spitrans.length=8*len;
        spitrans.tx_buffer=(void *)txbuf;
        spitrans.rxlength=0;
        spitrans.rx_buffer=0;
        //spi_device_queue_trans(spidevhandle,&spitrans,portMAX_DELAY);
        //need to allocate a new instance of spitrans to non blocking
        spi_device_queue_trans(spidevhandle,&spitrans,portMAX_DELAY);
        //spi_device_transmit(spidevhandle,&spitrans);
        if(blocking) flush();
    }

    void transmit(unsigned char* txbuf,unsigned long len){
        transmit(txbuf,len,true);
    }

    void transmit(unsigned char txchar,bool blocking){
        unsigned char txchrmem=txchar;
        spitrans.cmd=0;
        spitrans.addr=0;
        spitrans.flags=0;
        spitrans.length=8;
        spitrans.tx_buffer=&txchrmem;
        spitrans.rxlength=0;
        spitrans.rx_buffer=0;
        spi_device_queue_trans(spidevhandle,&spitrans,portMAX_DELAY);
        if(blocking) flush();
    }

    void transmit(unsigned char txchar){
        transmit(txchar,true);
    }

    void flush(){
        //spi_transaction_t* spitransp=&spitrans;
        //spi_device_get_trans_result(spidevhandle,&spitransp,portMAX_DELAY);
        while(spitransmitting){
            ;
        }
        if(spifinishedtransmitting){
            spi_transaction_t* spitransp=&spitrans;
            spi_device_get_trans_result(spidevhandle,&spitransp,portMAX_DELAY);
        }
        spifinishedtransmitting=false;
    }
};


#endif

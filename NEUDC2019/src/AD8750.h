# define uchar unsigned char
# define CLOCKSPEED 100000
#include <SPI.h>

class AD8750
{
public:
    SPIClass * _hspi;
    int _cs, _sclk, _mosi;

    AD8750(int cs, int sclk, int mosi, SPIClass * hspi){
        _cs = cs;
        _sclk = sclk;
        _mosi = mosi;
        _hspi = hspi;
    }

    void AD8750_WriteByte(uchar data){
        (_hspi)->beginTransaction(SPISettings(CLOCKSPEED, MSBFIRST, SPI_MODE0));
        digitalWrite(_cs , LOW);    // 手动拉低CS
        (_hspi)->transfer(data);
        digitalWrite(_cs , HIGH);
        (_hspi)->endTransaction();
    }

    void begin(){
        pinMode(_cs,OUTPUT);
        pinMode(_sclk,OUTPUT);
        pinMode(_mosi,OUTPUT);

        AD8750_WriteByte(uint8_t(0x0F));
    }
};





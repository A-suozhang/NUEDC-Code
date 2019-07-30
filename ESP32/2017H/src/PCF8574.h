# include<Arduino.h>
# include<Wire.h>
# include"esp_log.h"

# define uchar unsigned char

const static char * tag = "PCF8574";

class PCF8574
{
public:
    uchar _temp_data;
    uchar _addr;        // The 3 bit ADDR:  A0 A1 A2

    PCF8574(uchar addr){
        _addr = addr;
        _temp_data = 0x00;
    }

    void begin(){
        _temp_data = 0xFF;
        // ESP_LOGI(tag, "Sucessfully Iniialized In\n");
    }
    
    void WriteByte(uchar data_in){
        _temp_data = data_in;
        Wire.beginTransmission(0x20+_addr); 
        // The IIC(Wire.h) Uses The 7BIT ADDR So The ADDR We Give beginTransmission & requestFrom should Be The Same And In 0~127
        Wire.write(data_in);
        // ESP_LOGI(tag, "Sucessfully Write In\n");
        Wire.endTransmission();
    }

    void digitalWrite(int i, bool LOGIC){
        if (LOGIC == true){
            _temp_data = _temp_data | (0x01 << i);
        }
        else {
            _temp_data = _temp_data & (~(0x01 << i));
        }
        // ESP_LOGI(tag, "Sucessfully Write %dTemp Data Is %c\n",i,_temp_data);
        WriteByte(_temp_data);
    }

    uchar ReadByte(){
        Wire.requestFrom(0x20+_addr,1);
        uchar data_out;
        while (Wire.available()){
            data_out = Wire.read();
        }
        // ESP_LOGI(tag, "Reading %c\n", data_out);
        _temp_data = data_out;
        return data_out;
    }

    bool digitalRead(uchar i){
        bool logic;
        uchar data_out,logic_char;
        data_out = ReadByte();
        // ESP_LOGI(tag, "The Current DATA is ;");
        // Serial.println(data_out,BIN);
        logic_char = data_out & (0x01 << i);
        if (logic_char == 0x00){
            // ESP_LOGI(tag, "Sucessfully Read Data Is %c\n",i,logic_char);
            // Serial.println("The Readed BIT is LOW");
            return false;
        }
        else{
            // ESP_LOGI(tag, "Sucessfully Read Data Is %c\n",i,logic_char);
            // Serial.println("The Readed BIT is HIGH");
            return true;
        }
    }

    uchar get_temp_data(){
        return _temp_data;
    }

    
};


# include "AD9854.h"

# define led 13

# define rst 22
# define io_update 24
# define wr 26
# define rd 28
# define osk 30 
# define fsk 32

int data[8] = {36,38,40,42,44,46,48,50};
int addr[6] = {31,33,35,37,39,41};

double freq = 10000000;
short amp = 0x00FFFFFF;

AD9854 DDS(rst, io_update, wr, rd, osk, fsk);

void setup()
{
    Serial.begin(115200);
    pinMode(13, OUTPUT);
    DDS.begin();
}

void loop(){
    digitalWrite(13, HIGH);
    Serial.println("Mega Working...");
    DDS.Set_Sine_Wave(freq, amp);
}
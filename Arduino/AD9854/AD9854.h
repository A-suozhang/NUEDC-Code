# define uchar unsigned char

// Mystery Bug With define uint32_t unsigned long 

// ------------- A FEW PARAMS ------------------
/* 
      Sys_clk 与 Ref_CLK之间的乘法因子 (外部时钟为20MHz晶振, 系统时钟最大为300MHz)
      freq_mult因子与频率计算有关 = 2^48 / Sys_CLK

*/
// ---------------------------------------------
/*
#define      clk_mult_index            4
const uint32_t  Freq_mult_uint32_t  = 3518437;
const double Freq_mult_doulle = 3518437.2088832;
*/

/*
#define      clk_mult_index            5
const uint32_t  Freq_mult_uint32_t  = 2814750;
const double Freq_mult_doulle = 2814749.76710656;
*/

 /*
#define      clk_mult_index            6
const uint32_t  Freq_mult_uint32_t  = 2345625;
const double Freq_mult_doulle = 2345624.80592213;
  */

/*
#define      clk_mult_index            7
const uint32_t  Freq_mult_uint32_t  = 2010536;
const double Freq_mult_doulle = 2010535.54793326;
*/

/*
#define      clk_mult_index            8
const uint32_t  Freq_mult_uint32_t  = 1759219;
const double Freq_mult_doulle = 1759218.6044416;
*/

/*
#define      clk_mult_index            9
const uint32_t  Freq_mult_uint32_t  = 1563750;
const double Freq_mult_doulle = 1563749.87061476;
*/

/*
#define      clk_mult_index            10
const uint32_t  Freq_mult_uint32_t  = 1407375;
const double Freq_mult_doulle = 1407374.88355328;
*/

/*
#define      clk_mult_index            11
const uint32_t  Freq_mult_uint32_t  = 1279432;
const double Freq_mult_doulle = 1279431.712321164;
*/


#define      clk_mult_index            12
const uint32_t  Freq_mult_uint32_t  = 1172812;
const double Freq_mult_doulle = 1172812.402961067;


/*
#define      clk_mult_index            13
const uint32_t  Freq_mult_uint32_t  = 1082596;
const double Freq_mult_doulle = 1082596.064271754;
*/

/*
#define      clk_mult_index            14
const uint32_t  Freq_mult_uint32_t  = 1005268;
const double Freq_mult_doulle = 1005267.773966629;
*/

/*
#define      clk_mult_index            15
const uint32_t  Freq_mult_uint32_t  = 938250;
const double Freq_mult_doulle = 938249.9223688533;

 */
// -------------------------------------------------------


class AD9854
{
    public:

    // PIN_OUTS
    int _rst, _io_update, _wr, _rd, _osk, _fsk;
    int _data[8] = {36,38,40,42,44,46,48,50};    // Deffault Pins
    int _addr[6] = {31,33,35,37,39,41};
    uchar FTW[6] = {0}; // Frequency Tunning Word

    // Build Function
    AD9854(int rst,int io_update,int wr,int rd,int osk,int fsk)
    {
        _rst = rst;
        _io_update = io_update;
        _wr = wr;
        _rd = rd;
        _osk = osk;
        _fsk = fsk;
    }


    // Initialization For AD9854
    void begin()
    {
        // Set io_update
        pinMode(_rst,OUTPUT);
        pinMode(_io_update, OUTPUT);
        pinMode(_wr, OUTPUT);
        pinMode(_rd, OUTPUT);
        pinMode(_osk, OUTPUT);
        pinMode(_fsk, OUTPUT);

        for(int i=0;i<8;i++){
            pinMode(_data[i],OUTPUT);
        }

        for(int i=0;i<6;i++){
            pinMode(_addr[i], OUTPUT);
        }

        // Set Deffault HIGH/LOW
        digitalWrite(_io_update, LOW);
        digitalWrite(_wr, HIGH);
        digitalWrite(_rd, HIGH);

        // Do The MASTER RST
        digitalWrite(_rst, HIGH);
        delay(1);
        digitalWrite(_rst, LOW);

        // Write Some Initial Value Into REGS
        digitalWrite(_wr, HIGH);
        digitalWrite(_rd, HIGH);
        digitalWrite(_io_update, LOW);
        // Held RST HIGH TO MAKE EVERYTHING DEFAULT
        digitalWrite(_rst, HIGH);
        delay(1);
        digitalWrite(_rst,LOW);

        //WRITE A FEW REGS
        Parallel_Write_Reg(0x1d,0x00);  // Close Comparator
        Parallel_Write_Reg(0x1e,clk_mult_index);    // Set Sys_clk
        Parallel_Write_Reg(0x1f,0x00);    // Syatem Mode 0
        Parallel_Write_Reg(0x20,0x60);    // Amp Modifiable, NO Compensation
        
        // Update REGS
        digitalWrite(_io_update, HIGH);
        digitalWrite(_io_update, LOW);  
    }

    void Parallel_Write_Reg(uchar addr, uchar data2write)
    {
        uchar temp_data;
        uchar temp_addr;

        // Prepare Data
        for (int i = 0;i<8;i++){
            temp_data = (data2write >> i) & 0x01;
            if (temp_data) {
                digitalWrite(_data[i], HIGH);
            }
            else {
                digitalWrite(_data[i],LOW);
            }
        }

        // Prepare addr
        for (int i = 0;i<6;i++){
            temp_addr = (addr >> i) & 0x01;
            if (temp_addr) {
                digitalWrite(_data[i], HIGH);
            }
            else {
                digitalWrite(_data[i],LOW);
            }
        }

        // Data Prepaerd, WR_EN Pulse
        digitalWrite(_wr,LOW);
        delay(1);
        digitalWrite(_wr,HIGH);
    }

    void Set_Sine_Wave(double freq,  uint16_t amp)
    {
        uint32_t freq_upper, freq_lower;
        // Convert Frequency 
        freq = freq*Freq_mult_doulle;   // double   FTW = OUTPUT_FREQ*(SYS_CLK)/(2^32)
        freq_upper = uint32_t(freq/4294967295); // 2^32 = 4294967295
        freq_lower = uint32_t(freq - (double)freq_upper*4294967295);

        // Fill FTW
        FTW[0] = (uchar)freq_lower;
        FTW[1] = (uchar)freq_lower >> 8;
        FTW[2] = (uchar)freq_lower >> 16;
        FTW[3] = (uchar)freq_lower >> 24;
        FTW[4] = (uchar)freq_upper;
        FTW[5] = (uchar)freq_upper >> 8;


        // Writing FTW, Set Freq
        for (int i = 0;i<6;i++){
            Parallel_Write_Reg(0x04+i, FTW[i]);
        }

        // Set I,Q Dual Channel Amp
        Parallel_Write_Reg(0x21, (uchar)((amp & 0x0F00) >> 8));   // Take amp[11:8]
        Parallel_Write_Reg(0x22, (uchar)(amp));
        Parallel_Write_Reg(0x23, (uchar)((amp & 0x0F00) >> 8));   // Take amp[11:8]
        Parallel_Write_Reg(0x24, (uchar)(amp));

        // Update REGS
        digitalWrite(_io_update, HIGH);
        digitalWrite(_io_update, LOW);

    }

};
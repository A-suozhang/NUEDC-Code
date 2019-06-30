/**********************************************************
                       ��������
										 
���ܣ�stm32f103rct6����
			��ʾ��12864
�ӿڣ������ӿ������key.h
ʱ�䣺2015/11/3
�汾��1.0
���ߣ���������
����������������ԭ�ӣ������Լ���������δ���������ɣ��������������κ���;

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/

#include "AD9910.h"
#include "delay.h"
#include "sys.h"
 /*-----------------------------------------------
  ���ƣ�AD9910��������
  ��д��Liu
  ���ڣ�2014.6
  �޸ģ���
  ���ݣ�
------------------------------------------------*/
const uchar cfr1[]={0x00,0x40,0x00,0x00};       //cfr1������
const uchar cfr2[]={0x01,0x00,0x00,0x00};       //cfr2������
const uchar cfr3[]={0x05,0x0F,0x41,0x50};       //cfr3������  40M����  25��Ƶ  VC0=101   ICP=001;
uchar profile11[]={0x3f,0xff,0x00,0x00,0x25,0x09,0x7b,0x42};       //profile1������ 0x25,0x09,0x7b,0x42
                  //01������� 23��λ���� 4567Ƶ�ʵ�г��
void AD9110_IOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOE,ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_Init(GPIOD,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7;
    GPIO_Init(GPIOG,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_Init(GPIOE,&GPIO_InitStructure); 
}

//=====================================================================


//======================����8λ���ݳ���===================================
void txd_8bit(uchar txdat)
{
    uchar i,sbt;
    sbt=0x80;
    SCLK=0;
    for (i=0;i<8;i++)
    {
        if ((txdat & sbt)==0) SDO=0; else SDO=1;
        SCLK=1;
        sbt=sbt>>1;
        SCLK=0;
    }
}    

//======================ad9910��ʼ������================================
void Init_ad9910(void)
{
	uchar k,m;
	
	AD9110_IOInit();//IO��ʼ��
	AD9910_PWR = 0;//��������
	
    PROFILE2=PROFILE1=PROFILE0=0;
    DRCTL=0;DRHOLD=0;
    MAS_REST=1; delay_ms(5);  MAS_REST=0; 
	
    CS=0;
    txd_8bit(0x00);    //����CFR1�����ֵ�ַ
    for (m=0;m<4;m++)
        txd_8bit(cfr1[m]); 
    CS=1;  
    for (k=0;k<10;k++);

    CS=0;
    txd_8bit(0x01);    //����CFR2�����ֵ�ַ
    for (m=0;m<4;m++)
        txd_8bit(cfr2[m]); 
    CS=1;  
    for (k=0;k<10;k++);

    CS=0;
    txd_8bit(0x02);    //����CFR3�����ֵ�ַ
    for (m=0;m<4;m++)
        txd_8bit(cfr3[m]); 
    CS=1;
    for (k=0;k<10;k++);

    UP_DAT=1;
    for(k=0;k<10;k++);
    UP_DAT=0;
    delay_ms(1);

}      
//=====================================================================

//======================ad9910����Ƶ��ֵ����===================================
void Txfrc(void)
{
    uchar m,k;


// Profile 8x Byte Transmit
    CS=0;
    txd_8bit(0x0e);    //����profile1�����ֵ�ַ
    for (m=0;m<8;m++)
        txd_8bit(profile11[m]); 
    CS=1;
    for(k=0;k<10;k++);

// IO_UPdate UPdate
    UP_DAT=1;
    for(k=0;k<10;k++);
    UP_DAT=0;
    delay_ms(1);
    
}         
//=====================================================================
//===================����Ƶƫ�֡�Ƶ���ֺͷ��ͳ���======================
void Write_F(double Freq)
{
    ulong Temp; 
	if(Freq > 400000000)
		Freq = 400000000;    
    Temp=Freq*4.294967296;	   //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�  4.294967296=(2^32)/1000000000
    profile11[7]=(uchar)Temp;
    profile11[6]=(uchar)(Temp>>8);
    profile11[5]=(uchar)(Temp>>16);
    profile11[4]=(uchar)(Temp>>24);
    Txfrc();
}
//=====================================================================

//======================ad9910����DRG��������==========================
void Write_A(double Amp)
{
	ulong Temp;
	Temp = (ulong)Amp*25.20615385;	   //������������ӷ�Ϊ�����ֽ�  25.20615385=(2^14)/650
	if(Temp > 0x3fff)
		Temp = 0x3fff;
	Temp &= 0x3fff;
	profile11[1]=(uchar)Temp;
	profile11[0]=(uchar)(Temp>>8);
	Txfrc();
}
//=====================================================================

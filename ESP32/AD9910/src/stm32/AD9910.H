 /*-----------------------------------------------
  名称：AD9854并口驱动
  编写：Liu
  日期：2014.3
  修改：无
  内容：
------------------------------------------------*/
#ifndef __AD9910_H__
#define __AD9910_H__

#include "sys.h"

#define uchar unsigned char
#define uint  unsigned int	
#define ulong  unsigned long int						
/*
sbit MASTER_RESET= P1^3;	//
sbit IORESET  = P2^6;	  //
sbit SCLK = P2^4;	  //
sbit SDIO    = P2^2;	 //
sbit OSK     = P2^0;	//
sbit IOUPDATA   = P0^4;	 //
sbit PROFILE2  = P0^2;	 //
sbit PROFILE1    = P0^0;	//
sbit PROFILE0 = P1^1;	 //
*/

//新增IO
//#define SDO PAout(4)
//#define PDCLK PCout(6)//PDCLK
//#define PLL_LOCK PCout(7)//悬空
//#define SYNC_CLK PCout(9)//悬空
//#define TXENABLE PCout(11)//TXEN
//#define RSO PCout(12)//RAM-SWP-OVR
#define AD9910_PWR PBout(12)// 
//原始IO
#define SDO PDout(12)       //SDIO  
#define DRHOLD PDout(15)    //DRH
#define DROVER PDout(13)    //DRO
#define UP_DAT PDout(11)    //IO_UP
#define PROFILE1 PBout(14)  //PROF1
#define MAS_REST PBout(13)  //RST
#define SCLK PDout(14)      //SCLK
#define DRCTL  PDout(10)    //DRC
#define OSK PDout(8)        //OSK
#define PROFILE0 PDout(9)   //PRFO
#define PROFILE2 PBout(15)  //PROF2
#define CS  PGout(2)//      //CSB

#define F0 PGout(7)
#define F1 PGout(4)
#define TEN PGout(3)

void AD9110_IOInit(void);
void Init_ad9910(void);
void Write_F(double Freq);  //写频率
void Write_A(double Amp); //写幅度
#endif



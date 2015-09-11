//CX II AFE functions//
#include "p32mx795f512l.h"
#include "stdio.h"
#include "Port_init.h"
#include "plib.h"
#include "Glob_Var_Dec.h"
#include "math.h"


//*********************Function declarations***************//
void initAFE1(void);

void initDAQ(void);
void pauseDAQ(void);

void AFE_DataRead_EMG (void);
void Analog_DataRead_EMG_Test (void);

void ReadAnalogInput(unsigned char);

//**********************Initialize AFE1********************************//
void initAFE1(void)
{
//CS3=1;
//
// delay(500000);
// 
// AFE_RESET=1;
// CS3=1;
// delay(500000);
// AFE_RESET=0;
// delay(2000);
// AFE_RESET=1;
// delay(2000);
//
// CS3=0;
// CMD_stop_read
// CS3=1;
// delay(2000);
//
// CS3=0;
// SPI3Rx_Buf=writeSPI3(config1_addr); // WREG start at 0x01 
// SPI3Rx_Buf=writeSPI3(0); // number is 1
// SPI3Rx_Buf=writeSPI3(config1_data); // set the CONFIG1 value sample rate=500 SPS
// delay(2000);
//
// SPI3Rx_Buf=writeSPI3(config2_addr); // WREG start at 0x02 
// SPI3Rx_Buf=writeSPI3(0); // number is 1
// SPI3Rx_Buf=writeSPI3(config2_data); //
// delay(2000);
//
// SPI3Rx_Buf=writeSPI3(config3_addr); // WREG start at 0x03 
// SPI3Rx_Buf=writeSPI3(0); // number is 1
// SPI3Rx_Buf=writeSPI3(config3_data); // 
// delay(2000);
// CS3=1;
// delay(2000);
//
// CS3=0;
// SPI3Rx_Buf=writeSPI3(CH1SET_addr); // WREG start at 0x05 
// SPI3Rx_Buf=writeSPI3(7); // number is 8
// SPI3Rx_Buf=writeSPI3(CH1SET); // set the CHnSET  value 12 gain
// SPI3Rx_Buf=writeSPI3(CH2SET); // set the CHnSET  value
// SPI3Rx_Buf=writeSPI3(CH3SET); // set the CHnSET  value
// SPI3Rx_Buf=writeSPI3(CH4SET); // set the CHnSET  value
// SPI3Rx_Buf=writeSPI3(CH5SET); // set the CHnSET  value
// SPI3Rx_Buf=writeSPI3(CH6SET); // set the CHnSET  value
// SPI3Rx_Buf=writeSPI3(CH7SET); // set the CHnSET  value
// SPI3Rx_Buf=writeSPI3(CH8SET); // set the CHnSET  value
// delay(2000);
// CS3=1;
// delay(2000);
//
////********************************************************
// CS3=0;
// SPI3Rx_Buf=writeSPI3(RLD_SENSP_addr); // WREG start at 
// SPI3Rx_Buf=writeSPI3(1); // number is 2
// SPI3Rx_Buf=writeSPI3(RLD_SENSP); // 
// SPI3Rx_Buf=writeSPI3(RLD_SENSN); // 
// delay(2000);
// CS3=1;
// delay(2000);
//
// CS3=0;
// SPI3Rx_Buf=writeSPI3(config4_addr); // RREG start at 0x17 CONFIG4
// SPI3Rx_Buf=writeSPI3(0); //  number is 1
// SPI3Rx_Buf=writeSPI3(config4_data); // WREG start at 0x17 CONFIG4
// delay(2000);
// CS3=1;
// delay(2000);
//
// CS3=0;
// SPI3Rx_Buf=writeSPI3(WCT1_addr ); // send the command "WREG WCT1"
// SPI3Rx_Buf=writeSPI3(0); // 
// SPI3Rx_Buf=writeSPI3(WCT1_data); // set the WCT1 value (enable WCTA, channel2 neg to WCTA, disable aVx channels)
// delay(2000);
// CS3=1;
// delay(2000);
//
// CS3=0;
// SPI3Rx_Buf=writeSPI3(WCT2_addr ); // send the command "WREG WCT2"
// SPI3Rx_Buf=writeSPI3(0); // 
// SPI3Rx_Buf=writeSPI3(WCT2_data); // set the WCT2 value (enable WCTB, WCTC, channel2 pos to WCTB, channel3 pos to WCTC)
// delay(2000);
// CS3=1;
// delay(2000);
//
// CS3=0;
// SPI3Rx_Buf=writeSPI3(LOFF_addr ); // send the command 
// SPI3Rx_Buf=writeSPI3(0); // 
// SPI3Rx_Buf=writeSPI3(LOFF); // 
// delay(2000);
// CS3=1;
// delay(2000);
//
// CS3=0;
// SPI3Rx_Buf=writeSPI3(LOFF_SENSP_addr ); // send the command
// SPI3Rx_Buf=writeSPI3(0); // 
// SPI3Rx_Buf=writeSPI3(LOFF_SENSP); // 
// delay(2000);
// CS3=1;
// delay(2000);
//
// CS3=0;
// SPI3Rx_Buf=writeSPI3(LOFF_SENSN_addr ); // send the command 
// SPI3Rx_Buf=writeSPI3(0); // 
// SPI3Rx_Buf=writeSPI3(LOFF_SENSN); // 
// delay(2000);
// CS3=1;
// delay(2000);
//
}



//*********************************init the data collection********************//
void initDAQ(void)
{
 //initAFE1();
 //initAFE2();
 initT1(32);  //Start T1 (T1 interrupt) data collection
 IFS0bits.T3IF=0;
 IFS0bits.T2IF=0;
 IFS0bits.T1IF=0;
}

//**********************************pause the data collection************//
void pauseDAQ(void)
{
 T1CONbits.ON=0;     //stop T1
 IFS0bits.T1IF=0;
 IEC0bits.T1IE=0;
}


//********************************************************//
void AFE_DataRead_EMG (void)
{
 unsigned char i;
 CMD_read_data  // send the command "RDATA"
 AFE_STATUS[0]=readSPI3();
 AFE_STATUS[1]=readSPI3();
 AFE_STATUS[2]=readSPI3();

 initSPI3_Master_EB();  //SPI3 enhaced buffer mode


 while (SPI3STATbits.TXBUFELM!=0);  //for any reason the SPI3TXBUF is not empty, wait until it is empty
 if (SPI3STATbits.RXBUFELM!=0);   // for any reason the SPI3RFBUF is not empty, then empty it
   {
    for (i=0;i<SPI3STATbits.RXBUFELM; i++)
       {
        TempBuf=SPI3BUF;
       }
   }

 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;

 while (SPI3STATbits.RXBUFELM==0);  //1.1
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //1.2
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //1.3
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //2.1
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //2.2
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //2.3
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //3.1
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //3.2
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;

 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;
 SPI3BUF=0;

 while (SPI3STATbits.RXBUFELM==0);  //3.3
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //4.1
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //4.2
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //4.3
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //5.1
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //5.2
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //5.3
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //6.1
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //6.2
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //6.3
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //7.1
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //7.2
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //7.3
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //8.1
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //8.2
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;
 while (SPI3STATbits.RXBUFELM==0);  //8.3
 *BufPointer_W = SPI3BUF;
 BufPointer_W++;

 initSPI3_Master_EB_Disable(); 
}


//********************************************************//
void Analog_DataRead_EMG_Test (void)
{
 unsigned char i=0;
 unsigned char j=0;
 unsigned int k=0;

 for (i=0;i<8;i++)
 { 
	DATA_BUF_16b[i][Buf_16b_Index] = AN_DATABUF[i];
 }

 unsigned int Data_temp=0;
 unsigned char Data_L=0;
 unsigned char Data_H=0;

 for (i=0;i<8;i++)
 {
	Data_temp=(DATA_BUF_16b[i][Buf_16b_Index]); //+0x8000); //-(HPF_Buf_H[i]>>7);
	Data_L=Data_temp;
	Data_H=Data_temp>>8;

	Data_Buf[i][Feat_Buf_Index] = Data_temp; //filling the waveform length buffer    

//  if (Data_temp<0x8000)						//rectifies the EMG signal
//  {Data_temp= (-1*(Data_temp-0x8000));}
//  else
//  {Data_temp = Data_temp - 0x8000;}

	Data_Rect_Buf[i][Feat_Buf_Index] = Data_temp; //filling the average buffer

  if (Feat_Buf_Index>=MovingBufLength)
  {
  	Avg_Buf[i] = Avg_Buf[i] + Data_Rect_Buf[i][Feat_Buf_Index] - Data_Rect_Buf[i][Feat_Buf_Index-MovingBufLength]; //sum for average
  }
  else if (Feat_Buf_Index<MovingBufLength)
  {
  	Avg_Buf[i] = Avg_Buf[i] + Data_Rect_Buf[i][Feat_Buf_Index] - Data_Rect_Buf[i][FeatureLength-(MovingBufLength-Feat_Buf_Index)]; //sum for average
  }

 }

Buf_16b_Index++;
if(Buf_16b_Index>FilterLength)
Buf_16b_Index=0;

Buf_16b_Index_Last=Buf_16b_Index+1;
if(Buf_16b_Index_Last>FilterLength)
Buf_16b_Index_Last=0;

Feat_Buf_Index++;
if(Feat_Buf_Index>=FeatureLength)
Feat_Buf_Index=0;
}


////*****************Analog input****************************/////

void ReadAnalogInput(unsigned char channel_map)
{
 unsigned char i=0;  

 initADC();	
 for(;i<8;i++)
 {
  if(channel_map & 0x01)
  {
   AN_DATABUF[i]=(readADC(i+6) << 1); //AN start with channel AN6, DEBUG bitshift left to get 3FF because of voltage divider
  //delay(d_50us);
putnumUART5(AN_DATABUF[0]);
   }
  channel_map= channel_map>>1;
 }

 disableADC();  
}
//CX II general I/O functions//
#include "p32mx795f512l.h"
#include "stdio.h"
#include "Port_init.h"
#include "plib.h"
#include "Glob_Var_Dec.h"



//*********************Function declerations***************//
void delay(unsigned int);

void writeSPI1 (unsigned char);
unsigned char readSPI1 (void);

void writeSPI3 (unsigned char);
unsigned char readSPI3 (void);

void putcharUART2(unsigned char);
void putstringUART2(unsigned char *);

void putcharUART3(unsigned char);
void putstringUART3(unsigned char *);
void puthexUART3(unsigned char);
void puthexUART3_short(unsigned short);
void putnumUART3(unsigned char);
//char ReadUART3(void);
//void WriteUART3(char);
//char DataRdyUART3(void);
//char BusyUART3(void);

void putcharUART5(unsigned char);
void putstringUART5(unsigned char *);
void putarrayUART5(unsigned char *, unsigned char *);
void UART5_Put_Inf(char *inf, unsigned short int dat);
void puthexUART5(unsigned char);
void puthexUART5_short(unsigned short);
void puthexUART5_int(unsigned int);
void putnumUART5(unsigned char);
void putnumUART5_short(unsigned short);
void putnumUART5_int(unsigned int);

void MCU_Sleep(void);
void MCU_Idle(void);

unsigned char start_I2C2(void);
unsigned char restart_I2C2(void);
unsigned char stop_I2C2(void);
unsigned char ack_I2C2(void);
unsigned char nack_I2C2(void);
unsigned char putchar_I2C2(unsigned char);
unsigned char getchar_I2C2(void);

unsigned short readADC(unsigned char);

//*****************************************************//
//based on the test, for main clock=18MHz, 1 second = 0x001BA308
//void delay(unsigned int cy)
//{
// while(--cy != 0);
//}


//**********************SPI1********************// 
void writeSPI1 (unsigned char data)  
{
 SPI1BUF=data;
 while(!SPI1STATbits.SPIRBF);
 unsigned char d=SPI1BUF;
}
//**********************SPI1***************************// 
unsigned char readSPI1 (void) 
{
 SPI1BUF=0;
 while(!SPI1STATbits.SPIRBF);
 return SPI1BUF;
}


//**********************SPI3***************************// 
void writeSPI3 (unsigned char data)     //AFE
{
 SPI3BUF=data;
 while(!SPI3STATbits.SPIRBF);
 unsigned char d=SPI3BUF;
}
//***********************SPI3**************************// 
unsigned char readSPI3 (void)           //AFE
{
 SPI3BUF=0;
 while(!SPI3STATbits.SPIRBF);
 return SPI3BUF;
}

//*************************************************// 
char writeSPI2 (char data)
{
 SPI2BUF=data;
 while(!SPI2STATbits.SPIRBF);
 return SPI2BUF;
}
//*************************************************// 
char readSPI2 (void)
{
 SPI2BUF=0;
 while(!SPI2STATbits.SPIRBF);
 return SPI2BUF;
}



//*************************************************//
void putcharUART2(unsigned char character)
{
        while(!UARTTransmitterIsReady(UART2));
        UARTSendDataByte(UART2, character);
        while(!UARTTransmissionHasCompleted(UART2));
}


//****************************************************//
void putstringUART2(unsigned char *string)
{
    while(*string != '\0')
    {
        while(U2STAbits.UTXBF);
        UARTSendDataByte(UART2, *string);
        string++;
    }
}

//*************************************************//
void putcharUART3(unsigned char character)
{
        while(!UARTTransmitterIsReady(UART3));
        UARTSendDataByte(UART3, character);
        while(!UARTTransmissionHasCompleted(UART3));
}

//****************************************************//
void putstringUART3(unsigned char *string)
{
    while(*string != '\0')
    {
        while(!UARTTransmitterIsReady(UART3));
        UARTSendDataByte(UART3, *string);
        string++;
        while(!UARTTransmissionHasCompleted(UART3));
    }
}

//*******************************************//
void puthexUART3(unsigned char num)
{
	if(num<0xa0)
		putcharUART3((num>>4)+0x30);
	else
		putcharUART3((num>>4)+55);
    num = num & 0x0F;  //lower four bits
	if(num<0xa)
		putcharUART3(num+0x30);
	else
		putcharUART3(num+55);	
    putcharUART3(' ');	
}

//*******************************************//
void puthexUART3_short(unsigned short num)
{
    unsigned char bs0=num & 0xF;  //bit 0~3
    num=num>>4;
    unsigned char bs1=num & 0xF;  //bit 4~7
    num=num>>4;
    unsigned char bs2=num & 0xF;  //bit 8~11
    num=num>>4;                   //bit 12~15
 
	if(num<0xA)
		putcharUART3(num+0x30);
	else
		putcharUART3(num+55);
	if(bs2<0xA)
		putcharUART3(bs2+0x30);
	else
		putcharUART3(bs2+55);
	if(bs1<0xA)
		putcharUART3(bs1+0x30);
	else
		putcharUART3(bs1+55);
	if(bs0<0xA)
		putcharUART3(bs0+0x30);
	else
		putcharUART3(bs0+55);
    putcharUART3(' ');
}

/******************************************************************/
void putnumUART3(unsigned char num)
{
 #ifdef Debug_Mode      //the device is in debug mode
  unsigned char i=0;

  while(num>(DtoH_10E2-1))  //10^2
  {
   num=num-DtoH_10E2;
   i++;
  }
  putcharUART3(i+0x30);

  i=0;
  while(num>(DtoH_10E1-1))  //10^1
  {
   num=num-DtoH_10E1;
   i++;
  }
  putcharUART3(i+0x30);

  putcharUART3(num+0x30);

  putcharUART3(' ');
 #endif
}
//*************************************************//

//void WriteUART3(char dataByte)
//{
//	UARTSendDataByte(UART3,dataByte);
//}
//
////*************************************************//
//char ReadUART3(void)
//{
//	return UARTGetDataByte(UART3);
//}
//
////*************************************************//
//char DataRdyUART3(void)
//{
//	return U3STAbits.URXDA;
//}
//
////*************************************************//
//char BusyUART3(void)
//{
//	return !UARTTransmissionHasCompleted(UART3);
//}
//
//*************************************************//
void putcharUART5(unsigned char character)
{
 #ifdef Debug_Mode      //the device is in debug mode
        while(U5STAbits.UTXBF);
        UARTSendDataByte(UART5, character);
 #endif
}
//****************************************************//

void putstringUART5(unsigned char *string)
{
 #ifdef Debug_Mode      //the device is in debug mode
    while(*string != '\0')
    {
        while(U5STAbits.UTXBF);
        UARTSendDataByte(UART5, *string);
        string++;
    }
 #endif
}

//****************************************************//

void putarrayUART5(unsigned char *arrayStart, unsigned char *arrayEnd)
{
 #ifdef Debug_Mode      //the device is in debug mode
    while(arrayStart<=arrayEnd)
    {
        while(U5STAbits.UTXBF);
        UARTSendDataByte(UART5, *arrayStart);
        arrayStart++;
    }
 #endif
}
/*******************************************************************/

void UART5_Put_Inf(char *inf, unsigned short int dat)
{ 
 #ifdef Debug_Mode      //the device is in debug mode
 char temp[20];
 putstringUART5(inf);
 //u32tostr(dat,temp);
 putstringUART5(temp);
 putstringUART5("\r\n");  
 #endif
}

//*******************************************//
void puthexUART5(unsigned char num)
{
 #ifdef Debug_Mode      //the device is in debug mode
	if(num<0xa0)
		putcharUART5((num>>4)+0x30);
	else
		putcharUART5((num>>4)+55);
    num = num & 0x0F;  //lower four bits
	if(num<0xa)
		putcharUART5(num+0x30);
	else
		putcharUART5(num+55);	
    putcharUART5(' ');	
 #endif
}

//*******************************************//
void puthexUART5_short(unsigned short num)
{
 #ifdef Debug_Mode      //the device is in debug mode
    unsigned char bs0=num & 0xF;  //bit 0~3
    num=num>>4;
    unsigned char bs1=num & 0xF;  //bit 4~7
    num=num>>4;
    unsigned char bs2=num & 0xF;  //bit 8~11
    num=num>>4;                   //bit 12~15
 
	if(num<0xA)
		putcharUART5(num+0x30);
	else
		putcharUART5(num+55);
	if(bs2<0xA)
		putcharUART5(bs2+0x30);
	else
		putcharUART5(bs2+55);
	if(bs1<0xA)
		putcharUART5(bs1+0x30);
	else
		putcharUART5(bs1+55);
	if(bs0<0xA)
		putcharUART5(bs0+0x30);
	else
		putcharUART5(bs0+55);
    putcharUART5(' ');
 #endif
}

//*******************************************//
void puthexUART5_int(unsigned int num)
{
 #ifdef Debug_Mode      //the device is in debug mode
    unsigned char bs0=num & 0xF;  //bit 0~3
    num=num>>4;
    unsigned char bs1=num & 0xF;  //bit 4~7
    num=num>>4;
    unsigned char bs2=num & 0xF;  //bit 8~11
    num=num>>4;
    unsigned char bs3=num & 0xF;  //bit 12~15
    num=num>>4;
    unsigned char bs4=num & 0xF;  //bit 16~19
    num=num>>4;
    unsigned char bs5=num & 0xF;  //bit 20~23
    num=num>>4;
    unsigned char bs6=num & 0xF;  //bit 24~27
    num=num>>4;                   //bit 28~31
 
	if(num<0xA)
		putcharUART5(num+0x30);
	else
		putcharUART5(num+55);
	if(bs6<0xA)
		putcharUART5(bs6+0x30);
	else
		putcharUART5(bs6+55);
	if(bs5<0xA)
		putcharUART5(bs5+0x30);
	else
		putcharUART5(bs5+55);
	if(bs4<0xA)
		putcharUART5(bs4+0x30);
	else
		putcharUART5(bs4+55);
	if(bs3<0xA)
		putcharUART5(bs3+0x30);
	else
		putcharUART5(bs3+55);
	if(bs2<0xA)
		putcharUART5(bs2+0x30);
	else
		putcharUART5(bs2+55);
	if(bs1<0xA)
		putcharUART5(bs1+0x30);
	else
		putcharUART5(bs1+55);
	if(bs0<0xA)
		putcharUART5(bs0+0x30);
	else
		putcharUART5(bs0+55);
    putcharUART5(' ');
 #endif
}
/******************************************************************/
void putnumUART5(unsigned char num)
{
 #ifdef Debug_Mode      //the device is in debug mode
  unsigned char i=0;

  while(num>(DtoH_10E2-1))  //10^2
  {
   num=num-DtoH_10E2;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E1-1))  //10^1
  {
   num=num-DtoH_10E1;
   i++;
  }
  putcharUART5(i+0x30);

  putcharUART5(num+0x30);

  putcharUART5(' ');
 #endif
}
/******************************************************************/
void putnumUART5_short(unsigned short num)
{
 #ifdef Debug_Mode      //the device is in debug mode
  unsigned char i=0;

  while(num>(DtoH_10E5-1))  //10^5
  {
   num=num-DtoH_10E5;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E4-1))  //10^4
  {
   num=num-DtoH_10E4;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E3-1))  //10^3
  {
   num=num-DtoH_10E3;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E2-1))  //10^2
  {
   num=num-DtoH_10E2;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E1-1))  //10^1
  {
   num=num-DtoH_10E1;
   i++;
  }
  putcharUART5(i+0x30);

  putcharUART5(num+0x30);

  putcharUART5(' ');
 #endif
}

//**************************************************//
void putnumUART5_int(unsigned int num)
{
 #ifdef Debug_Mode      //the device is in debug mode
  unsigned char i=0;

  while(num>(DtoH_10E9-1))  //10^9
  {
   num=num-DtoH_10E9;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E8-1))  //10^8
  {
   num=num-DtoH_10E8;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E7-1))  //10^7
  {
   num=num-DtoH_10E7;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E6-1))  //10^6
  {
   num=num-DtoH_10E6;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E5-1))  //10^5
  {
   num=num-DtoH_10E5;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E4-1))  //10^4
  {
   num=num-DtoH_10E4;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E3-1))  //10^3
  {
   num=num-DtoH_10E3;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E2-1))  //10^2
  {
   num=num-DtoH_10E2;
   i++;
  }
  putcharUART5(i+0x30);

  i=0;
  while(num>(DtoH_10E1-1))  //10^1
  {
   num=num-DtoH_10E1;
   i++;
  }
  putcharUART5(i+0x30);

  putcharUART5(num+0x30);

  putcharUART5(' ');
 #endif
}

//*************************************************//
void MCU_Sleep(void)
{
 SYSKEY=0x0;
 SYSKEY=0xAA996655;
 SYSKEY=0x556699AA;
 OSCCONSET=0x10;  //set the power saving mode to sleep
 SYSKEY=0x0;
 asm volatile("wait"); //put the device in selected power saving mode
}

//**************************************************//
void MCU_Idle(void)
{
 SYSKEY=0x0;
 SYSKEY=0xAA996655;
 SYSKEY=0x556699AA;
 OSCCONCLR=0x10;  //set the power saving mode to idle
 SYSKEY=0x0;
 asm volatile("wait"); //put the device in selected power saving mode
}


//**************************************************//
unsigned char start_I2C2(void)
{
 I2C2CONbits.SEN=1;     //start
 unsigned short i=0;
 unsigned char I2C_Err=1;  //I2C error indicator, 1=error, 0= no error
 for (;i<I2C_TimeOut;i++)
 {
  if (!I2C2CONbits.SEN)
  {
   I2C_Err=0;
   break;
  }
 }
 return I2C_Err;

}

//**************************************************//
unsigned char restart_I2C2(void)
{
 I2C2CONbits.RSEN=1;         //restart
 unsigned short i=0;
 unsigned char I2C_Err=1;  //I2C error indicator, 1=error, 0= no error
 for (;i<I2C_TimeOut;i++)
 {
  if (!I2C2CONbits.RSEN)
  {
   I2C_Err=0;
   break;
  }
 }
 return I2C_Err;

}

//**************************************************//
unsigned char stop_I2C2(void)
{
 I2C2CONbits.PEN=1;   //stop
 unsigned short i=0;
 unsigned char I2C_Err=1;  //I2C error indicator, 1=error, 0= no error
 for (;i<I2C_TimeOut;i++)
 {
  if (!I2C2CONbits.PEN)
  {
   I2C_Err=0;
   break;
  }
 }
 return I2C_Err;

}

//**************************************************//
unsigned char ack_I2C2(void)
{
 I2C2CONbits.ACKDT=0;   //set acknoledge
 I2C2CONbits.ACKEN=1;   //not ack
 unsigned short i=0;
 unsigned char I2C_Err=1;  //I2C error indicator, 1=error, 0= no error
 for (;i<I2C_TimeOut;i++)
 {
  if (!I2C2CONbits.ACKEN)
  {
   I2C_Err=0;
   break;
  }
 }
 return I2C_Err;

}

//**************************************************//
unsigned char nack_I2C2(void)
{
 I2C2CONbits.ACKDT=1;   //set not acknoledge
 I2C2CONbits.ACKEN=1;   //not ack
 unsigned short i=0;
 unsigned char I2C_Err=1;  //I2C error indicator, 1=error, 0= no error
 for (;i<I2C_TimeOut;i++)
 {
  if (!I2C2CONbits.ACKEN)
  {
   I2C_Err=0;
   break;
  }
 }
 return I2C_Err;

}

//**************************************************//
unsigned char putchar_I2C2(unsigned char I2Cdata)
{
 I2C2TRN=I2Cdata;   //send data
 unsigned short i=0;
 unsigned char I2C_Err=1;  //I2C error indicator, 1=error, 0= no error
 for (;i<I2C_TimeOut;i++)
 {
  if (!I2C2STATbits.TRSTAT)
  {
   break;
  }
 }
 i=0;
 for (;i<I2C_TimeOut;i++)
 {
  if (!I2C2STATbits.ACKSTAT)
  {
   I2C_Err=0;
   break;
  }
 }
 return I2C_Err;

}

//**************************************************//
unsigned char getchar_I2C2(void)
{
 I2C2CONbits.RCEN=1;         //receive
 unsigned short i=0;
 for (;i<I2C_TimeOut;i++)
 {
  if (!I2C2CONbits.RCEN)
  {
   break;
  }
 }
 return I2C2RCV;
}

//***************************************************//
unsigned short readADC(unsigned char adc_port)
{
  AD1CHS=adc_port<<16;
  //delay(d_10us);
  AD1CON1SET=0x0002;
  delay(d_10us);
  AD1CON1CLR=0x0002;
  while(!(AD1CON1 & 0x0001));
  return ADC1BUF0;
}
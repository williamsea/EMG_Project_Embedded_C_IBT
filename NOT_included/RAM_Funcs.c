//CX II RAM functions//
#include "p32mx795f512l.h"
#include "stdio.h"
#include "Port_init.h"
#include "plib.h"
#include "Glob_Var_Dec.h"


//*********************Function declerations***************//
void portOpenSPI1 (void);
void portCloseSPI1 (void);
char SRAM_ReadByte (unsigned int);
char SRAM_ReadStat (void);
void SRAM_WriteByte (char, unsigned int);
void SRAM_WriteStat (char);
void SRAM_WritePage (char*, unsigned int);
void SRAM_WritePage_EB (char*, unsigned int);
void SRAM_WriteWREN (void);
void SRAM_WriteWRDI (void);
void SRAM_Write_Fast_EB (unsigned char*, unsigned char*,unsigned int);
void SRAM_Write_Fast_EB_2 (unsigned char*, unsigned char*,unsigned int);


//*************************************************// 

//**********************************************//
//
//*************SPI1 Port Open********************//
void portOpenSPI1 (void)
{
 SPI1CONbits.ON=1; // turn the SPI1 module on
 SCK1_TRIS=0;  // SPI1 all I/O initilized as input, will be changed to output before the data transmission, and truned back to input after transmission
 SDO1_TRIS=0;  //
 CS1_1_TRIS=0;
 CS1_2_TRIS=0;
}

//*************SPI1 Port Close********************//
void portCloseSPI1 (void)
{
 SPI1CONbits.ON=0; // turn the SPI1 module off
 SCK1=0;
 SDO1=0;
 SCK1_TRIS=1;  // SPI1 all I/O initilized as input, will be changed to output before the data transmission, and truned back to input after transmission
 SDO1_TRIS=1;  //
 CS1_1_TRIS=1;
 CS1_2_TRIS=1;
}

//**********************SRAM status write********************//
void SRAM_WriteStat (char MODE)
{
 CS1_1=0;
 writeSPI1(SRAM_WRSR);
 writeSPI1(MODE);
 CS1_1=1;
}

//************************SRAM status read**********************//
char SRAM_ReadStat (void)
{
 unsigned char data_8;
 CS1_1=0;
 writeSPI1(SRAM_RDSR);
 data_8=readSPI1();
 CS1_1=1;
 return data_8;
}

//**********************SRAM byte read********************//
char SRAM_ReadByte (unsigned int SRAM_address)
{
 char data_8;
 CS1_1=0;
 writeSPI1(SRAM_READ);
 writeSPI1(Hi(SRAM_address));
 writeSPI1(Mi(SRAM_address));
 writeSPI1(Lo(SRAM_address));
 data_8=readSPI1();
 CS1_1=1;
 return data_8;
}

//**********************SRAM byte write********************//
void SRAM_WriteByte (char data, unsigned int SRAM_address)
{
 char data_8;
 CS1_1=0;
 writeSPI1(SRAM_WRITE);
 writeSPI1(Hi(SRAM_address));
 writeSPI1(Mi(SRAM_address));
 writeSPI1(Lo(SRAM_address));
 writeSPI1(data);
 CS1_1=1;
}

//**********************SRAM page write********************//
void SRAM_WritePage (char *Start_Index, unsigned int SRAM_address)
{
 char data_8;
 unsigned char i=0;
 CS1_1=0;
 writeSPI1(SRAM_WRITE);
 writeSPI1(Hi(SRAM_address));
 writeSPI1(Mi(SRAM_address));
 writeSPI1(Lo(SRAM_address));
 for (i=0;i<32;i++)
 {
  SPI1BUF=*Start_Index++;
  while(!SPI1STATbits.SPIRBF);
  data_8=SPI1BUF;
 } 
 CS1_1=1;
}

//**********************SRAM enhanced buffer page write********************//
void SRAM_WritePage_EB (char *Start_Index, unsigned int SRAM_address)
{
 char data_8;
 unsigned char i=0;
 initSPI1_Master_EB();
 CS1_1=0;
 while (SPI1STATbits.TXBUFELM!=0);  //for any reason the SPI1TXBUF is not empty, wait until it is empty
 if (SPI1STATbits.RXBUFELM!=0);   // for any reason the SPI1RFBUF is not empty, then empty it
   {
    for (i=0;i<SPI1STATbits.RXBUFELM; i++)
       {
        data_8=SPI1BUF;
       }
   }

 SPI1BUF=SRAM_WRITE;
 SPI1BUF=Hi(SRAM_address);
 SPI1BUF=Mi(SRAM_address);
 SPI1BUF=Lo(SRAM_address);
 for (i=0;i<4;i++)
 {
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
 } 

  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
 CS1_1=1;
}

//**********************SRAM write enable********************//
void SRAM_WriteWREN (void)
{
 CS1_1=0;
 writeSPI1(SRAM_WREN);
 CS1_1=1;
}

//**********************SRAM write disable********************//
void SRAM_WriteWRDI (void)
{
 CS1_1=0;
 writeSPI1(SRAM_WRDI);
 CS1_1=1;
}

//**********************SRAM enhanced buffer fast write********************//
void SRAM_Write_Fast_EB (unsigned char *Start_Index, unsigned char *End_Index,unsigned int SRAM_address)
{
 char data_8;
 unsigned char i=0;
 initSPI1_Master_EB();
 CS1_1=0;
 while (SPI1STATbits.TXBUFELM!=0);  //for any reason the SPI1TXBUF is not empty, wait until it is empty
 if (SPI1STATbits.RXBUFELM!=0);   // for any reason the SPI1RFBUF is not empty, then empty it
   {
    for (i=0;i<SPI1STATbits.RXBUFELM; i++)
       {
        data_8=SPI1BUF;
       }
   }

 SPI1BUF=SRAM_WRITE;
 SPI1BUF=Hi(SRAM_address);
 SPI1BUF=Mi(SRAM_address);
 SPI1BUF=Lo(SRAM_address);
 while (Start_Index<=End_Index)
 {
  SPI1BUF=*Start_Index++;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
 } 

  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
 CS1_1=1;

 initSPI1_Master();
}


//**********************SRAM enhanced buffer fast write version 2********************//
void SRAM_Write_Fast_EB_2 (unsigned char *Start_Index, unsigned char *End_Index,unsigned int SRAM_address)
{
 char data_8;
 unsigned char i=0;
 initSPI1_Master_EB();
 CS1_1=0;
 while (SPI1STATbits.TXBUFELM!=0);  //for any reason the SPI1TXBUF is not empty, wait until it is empty
 if (SPI1STATbits.RXBUFELM!=0);   // for any reason the SPI1RFBUF is not empty, then empty it
   {
    for (i=0;i<SPI1STATbits.RXBUFELM; i++)
       {
        data_8=SPI1BUF;
       }
   }

 unsigned char *Stop_Cycle_Index=End_Index-8; //the address where for cycle should stop. Every for cycle write 8 bytes

 SPI1BUF=SRAM_WRITE;
 SPI1BUF=Hi(SRAM_address);
 SPI1BUF=Mi(SRAM_address);
 SPI1BUF=Lo(SRAM_address);

 while (Start_Index<Stop_Cycle_Index)
 {
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;
  SPI1BUF=*Start_Index++;

  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
 } 

  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;

 while (Start_Index<=End_Index)
 {
  SPI1BUF=*Start_Index++;
  while (SPI1STATbits.RXBUFELM==0);
  data_8=SPI1BUF;
 } 

 CS1_1=1;
 initSPI1_Master();
}


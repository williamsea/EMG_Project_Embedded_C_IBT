//CXIII DAC functions//
#include "p32mx795f512l.h"
#include "stdio.h"
#include "Port_init.h"
#include "plib.h"
#include "Glob_Var_Dec.h"

//*****************************************************//
/*How you would use it, you have to define an output buffer which you update with the value you want to output (0-255, corresponds to 0-3.3V)

unsigned char DAC_WRITE_BUF_on[8]={255,128,255,255,255,0,255,255};

Then you call the function

DAC_Output(0b00100011,DAC_WRITE_BUF_off);
A 1 indicates that the you should update that channel and a 0 indicates you should leave it alone.

So the value in the first index of the output buffer goes to channel 1. The channel one indicator in DAC_Output is actually the least significant bit.
In the example above, you are updating channel 1 to 255 (3.3V), channel 2 to 128 (1.65V) and channel 6 to 0 (0V)

AN_OUT: 2,4,6,8,7,5,3,1
*/
//*****************************************************//

//*********************Function declerations***************//
unsigned char initDAC(void);
unsigned char DAC_Read(unsigned char, unsigned char, unsigned char*);
unsigned char DAC_Write(unsigned char, unsigned char, unsigned char*);

unsigned char DAC_Read2(unsigned char, unsigned char, unsigned char*);
unsigned char DAC_Write2(unsigned char, unsigned char, unsigned char*);

unsigned char DAC_Output(unsigned char, unsigned char*);
//example: DAC_Output(0b0011,DAC_WRITE_BUF);


//*****************************************************//
//*****************************************************//
unsigned char initDAC(void)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit
 unsigned char DAC_Set_Buf[0x32]=
{
 ///////Status
 0b00000000,   //0x00
 ///////Data
 0b00000000,   //0x01
 0b00000000,   //0x02
 0b00000000,   //0x03
 0b00000000,   //0x04
 0b00000000,   //0x05
 0b00000000,   //0x06
 0b00000000,   //0x07  N/A
 0b00000000,   //0x08  N/A
 0b00000000,   //0x09  N/A
 0b00000000,   //0x0A  N/A
 ///////System mode
 0b00000000,   //0x0B
 ///////INT source
 0b00000000,   //0x0C
 ///////Device ID
 0b00000000,   //0x0D
 ///////Dynamic range and high pass filter
 0b00010000,   //0x0E  RW
 0b00010010,   //0x0F  RW  //high pass cut-off setting
 ///////Orientation
 0b00000000,   //0x10
 0b11000000,   //0x11  RW
 0b00111111,   //0x12  RW  //orientation debonce counter
 0b00000000,   //0x13
 0b00000000,   //0x14      //orientation threshold and hysteresis
 ///////motion and free fall
 0b11111000,   //0x15  RW
 0b00000000,   //0x16
 0b00011111,   //0x17  RW //motion/free fall threshold
 0b00000011,   //0x18  RW //motion/free fall debonce counter
 0b00000000,   //0x19  N/A
 0b00000000,   //0x1A  N/A
 0b00000000,   //0x1B  N/A
 0b00000000,   //0x1C  N/A
 ///////Transient acceleration detection
 0b00011110,   //0x1D  RW
 0b00000000,   //0x1E
 0b00000001,   //0x1F  RW  //transient detection threshold
 0b00000011,   //0x20  RW  //transient debonce counter
 ///////Tap detection
 0b00000000,   //0x21  RW
 0b00000000,   //0x22
 0b00000000,   //0x23  RW
 0b00000000,   //0x24  RW
 0b00000000,   //0x25  RW
 0b00000000,   //0x26  RW
 0b00000000,   //0x27  RW
 0b00000000,   //0x28  RW
 ///////Auto-wake/sleep detection
 0b00001000,   //0x29  RW  //the time delay counter for entering the sleep mode
 ///////Config
 0b01011101,   //0x2A  RW //CONFIG1
 0b00001101,   //0x2B  RW //CONFIG2
 0b01111010,   //0x2C  RW //CONFIG3
 0b10110000,   //0x2D  RW //CONFIG4
 0b10111101,   //0x2E  RW //CONFIG5
 ///////Offset correctoin
 0b00000000,   //0x2F  RW //Offset X
 0b00000000,   //0x30  RW //Offset Y
 0b00000000,   //0x31  RW //Offset Z
};
 /////////////////////////////
 unsigned char i=0b01000000;
 if(DAC_Write(0x2B,1,&i)) //reset
 I2C2_Error=1;

 delay(d_100ms);

 if(DAC_Write(0x0E,1,DAC_Set_Buf+0x0E)) //XYZ_DATA_CFG
 I2C2_Error=1;

 if(DAC_Write(0x0F,1,DAC_Set_Buf+0x0F)) //HP_FILTER_CUTOFF
 I2C2_Error=1;

 if(DAC_Write(0x11,1,DAC_Set_Buf+0x11)) //PL_CFG
 I2C2_Error=1;

 if(DAC_Write(0x12,1,DAC_Set_Buf+0x12)) //
 I2C2_Error=1;

 if(DAC_Write(0x15,1,DAC_Set_Buf+0x15)) //FF_MT_CFG
 I2C2_Error=1;

 if(DAC_Write(0x17,1,DAC_Set_Buf+0x17)) //FF_MT_THS
 I2C2_Error=1;

 if(DAC_Write(0x18,1,DAC_Set_Buf+0x18)) //FF_MT_COUNT
 I2C2_Error=1;

 if(DAC_Write(0x1D,1,DAC_Set_Buf+0x1D)) //TRANSIENT_CFG
 I2C2_Error=1;

 if(DAC_Write(0x1F,1,DAC_Set_Buf+0x1F)) //TRANSIENT_THS
 I2C2_Error=1;
       
 if(DAC_Write(0x20,1,DAC_Set_Buf+0x20)) //TRANSIENT_COUNT
 I2C2_Error=1;

 if(DAC_Write(0x21,1,DAC_Set_Buf+0x21)) //PULSE_CFG
 I2C2_Error=1;

 if(DAC_Write(0x23,1,DAC_Set_Buf+0x23)) //PULSE_THSX
 I2C2_Error=1;

 if(DAC_Write(0x24,1,DAC_Set_Buf+0x24)) //PULSE_THSY
 I2C2_Error=1;

 if(DAC_Write(0x25,1,DAC_Set_Buf+0x25)) //PULSE_THSZ
 I2C2_Error=1;

 if(DAC_Write(0x26,1,DAC_Set_Buf+0x26)) //PULSE_TMLT
 I2C2_Error=1;

 if(DAC_Write(0x27,1,DAC_Set_Buf+0x27)) //PULSE_LTCY
 I2C2_Error=1;

 if(DAC_Write(0x28,1,DAC_Set_Buf+0x28)) //PULSE_WIND
 I2C2_Error=1;

 if(DAC_Write(0x29,1,DAC_Set_Buf+0x29)) //ASLP_COUNT
 I2C2_Error=1;

 if(DAC_Write(0x2B,1,DAC_Set_Buf+0x2B)) //CTRL_REG2
 I2C2_Error=1;

 if(DAC_Write(0x2C,1,DAC_Set_Buf+0x2C)) //CTRL_REG3
 I2C2_Error=1;

 if(DAC_Write(0x2D,1,DAC_Set_Buf+0x2D)) //CTRL_REG4
 I2C2_Error=1;

 if(DAC_Write(0x2E,1,DAC_Set_Buf+0x2E)) //CTRL_REG5
 I2C2_Error=1;

 if(DAC_Write(0x2F,1,DAC_Set_Buf+0x2F)) //OFF_X
 I2C2_Error=1;

 if(DAC_Write(0x30,1,DAC_Set_Buf+0x30)) //OFF_Y
 I2C2_Error=1;

 if(DAC_Write(0x31,1,DAC_Set_Buf+0x31)) //OFF_Z
 I2C2_Error=1;

 if(DAC_Write(0x2A,1,DAC_Set_Buf+0x2A)) //CTRL_REG1
 I2C2_Error=1;

 return I2C2_Error;
}


//**********************read DAC************************//
//this function reads up to 256 bytes from the DAC.
//returns the error message. 1=error, 0=no error
//TP_RedAddr: the start register address to read
//n: number of registers to read
//TPBuffer_ptr: the pointer of data buffer where the received data to be saved

unsigned char DAC_Read(unsigned char DAC_RegAddr, unsigned char n, unsigned char* DACBuffer_ptr)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit
 unsigned char i=0;

 if(start_I2C2())
 I2C2_Error=1; //start

 if(putchar_I2C2(DACAddress_W))
 I2C2_Error=1;  //send address

 if(putchar_I2C2(DAC_RegAddr))
 I2C2_Error=1;               //send register address

 if(restart_I2C2())
 I2C2_Error=1;         //restart

 if(putchar_I2C2(DACAddress_R))
 I2C2_Error=1;  //send address

 for (;i<n;i++)
 {
  *DACBuffer_ptr++=getchar_I2C2();
  if (i<(n-1))
  {
   if(ack_I2C2())
   I2C2_Error=1;
  }
 }

 if(nack_I2C2())
 I2C2_Error=1;

 if(stop_I2C2())
 I2C2_Error=1;

 return I2C2_Error;
}

//**********************write DAC************************//
//this function writes up to 256 bytes to the DAC.
//returns the error message. 1=error, 0=no error
//DAC_RedAddr: the start register address to write
//n: number of registers to read
//DACBuffer_ptr: the pointer of data buffer where the data to be written
unsigned char DAC_Write(unsigned char DAC_RegAddr, unsigned char n, unsigned char* DACBuffer_ptr)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit
 unsigned char i=0;

 if(start_I2C2())
 { I2C2_Error=1; } //start

//putstringUART5("Start = ");
//putnumUART5(I2C2_Error);
//putstringUART5("\r\n");
//I2C2_Error=0;

 if(putchar_I2C2(DACAddress_W))
 { I2C2_Error=1;}  //send address

//putstringUART5("DACAdd = ");
//putnumUART5(I2C2_Error);
//putstringUART5("\r\n");
//I2C2_Error=0;

 if(putchar_I2C2(DAC_RegAddr))
 { I2C2_Error=1; } //send register address

//putstringUART5("RegAddr = ");
//putnumUART5(I2C2_Error);
//putstringUART5("\r\n");
//I2C2_Error=0;

 for (;i<n;i++)
 {
  if(putchar_I2C2(*DACBuffer_ptr++))
  I2C2_Error=1; //send data
 }

//putstringUART5("Data = ");
//putnumUART5(I2C2_Error);
//putstringUART5("\r\n");
//I2C2_Error=0;

 if(stop_I2C2())
 { I2C2_Error=1;}

//putstringUART5("Stop = ");
//putnumUART5(I2C2_Error);
//putstringUART5("\r\n");
//I2C2_Error=0;
//
 return I2C2_Error;
}



//**********************read DAC************************//
//this function reads up to 256 bytes from the DAC.
//returns the error message. 1=error, 0=no error
//TP_RedAddr: the start register address to read
//n: number of registers to read
//TPBuffer_ptr: the pointer of data buffer where the received data to be saved

unsigned char DAC_Read2(unsigned char DAC_RegAddr, unsigned char n, unsigned char* DACBuffer_ptr)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit
 unsigned char i=0;

 if(start_I2C2())
 I2C2_Error=1; //start

 if(putchar_I2C2(DACAddress_W2))
 I2C2_Error=1;  //send address

 if(putchar_I2C2(DAC_RegAddr))
 I2C2_Error=1;               //send register address

 if(restart_I2C2())
 I2C2_Error=1;         //restart

 if(putchar_I2C2(DACAddress_R2))
 I2C2_Error=1;  //send address

 for (;i<n;i++)
 {
  *DACBuffer_ptr++=getchar_I2C2();
  if (i<(n-1))
  {
   if(ack_I2C2())
   I2C2_Error=1;
  }
 }

 if(nack_I2C2())
 I2C2_Error=1;

 if(stop_I2C2())
 I2C2_Error=1;

 return I2C2_Error;
}

//**********************write DAC************************//
//this function writes up to 256 bytes to the DAC.
//returns the error message. 1=error, 0=no error
//DAC_RedAddr: the start register address to write
//n: number of registers to read
//DACBuffer_ptr: the pointer of data buffer where the data to be written
unsigned char DAC_Write2(unsigned char DAC_RegAddr, unsigned char n, unsigned char* DACBuffer_ptr)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit
 unsigned char i=0;

 if(start_I2C2())
 I2C2_Error=1; //start

 if(putchar_I2C2(DACAddress_W2))
 I2C2_Error=1;  //send address

 if(putchar_I2C2(DAC_RegAddr))
 I2C2_Error=1;  //send register address

 for (;i<n;i++)
 {
  if(putchar_I2C2(*DACBuffer_ptr++))
  I2C2_Error=1; //send data
 }

 if(stop_I2C2())
 I2C2_Error=1;

 return I2C2_Error;
}

//*****************************************************//
//chns is the channel to be updated: 0b0001=channel 1, 0b0010=channel 2, 0b0100=channel 3, 0b1000=channel 4
//dac_value_ptr=the start pointer of a 4 unsigned char array. (for AD5305, 8bit resolution)
unsigned char DAC_Output(unsigned char chns, unsigned char* dac_value_ptr)
{
 unsigned char update_error=0;
 unsigned char data_string_header=0b00000000; //edited for new DAC
 unsigned char dac_write_buf[16];

 if(chns & 0b00000001) //channel 1 needs to be updated
 {
  dac_write_buf[0]=dac_value_ptr[0];
  dac_write_buf[1]=dac_value_ptr[0];
  if(DAC_Write(0x00,2,dac_write_buf))
  update_error=1;
 }

 if(chns & 0b00000010) //channel 2 needs to be updated
 {
  dac_write_buf[2]=dac_value_ptr[1];
  dac_write_buf[3]=dac_value_ptr[1];
  if(DAC_Write(0x01,2,dac_write_buf+2))
  update_error=1;
 }

 if(chns & 0b00000100) //channel 3 needs to be updated
 {
  dac_write_buf[4]=dac_value_ptr[2];
  dac_write_buf[5]=dac_value_ptr[2];
  if(DAC_Write(0x02,2,dac_write_buf+4))
  update_error=1;
 }

 if(chns & 0b00001000) //channel 4 needs to be updated
 {
  dac_write_buf[6]=dac_value_ptr[3];
  dac_write_buf[7]=dac_value_ptr[3];
  if(DAC_Write(0x03,2,dac_write_buf+6))
  update_error=1;
 }

 if(chns & 0b00010000) //channel 5 needs to be updated
 {
  dac_write_buf[8]=dac_value_ptr[4];
  dac_write_buf[9]=dac_value_ptr[4];
  if(DAC_Write(0x04,2,dac_write_buf+8))
  update_error=1;
 }

 if(chns & 0b00100000) //channel 6 needs to be updated
 {
  dac_write_buf[10]=dac_value_ptr[5];
  dac_write_buf[11]=dac_value_ptr[5];
  if(DAC_Write(0x05,2,dac_write_buf+10))
  update_error=1;
 }

 if(chns & 0b01000000) //channel 7 needs to be updated
 {
  dac_write_buf[12]=dac_value_ptr[6];
  dac_write_buf[13]=dac_value_ptr[6];
  if(DAC_Write(0x06,2,dac_write_buf+12))
  update_error=1;
 }

 if(chns & 0b10000000) //channel 8 needs to be updated
 {
  dac_write_buf[14]=dac_value_ptr[7];
  dac_write_buf[15]=dac_value_ptr[7];
  if(DAC_Write(0x07,2,dac_write_buf+14))
  update_error=1;
 }

 return update_error;

}
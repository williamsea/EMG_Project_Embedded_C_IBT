//CXIII Accelerometer functions//
#include "p32mx795f512l.h"
#include "stdio.h"
#include "Port_init.h"
#include "plib.h"
#include "Glob_Var_Dec.h"

//*********************Function declerations***************//
unsigned char initACM(void);
unsigned char ACM_Read(unsigned char, unsigned char, unsigned char*);
unsigned char ACM_Write(unsigned char, unsigned char, unsigned char*);

unsigned char ACM_data_read(void);
unsigned char ACM_orientation_read(void);
unsigned char ACM_motion_read(void);
unsigned char ACM_trans_read(void);
unsigned char ACM_mode_read(void);
unsigned char ACM_INT_read(void);
void ACM_INT_Process(void);

//*****************************************************//
//*****************************************************//
unsigned char initACM(void)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit
 unsigned char ACM_Set_Buf[0x32]=
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
 if(ACM_Write(0x2B,1,&i)) //reset
 I2C2_Error=1;

 delay(d_100ms);

 if(ACM_Write(0x0E,1,ACM_Set_Buf+0x0E)) //XYZ_DATA_CFG
 I2C2_Error=1;

 if(ACM_Write(0x0F,1,ACM_Set_Buf+0x0F)) //HP_FILTER_CUTOFF
 I2C2_Error=1;

 if(ACM_Write(0x11,1,ACM_Set_Buf+0x11)) //PL_CFG
 I2C2_Error=1;

 if(ACM_Write(0x12,1,ACM_Set_Buf+0x12)) //
 I2C2_Error=1;

 if(ACM_Write(0x15,1,ACM_Set_Buf+0x15)) //FF_MT_CFG
 I2C2_Error=1;

 if(ACM_Write(0x17,1,ACM_Set_Buf+0x17)) //FF_MT_THS
 I2C2_Error=1;

 if(ACM_Write(0x18,1,ACM_Set_Buf+0x18)) //FF_MT_COUNT
 I2C2_Error=1;

 if(ACM_Write(0x1D,1,ACM_Set_Buf+0x1D)) //TRANSIENT_CFG
 I2C2_Error=1;

 if(ACM_Write(0x1F,1,ACM_Set_Buf+0x1F)) //TRANSIENT_THS
 I2C2_Error=1;
       
 if(ACM_Write(0x20,1,ACM_Set_Buf+0x20)) //TRANSIENT_COUNT
 I2C2_Error=1;

 if(ACM_Write(0x21,1,ACM_Set_Buf+0x21)) //PULSE_CFG
 I2C2_Error=1;

 if(ACM_Write(0x23,1,ACM_Set_Buf+0x23)) //PULSE_THSX
 I2C2_Error=1;

 if(ACM_Write(0x24,1,ACM_Set_Buf+0x24)) //PULSE_THSY
 I2C2_Error=1;

 if(ACM_Write(0x25,1,ACM_Set_Buf+0x25)) //PULSE_THSZ
 I2C2_Error=1;

 if(ACM_Write(0x26,1,ACM_Set_Buf+0x26)) //PULSE_TMLT
 I2C2_Error=1;

 if(ACM_Write(0x27,1,ACM_Set_Buf+0x27)) //PULSE_LTCY
 I2C2_Error=1;

 if(ACM_Write(0x28,1,ACM_Set_Buf+0x28)) //PULSE_WIND
 I2C2_Error=1;

 if(ACM_Write(0x29,1,ACM_Set_Buf+0x29)) //ASLP_COUNT
 I2C2_Error=1;

 if(ACM_Write(0x2B,1,ACM_Set_Buf+0x2B)) //CTRL_REG2
 I2C2_Error=1;

 if(ACM_Write(0x2C,1,ACM_Set_Buf+0x2C)) //CTRL_REG3
 I2C2_Error=1;

 if(ACM_Write(0x2D,1,ACM_Set_Buf+0x2D)) //CTRL_REG4
 I2C2_Error=1;

 if(ACM_Write(0x2E,1,ACM_Set_Buf+0x2E)) //CTRL_REG5
 I2C2_Error=1;

 if(ACM_Write(0x2F,1,ACM_Set_Buf+0x2F)) //OFF_X
 I2C2_Error=1;

 if(ACM_Write(0x30,1,ACM_Set_Buf+0x30)) //OFF_Y
 I2C2_Error=1;

 if(ACM_Write(0x31,1,ACM_Set_Buf+0x31)) //OFF_Z
 I2C2_Error=1;

 if(ACM_Write(0x2A,1,ACM_Set_Buf+0x2A)) //CTRL_REG1
 I2C2_Error=1;

 return I2C2_Error;
}


//**********************read accelerometer************************//
//this function reads up to 256 bytes from the accelerometer.
//returns the error message. 1=error, 0=no error
//TP_RedAddr: the start register address to read
//n: number of registers to read
//TPBuffer_ptr: the pointer of data buffer where the received data to be saved

unsigned char ACM_Read(unsigned char ACM_RegAddr, unsigned char n, unsigned char* ACMBuffer_ptr)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit
 unsigned char i=0;

 if(start_I2C2())
 I2C2_Error=1; //start

 if(putchar_I2C2(ACMAddress_W))
 I2C2_Error=1;  //send address

 if(putchar_I2C2(ACM_RegAddr))
 I2C2_Error=1;               //send register address

 if(restart_I2C2())
 I2C2_Error=1;         //restart

 if(putchar_I2C2(ACMAddress_R))
 I2C2_Error=1;  //send address

 for (;i<n;i++)
 {
  *ACMBuffer_ptr++=getchar_I2C2();
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

//**********************write accelerometer************************//
//this function writes up to 256 bytes to the accelerometer.
//returns the error message. 1=error, 0=no error
//ACM_RedAddr: the start register address to read
//n: number of registers to read
//ACMBuffer_ptr: the pointer of data buffer where the data to be written
unsigned char ACM_Write(unsigned char ACM_RegAddr, unsigned char n, unsigned char* ACMBuffer_ptr)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit
 unsigned char i=0;

 if(start_I2C2())
 I2C2_Error=1; //start

 if(putchar_I2C2(ACMAddress_W))
 I2C2_Error=1;  //send address

 if(putchar_I2C2(ACM_RegAddr))
 I2C2_Error=1;               //send register address

 for (;i<n;i++)
 {
  if(putchar_I2C2(*ACMBuffer_ptr++))
  I2C2_Error=1;  //send address
 }

 if(stop_I2C2())
 I2C2_Error=1;

 return I2C2_Error;
}


//*******************accelerometer data read*********************//
unsigned char ACM_data_read(void)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit
 unsigned char acm_buf_temp[6];

 if(ACM_Read(0x00,1,&ACM_STATYS_BUF)) //status byte read
 I2C2_Error=1;

 if(ACM_Read(0x01,1,acm_buf_temp)) //X_MSB
 I2C2_Error=1;
 if(ACM_Read(0x02,1,acm_buf_temp+1)) //X_LSB
 I2C2_Error=1;
 if(ACM_Read(0x03,1,acm_buf_temp+2)) //Y_MSB
 I2C2_Error=1;
 if(ACM_Read(0x04,1,acm_buf_temp+3)) //Y_LSB
 I2C2_Error=1;
 if(ACM_Read(0x05,1,acm_buf_temp+4)) //Z_MSB
 I2C2_Error=1;
 if(ACM_Read(0x06,1,acm_buf_temp+5)) //Z_LSB
 I2C2_Error=1;

 ACM_DATA_BUF[0]=(acm_buf_temp[0]<<8)+acm_buf_temp[1]+0x8000;  //X, the "+0x8000" is to move the base line to 0x8000
 ACM_DATA_BUF[1]=(acm_buf_temp[2]<<8)+acm_buf_temp[3]+0x8000;  //Y
 ACM_DATA_BUF[2]=(acm_buf_temp[4]<<8)+acm_buf_temp[5]+0x8000;  //Z

 ////test////
 putstringUART5("\r\nX=");
 putnumUART5_short(ACM_DATA_BUF[0]);
 putstringUART5(" Y=");
 putnumUART5_short(ACM_DATA_BUF[1]);
 putstringUART5(" Z=");
 putnumUART5_short(ACM_DATA_BUF[2]);
 putstringUART5("\r\n");
 ///////////

 return I2C2_Error;
}

//*******************accelerometer data read*********************//
unsigned char ACM_orientation_read(void)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit

 if(ACM_Read(0x10,1,&ACM_ORIENTATION_BUF)) //status byte read
 I2C2_Error=1;

 ////test////
 putstringUART5("\r\nORI=");
 puthexUART5(ACM_ORIENTATION_BUF);
 putstringUART5("\r\n");
 ///////////

 return I2C2_Error;
}

//*******************accelerometer data read*********************//
unsigned char ACM_motion_read(void)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit

 if(ACM_Read(0x16,1,&ACM_MOTION_BUF)) //status byte read
 I2C2_Error=1;

 ////test////
 putstringUART5("MOT=");
 puthexUART5(ACM_MOTION_BUF);
 putstringUART5("\r\n");
 ///////////

 return I2C2_Error;
}

//*******************accelerometer data read*********************//
unsigned char ACM_trans_read(void)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit

 if(ACM_Read(0x1E,1,&ACM_TRANS_BUF)) //status byte read
 I2C2_Error=1;

 ////test////
 putstringUART5("TNS=");
 puthexUART5(ACM_TRANS_BUF);
 putstringUART5("\r\n");
 ///////////

 return I2C2_Error;
}

//*******************accelerometer data read*********************//
unsigned char ACM_mode_read(void)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit

 if(ACM_Read(0x0B,1,&ACM_MODE_BUF)) //status byte read
 I2C2_Error=1;

 ////test////
 putstringUART5("MOD=");
 puthexUART5(ACM_MODE_BUF);
 putstringUART5("\r\n");
 ///////////

 return I2C2_Error;
}

//*******************accelerometer data read*********************//
unsigned char ACM_INT_read(void)
{
 unsigned char I2C2_Error=0;  //I2C2 error bit
 unsigned char acm_buf_temp[6];

 if(ACM_Read(0x0C,1,&ACM_INT_BUF)) //status byte read
 I2C2_Error=1;

 ////test////
 putstringUART5("ACM INT=");
 puthexUART5(ACM_INT_BUF);
 putstringUART5("\r\n");
 ///////////

 return I2C2_Error;
}

//*******************accelerometer data read*********************//
void ACM_INT_Process(void)
{
  putstringUART5("ACM event detected >>>>>>>>>>>>>>>>>>>>>>>\r\n");
  ACM_INT_BUF=1;
  while(ACM_INT_BUF !=0)
  {
	 if(ACM_INT_read()==0)  //if no error
	 {

	   if((ACM_INT_BUF & 0b10000000)!=0)  //sleep/wake int
	   {
	    ACM_mode_read();	
	   }
	
	   if((ACM_INT_BUF & 0b00100000)!=0)  //trans int
	   {
	    ACM_trans_read();	
	   }
	
	   if((ACM_INT_BUF & 0b00010000)!=0)  //orientation int
	   {
	    ACM_orientation_read();
	   }
	
	   if((ACM_INT_BUF & 0b00001000)!=0)  //pulse int
	   {

	   }
	
	   if((ACM_INT_BUF & 0b00000100)!=0)  //motion/free fall int
	   {
	    ACM_motion_read();
	   }
	
	   if((ACM_INT_BUF & 0b00000001)!=0) //data ready int
	   {
	
	   }
	 }
  }

  if(ACM_TRANS_BUF!=0)  //if new transient event is detected
  {
   ACM_RESULT=(ACM_ORIENTATION_BUF & 0b00000111)+0b10000000;
   ACM_TRANS_BUF=0;  //clear the buffer
  }
  else
  {
   ACM_RESULT=(ACM_ORIENTATION_BUF & 0b00000111);
  }
  
}
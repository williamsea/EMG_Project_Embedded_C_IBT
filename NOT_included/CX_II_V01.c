#include "p32mx795f512l.h"
#include "system.h"
#include "Port_init.h"
#include "plib.h"
#include "Glob_Var_Dec.h"
#include "math.h"
#include "stdio.h"
#include "string.h"


//*********************************************************************************//
    #pragma config POSCMOD=HS, FNOSC=FRCPLL, ICESEL = ICS_PGx1, FSOSCEN=OFF, FCKSM=CSECME
    #pragma config FPLLIDIV=DIV_2, FPLLMUL=MUL_18, FPLLODIV=DIV_1   //18MHz ALU clock speed
    #pragma config FPBDIV=DIV_1, FWDTEN=OFF, CP=OFF, BWP=OFF, UPLLEN=OFF
    #pragma config PWP=OFF
 
//*********************************************************************************//
///////test///////////

unsigned short counter01=0;

//////////////////////

//****************************************************//


///********************************Global Variables Local to THIS file*********************************************/

unsigned char CommandCount=0;	
unsigned char CmdBuf_temp[8];
unsigned char prog_counter=0;  

unsigned char hold;

//Feature extraction initialization variables
int i, j;							//variables
//int start_count =0;					//check

unsigned int classcount = 0; //I2C testing
unsigned int prevclasstest; //I2C testing

//unsigned char motor_count =0;
//unsigned char ms_state;
int d;
unsigned int GUI_status;

unsigned short test1=0;   //test
unsigned char test_I2Cbusy=0;  //test
unsigned char test_T1busy=0;  //test

int idleflag1 = 0;		//I2C
int idleflag2 = 2;		//I2C

int testcount = 0;	//counter for automatic EMG switching

int prev_class = 0;	
unsigned int mvmt_cnt[25] = {0};
unsigned int grip_cnt = 0;
unsigned int time_cnt = 0;

unsigned int initcnt = 0;
unsigned char trackingtemp = 0;

/******************************END of Global Variables***************************************/

//****************************************************************//

int main (void)
{
	delay(d_1s);
 	Device_PowerON();

//	initDAC();

 while (1)
 {
//	delay(d_1s);

//0 = rest, 1 = hold open, 2 = hold close, 3 = power, 4 = index, 5 = pinch, 6 = three jaw chuck, 7 = precision close, 8 = short open, 9 = short close
//p = pronate, s = supinate, b = bend, e = extend
	last_class = classtest;
	if(get == 48) //turn off all movement output (0)
	{
		classtest = 0;
		LED1_RGB(1,0,0);
 		LED2_RGB(1,0,1);
 		LED3_RGB(0,1,1);
 		LED4_RGB(1,1,0);
	}	
	else if(get == 49) 	//hold open until command is changed (1)
	{
		classtest = 1;
		LED1_RGB(0,0,1);
 		LED2_RGB(1,0,1);
 		LED3_RGB(0,0,0);
 		LED4_RGB(0,0,0);
	}
	else if(get == 50) //hold close until command is changed (2)
	{
		classtest = 2;
		LED1_RGB(0,0,0);
 		LED2_RGB(0,0,1);
 		LED3_RGB(1,0,1);
 		LED4_RGB(0,0,0);
	}
	else if(get == 51) //change i2c command to power grip (finger point in non-opposed)(3)
	{
		classtest = 3;
		LED1_RGB(0,0,0);
 		LED2_RGB(0,0,0);
 		LED3_RGB(0,0,1);
 		LED4_RGB(1,0,1);
 		get = 0;
	}
	else if(get == 52) //change i2c command to trigger grip (column in non-opposed) (4)
	{
		classtest = 4;
		LED1_RGB(1,0,1);
 		LED2_RGB(0,0,0);
 		LED3_RGB(0,0,0);
 		LED4_RGB(0,0,1);
 		get = 0;
	}
	else if(get == 53) //change i2c command to precision open grip (mouse in non-opposed) (5)
	{
		classtest = 5;
		LED1_RGB(1,0,0);
 		LED2_RGB(0,1,0);
 		LED3_RGB(0,0,0);
 		LED4_RGB(0,0,0);
 		get = 0;
	}			
	else if(get == 54) //change i2c command to tripod grip (key in non-opposed) (6)
	{
		classtest = 6;
		LED1_RGB(0,0,0);
 		LED2_RGB(1,0,0);
 		LED3_RGB(0,1,0);
 		LED4_RGB(0,0,0);
 		get = 0;
	}			
	else if(get == 55) //change i2c command to precision close grip (relaxed in non-opposed) (7)
	{
		classtest = 7;
		LED1_RGB(0,0,0);
 		LED2_RGB(0,0,0);
 		LED3_RGB(1,0,0);
 		LED4_RGB(0,1,0);
 		get = 0;
	}			
	else if(get == 56) //open for short increment (8)
	{
		classtest = 8;
		LED1_RGB(0,1,0);
 		LED2_RGB(0,0,0);
 		LED3_RGB(0,0,0);
 		LED4_RGB(1,0,0);
 		get = 0;
	}	
	else if(get == 57) //close for short increment (9)
	{
		classtest = 9;
		LED1_RGB(0,1,0);
 		LED2_RGB(0,0,0);
 		LED3_RGB(0,1,1);
 		LED4_RGB(1,0,0);
 		get = 0;
	}
	else if(get == 112) //hold pronate until command is changed (p)
	{
		classtest = 10;
		LED1_RGB(0,1,0);
 		LED2_RGB(1,1,0);
 		LED3_RGB(0,0,0);
 		LED4_RGB(1,0,0);
	}
	else if(get == 115) //hold supinate until command is changed (s)
	{
		classtest = 11;
		LED1_RGB(0,1,0);
 		LED2_RGB(0,0,1);
 		LED3_RGB(0,0,1);
 		LED4_RGB(1,0,0);
	}
	else if(get == 98) //hold bend until command is changed (b)
	{
		classtest = 12;
		LED1_RGB(0,1,0);
 		LED2_RGB(1,0,0);
 		LED3_RGB(0,0,0);
 		LED4_RGB(1,1,1);
	}
	else if(get == 101) //hold extend until command is changed (e)
	{
		classtest = 13;
		LED1_RGB(1,1,1);
 		LED2_RGB(0,1,0);
 		LED3_RGB(0,0,0);
 		LED4_RGB(1,0,0);
	}
	else if(get == 119) //pronate for short increment (w)
	{
		classtest = 14;
		LED1_RGB(1,1,1);
 		LED2_RGB(0,0,0);
 		LED3_RGB(0,1,1);
 		LED4_RGB(1,0,0);
 		get = 0;
	}
	else if(get == 120) //supinate for short increment (x)
	{
		classtest = 15;
		LED1_RGB(0,1,0);
 		LED2_RGB(0,1,0);
 		LED3_RGB(0,1,0);
 		LED4_RGB(1,0,0);
 		get = 0;
	}
	else if(get == 121) //bend for short increment (y)
	{
		classtest = 16;
		LED1_RGB(1,1,0);
 		LED2_RGB(1,0,0);
 		LED3_RGB(1,0,0);
 		LED4_RGB(1,0,1);
 		get = 0;
	}
	else if(get == 122) //extend for short increment (z)
	{
		classtest = 17;
		LED1_RGB(1,1,0);
 		LED2_RGB(0,1,0);
 		LED3_RGB(0,0,1);
 		LED4_RGB(1,1,0);
 		get = 0;
	}
	else //any other key turns off the outputs and puts on a red light
	{
		classtest = 127;
		LED1_RGB(0,1,1);
 		LED2_RGB(1,1,1);
 		LED3_RGB(1,1,1);
 		LED4_RGB(1,1,1);
 		get = 0;
	}	
	sendArmFcns(classtest);

 }

}


//**************************************************************//
void __ISR(4, T1_IPL) T1Interrupt(void)
{
	TMR1=0;
 	IFS0bits.T1IF=0;
 
}

//********************T2 interrupt************************//
void __ISR(8, T2_IPL) T2Interrupt(void)
{

//time_cnt++; using movement counter as an estimate


 TMR2=0;
 IFS0bits.T2IF=0;
}

//********************T3 interrupt************************//
/*void __ISR(12, T3_IPL) T3Interrupt(void)
{
 IFS0bits.T3IF=0;
 TMR3=0xF922;
}
*/

//***********************************************//
void __ISR(16, T4_IPL) T4Interrupt(void)
{
 IFS0bits.T4IF=0;
 TMR4=0;
}

//**********************************************//
//void __ISR(20, T5_IPL) T5Interrupt(void)
//{
//  MotorStop(); //stop motor
//  IFS0bits.T5IF=0;
//  TMR5=0;
//  T5CONbits.ON=0;
//}

//***********************UART2 Rx interrupt*********************//
void __ISR(32, U2_IPL) U2RXInterrupt(void)    
{     
  IFS1bits.U2RXIF = 0;
  U2Rx_Buf=U2RXREG;
}

////***********************UART3 Rx interrupt*******************//

void __ISR(31, U3_IPL) U3RXInterrupt(void)   // bluetooth
{    
//	putstringUART5("U3 isr \r\n"); 
//	U2TXREG = U3RXREG;
//	while (U2STAbits.TRMT == 0);
//	U5TXREG = U3RXREG;
//	while (U5STAbits.TRMT == 0);

	get = U3RXREG;
//	puthexUART5(get);
	putnumUART5(get);

IFS1bits.U3RXIF=0;

}


//***********************UART5 Rx interrupt*******************//
void __ISR(51, U5_IPL) U5RXInterrupt(void)   // uart receive interupt from the external computer
{     
 U5Rx_Buf=U5RXREG;
// putcharUART5(U5Rx_Buf);

//sendArmFcns(3);
//HandMove(1);
 //LED1_RGB(0,0,0);
 IFS2bits.U5RXIF=0;
}

//******************INT1*********************//
void __ISR(7, INT1_IPL) INT1Interrupt(void)  //
{
	IO1 = 1;
 IFS0bits.INT1IF=0;
}

//*******************INT2************************//
void __ISR(11, INT2_IPL) INT2Interrupt(void)  //
{
	IO1 = 0;
 LED1_RGB(0,1,1);


 IFS0bits.INT2IF=0;
}



//****************************************************//
void __ISR(33, ipl7) I2C2SlaveInterrupt(void)
{
	 unsigned char Temp=0;

	if(I2C2STATbits.I2COV)
		{
			Temp = I2C2RCV;
			while(I2C2STATbits.RBF);
			I2C2STATbits.I2COV = 0;     
		}

	if((I2C2STATbits.R_W == 0)&&(I2C2STATbits.D_A == 0))	//Address matched
		{
			Temp = I2C2RCV;		//dummy ready
	     	while(I2C2STATbits.RBF);  //wait until received
            CommandCount=0;
		}

	else if((I2C2STATbits.R_W == 0)&&(I2C2STATbits.D_A == 1))	//check for data	
		{  
			int I2C2event_code;
			Temp = I2C2RCV;										//dummy read //release SCL lin
			I2C2CONSET = 0x1000;								//release SCL line
	     	while(I2C2STATbits.RBF);							//wait until received
            I2C2event_code=Temp;
		}

	else if((I2C2STATbits.R_W == 1)&&(I2C2STATbits.D_A == 0))
		{
			Temp = I2C2RCV;
        	CommandCount==0;
        	I2C2TRN=CmdBuf_temp[CommandCount];
        	CommandCount++;
			I2C2CONSET = 0x1000;								//release SCL line
			while(I2C2STATbits.TBF);							//wait until transmitted
    	}

    else
    {
		Temp = I2C2RCV;
        I2C2TRN=CmdBuf_temp[CommandCount];
		I2C2CONSET = 0x1000;									//release SCL line
		while(I2C2STATbits.TBF);								//wait until transmitted
        CommandCount++;
    }

 IFS1bits.I2C2SIF=0;
 IFS1bits.I2C2MIF=0;
}

//****************************************************//
void __ISR(25, ipl7) I2C1SlaveInterrupt(void)
{
// disableT1();

test_I2Cbusy=1;
	I2C1CONbits.SCLREL = 0;

//T1CONbits.ON=0;
	unsigned char Temp=0;
//puthexUART5_int(I2C1CON);
 if(test_T1busy==1)
{
// putstringUART5("T1ISR\r\n");
}

//disableT1();
//initT1(64);
//T1CONbits.ON=0;
	//INTDisableInterrupts(); //DEBUG

//disableT1();
//T1CONbits.ON = 0;
//IFS0bits.T1IF=0;
//IEC0bits.T1IE=0;	

//	putstringUART5("I2C ");	//DEBUG
//	puthexUART5_int(I2C1CON);
//	putstringUART5("\r\n");	//DEBUG

//	if(idleflag1 == 1)
//	{
//		hand_action = IDLE;
//	}
//	if(idleflag2 == 0)
//	{
//		hand_action = IDLE;
//	}
//

	if(I2C1STATbits.I2COV)
	{
		Temp = I2C1RCV;
		while(I2C1STATbits.RBF);

		I2C1STATbits.I2COV = 0;
	}

	if((I2C1STATbits.R_W == 0)&&(I2C1STATbits.D_A == 0))	//Address matched
	{
		Temp = I2C1RCV;		//dummy ready
		I2C1CONbits.SCLREL = 1;
	 	while(I2C1STATbits.RBF);  //wait until received

		CommandCount=0;

	}
	
	else if((I2C1STATbits.R_W == 0)&&(I2C1STATbits.D_A == 1))	//check for data	
	{  
		Temp = I2C1RCV;										//dummy read //release SCL li
		I2C1CONbits.SCLREL = 1;
	 	while(I2C1STATbits.RBF);							//wait until received
	
	}
	
//	else if((I2C1STATbits.R_W == 1)&&(I2C1STATbits.D_A == 0))
	else
	{
		Temp = I2C1RCV;
//putnumUART5_int(hand_action);	//debug
//putstringUART5("\r\n");	//DEBUG
//putnumUART5_int(classtest);	//debug

/////////////////test///////////////
//        unsigned char test01=0;
//        if(prev_hand_action!=hand_action)
//        {
//
//         if(counter01>2000)
//         {
//          prev_hand_action=hand_action;
//          test01=1;
//          counter01=0;
//         }
//        }
//   	
////////////////////////////////////
putnumUART5(hand_action);
		switch(hand_action)	//used to be hand_action
		{
			case PINCH:

				I2C1TRN = txO_PINCH_UO_KEY[CommandCount++];
//				if(test01)putcharUART5('P');

//puthexUART5_int(I2C1TRN);
putstringUART5("\r\nPCH\r\n");	//DEBUG

			break;
			
			case POWER:

				I2C1TRN = txO_POWER_UO_FINGER_POINT[CommandCount++];
//				if(test01)putcharUART5('W');

//puthexUART5_int(I2C1TRN);
putstringUART5("\r\nPWR\r\n");	//DEBUG

			break;
			
			case PRECOPEN:

				I2C1TRN = txO_PRECOPEN_UO_MOUSE[CommandCount++];
//				if(test01)putcharUART5('R');

//puthexUART5_int(I2C1TRN);
putstringUART5("\r\nPRE\r\n");	//DEBUG

			break;
			
			case TRIGGER:

				I2C1TRN = txO_TRIGGER_UO_COLUMN[CommandCount++];
//				if(test01)putcharUART5('T');

//puthexUART5_int(I2C1TRN);
putstringUART5("\r\nTRI\r\n");	//DEBUG

			break;
			
			case PRECCLOSE:

				I2C1TRN = txO_PRECCLOSE_UO_RELAXED[CommandCount++];
//				if(test01)putcharUART5('E');

			break;
			
			case VIBRATE:

				I2C1TRN = tx_VIBRATE[CommandCount++];
//				if(test01)putcharUART5('V');

			break;

//			case IDLE:
//				//if(test01)putcharUART5('I');
//			break;
		}



//		if(hand_action != IDLE)
//		{
//		prev_hand_action = hand_action;
//		}	
//	

		if(CommandCount >= 1)
		{
			LED2_RGB(1,0,0);
		}
		
		if(CommandCount >= 2)
		{
			LED2_RGB(0,1,0);
		}

		if(CommandCount >= 3)
		{
//			if(idleflag1 == 0)
//			{
//				hand_action = IDLE;
//				idleflag1 = 1;
//			}
//			else if(idleflag1 == 1)
//			{
//				hand_action = prev_hand_action;
//				idleflag1 = 0;
//				idleflag2 = 1;
//			}
//			else if(idleflag2 == 1)
//			{
//				hand_action = IDLE;
//				idleflag2 = 0;
//			}
			CommandCount = 0;
//			IEC0bits.I2C1SIE=0;
//			classtest = IDLE;
			hand_action = IDLE;	// used to be hand_action, does 
			LED2_RGB(0,0,1);
		}
//		delay(1);


//if(hand_action != IDLE)
//{
//putnumUART5_int(hand_action);	//debug
//putstringUART5("\r\n");	//DEBUG
//}
		I2C1CONbits.SCLREL = 1;
		I2C1CONSET = 0x1000;								//release SCL line

		while(I2C1STATbits.TBF);							//wait until transmitted

//			IEC0bits.T1IE = 1;

    		
	}
	
	//IEC0bits.I2C1SIE=0;  //I2C1 slave interrupt // ADDED
	
	IFS0bits.I2C1SIF=0;
	IFS0bits.I2C1MIF=0;
	//	IEC0bits.I2C1SIE=0;  //I2C1 slave interrupt // ADDED

 //INTEnableInterrupts(); //DEBUG
// initT1(32);
// T1CONbits.ON=1;
test_I2Cbusy=0;
}
//*******************CN************************//
void __ISR(26, ICN_IPL) CNInterrupt(void)  //CN interrupts for buttons, accelerometer,  etc.
{
 IFS1bits.CNIF = 0;		// clear interrupt flag
}






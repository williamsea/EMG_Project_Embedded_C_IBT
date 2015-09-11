//CX II system functions//
#include "p32mx795f512l.h"
#include "stdio.h"
#include "Port_init.h"
#include "plib.h"
#include "Glob_Var_Dec.h"
#include "math.h"


///********************************Global Variables Local to THIS file*********************************************/

//M matrix variable initialization
char *data_buff_M;					
int data_length_M;
char data_H_M, data_L_M;			//define low and high bytes

//S matrix variable initialization
char *data_buff_S;
int data_length_S;
char data_H_S, data_L_S;			//define low and high bytes

//variables for error detection logic for M and S values
char chksum, chksum_rev;			//initialization
int sum_M, sum_S, sum_rx;		//variables


char ack=0;							//?????????.
char dtemp=0;						//used to skip steps

//Feature extraction initialization variables
int i, j;							//variables
double d;

//*********************Function declerations***************//
void MCU_Command_Decoder (unsigned char);
void Device_PowerON(void);

void LED1_RGB(unsigned char, unsigned char, unsigned char);
void LED2_RGB(unsigned char, unsigned char, unsigned char);
void LED3_RGB(unsigned char, unsigned char, unsigned char);
void LED4_RGB(unsigned char, unsigned char, unsigned char);

void sendClass(char);
void sendArmFcns(char); //for Johnny's arm

void MotorMove(unsigned char, unsigned char, unsigned short);
void MotorStop(void);
void HandMove(unsigned char);

//**************************decode the commands from the MCU2********************//
void MCU_Command_Decoder (unsigned char CommandCode)
{
 switch (CommandCode)
 {
  case 'S':                      //start DAQ
           putstringUART2("\r\ncommand recognized.\r\n");
           putstringUART2("AFE initializing...\r\n");
           BufPointer_W=DATA_BUF;  //buffer write pointer reset
           initDAQ();
           putstringUART2("DAQ started.\r\n");
           break;
  case 'P':                      //pause DAQ
           putstringUART2("\r\ncommand recognized.\r\n");
           pauseDAQ();
           putstringUART2("DAQ paused.\r\n");
           break;
  default:
           break; 
 } 

}

//*****************Power on************************//
void Device_PowerON(void)
{

 SYSTEMConfigPerformance(72000000ul);   // ALU performance optimized for the given clock frequency

 ETHCON1=0;
 DDPCONbits.JTAGEN = 0;   // disable JTAG port
 DDPCONbits.TROEN=0;
 DDPCONbits.TDOEN=0;
 initPORTs();
 delay(30000);
 INTEnableSystemMultiVectoredInt();  //enable the multivectored interrupt<<<<required for interrupt enabling>>>
 INTEnableInterrupts(); //DEBUG


 LED1_RGB(0,1,1);
 LED2_RGB(1,0,1);
 LED3_RGB(1,1,0);
 LED4_RGB(0,0,0);


 initSPI3_Master();
 initSPI1_Master();
 portOpenSPI1 ();
 
//initT1(32);
//initT2(32768);	//Tracking usage time, put this in disconnect loop, put disableT2 in connected code

initUART5(BR_921600_BST);	//DEBUG added BR_
//initUART2(BR_921600_BST);
//initUART3(BR_921600_BST);     //bluetooth
initUART3(BR_115200_BST);     //bluetooth

// if(RTCCONbits.ON==0)  //if the RTCC module is off
// {
//  initRTCC(InitialDate, InitialTime);   //start real time clock and calender
// }
//
//initICN();                //all ICN interrupts (accelerometers, etc.)
//initINT2();               //AFE
//initINT1();
//initACM(); 

 initI2C2(); //DEBUG commented out
 delay(d_1s);
 initI2C1(); 
 //delay(d_1s);


 U2MODEbits.ON =1;      //Turn ON UART  2
 U3MODEbits.ON =1;      //Turn ON UART  3
 U5MODEbits.ON =1;      //Turn ON UART  5	//DEBUG added UART5
 putstringUART5("Device on\n");
}

//******************************************************************//
//set the LED on/off and color. 1=om, 0=off
void LED1_RGB(unsigned char led_r, unsigned char led_g, unsigned char led_b)
{
 LED1_R=led_r;
 LED1_G=led_g;
 LED1_B=led_b;
}


//******************************************************************//
//set the LED on/off and color. 1=om, 0=off
void LED2_RGB(unsigned char led_r, unsigned char led_g, unsigned char led_b)
{
 LED2_R=led_r;
 LED2_G=led_g;
 LED2_B=led_b;
}

//******************************************************************//
//set the LED on/off and color. 1=om, 0=off
void LED3_RGB(unsigned char led_r, unsigned char led_g, unsigned char led_b)
{
 LED3_R=led_r;
 LED3_G=led_g;
 LED3_B=led_b;
}


//******************************************************************//
//set the LED on/off and color. 1=om, 0=off
void LED4_RGB(unsigned char led_r, unsigned char led_g, unsigned char led_b)
{
 LED4_R=led_r;
 LED4_G=led_g;
 LED4_B=led_b;
}


//**************************************************************************************
void sendClass(char send_class)//sends class value to GUI
{
	U3TXREG = (0xFF);
    while (U3STAbits.TRMT == 0);
	U3TXREG = (send_class);
    while (U3STAbits.TRMT == 0);
	U3TXREG = (0x08);
    while (U3STAbits.TRMT == 0);
}



//*********************************************************
void sendArmFcns(char send_class)
{

    MotorStop();

    unsigned char DAC_WRITE_BUF_on[8] = {50,100,150,200,250,255,0,10};
    DAC_Output(0b11111111, DAC_WRITE_BUF_on);


//
//	switch (send_class)
//	{
////commented out HandMove(0),(1),(2)
////testing cuff switch I2C, auto EMG
//
//		case 0:	         //rest
//			MotorStop();
//			DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//		break;
//
//		case 1:         //Open Hand, hold
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			//choose one of below outputs
//			HandMove(1);
//			DAC_Output(0b00000001,DAC_WRITE_BUF_on);
//        break;
//
//        case 2:         //Close Hand, hold
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			//choose one of the below outputs
//			HandMove(2);
//			DAC_Output(0b00000010,DAC_WRITE_BUF_on);
//        break;
//
//		case 3: 		//power grip
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			hand_action = POWER;
//			classtest = 0;
//        break;
//
//		case 4:			//trigger grip
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			hand_action = TRIGGER;
//			classtest = 0;
//		break;
//
//		case 5:			//precision open grip
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b111111,DAC_WRITE_BUF_off);
//			}
//
//			hand_action = PRECOPEN;
//			classtest = 0;
//        break;
//
//		case 6:			//tripod grip
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			hand_action = PINCH;
//			classtest = 0;
//        break;
//
//		case 7:			//precision close grip
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			hand_action = PRECCLOSE;
//			classtest = 0;
//        break;
//
//        case 8:         //Open Hand, short
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			//choose one of below outputs/off commands
//			HandMove(1);
//			DAC_Output(0b00000001,DAC_WRITE_BUF_on);
//			delay(d_1s);
//			HandMove(0);
//			DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			classtest = 0;
//        break;
//
//        case 9:         //Close Hand, short
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			//choose one of the below outputs/off commands
//			HandMove(2);
//			DAC_Output(0b00000010,DAC_WRITE_BUF_on);
//			delay(d_1s);
//			HandMove(0);
//			DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			classtest = 0;
//        break;
//
//		case 10:		//Pronate, hold
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			MotorMove(1,0,Motor_Run_Time);
//		break;
//
//		case 11:		//Supinate, hold
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			MotorMove(2,0,Motor_Run_Time);
//		break;
//
//		case 12:		//Bend, hold
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			MotorMove(0,1,Motor_Run_Time);
//		break;
//
//		case 13:		//Extend, hold
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			MotorMove(0,2,Motor_Run_Time);
//		break;
//
//		case 14:		//Pronate, short
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			MotorMove(1,0,Motor_Run_Time);
//			delay(d_1s);
//			MotorStop();
//			classtest = 0;
//		break;
//
//		case 15:		//Supinate, short
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			MotorMove(2,0,Motor_Run_Time);
//			delay(d_1s);
//			MotorStop();
//			classtest = 0;
//		break;
//
//		case 16:		//Bend, short
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			MotorMove(0,1,Motor_Run_Time);
//			delay(d_1s);
//			MotorStop();
//			classtest = 0;
//		break;
//
//		case 17:		//Extend, short
//			if (last_class != send_class)
//			{
//				MotorStop();
//				DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			}
//
//			MotorMove(0,2,Motor_Run_Time);
//			delay(d_1s);
//			MotorStop();
//			classtest = 0;
//		break;
//
//        default:		//Stop all outputs
//			MotorStop();
//			DAC_Output(0b11111111,DAC_WRITE_BUF_off);
//			classtest = 0;
//		break;
//	}//END of Switch statement
}


//***************************************//
void MotorMove(unsigned char motor1_cmd, unsigned char motor2_cmd, unsigned short motor_runtime)	//DEBUG PORTS
{
 switch(motor1_cmd)
 {
    case 0:
           H_BRG1A=0;
           H_BRG1B=0;
           break;
    case 1:
           H_BRG1A=1;
           H_BRG1B=0;
           break;
    case 2:
           H_BRG1A=0;
           H_BRG1B=1;
           break;
    default:
           break;
 }

 switch(motor2_cmd)
 {
    case 0:
           H_BRG2A=0;
           H_BRG2B=0;
           break;
    case 1:
           H_BRG2A=1;
           H_BRG2B=0;
           break;
    case 2:
           H_BRG2A=0;
           H_BRG2B=1;
           break;
    default:
           break;
 }

// initT5(motor_runtime);

}

//***************************************//
void MotorStop(void)		//DEBUG PORTS
{
           H_BRG1A=0;
           H_BRG1B=0;
           H_BRG2A=0;
           H_BRG2B=0;
		   IO1=0;
           IO2=0;
}

//***************************************//
void HandMove(unsigned char motor_cmd)
{
 switch(motor_cmd)
 {
    case 0:
           IO1=0;
           IO2=0;
           break;
    case 1:
           IO1=1;
           IO2=0;
           break;
    case 2:
           IO1=0;
           IO2=1;
           break;
    default:
           break;
 }
}
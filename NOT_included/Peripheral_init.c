//CX II peripheral initialization//
#include "p32mx795f512l.h"
#include "system.h"
#include "Port_init.h"
#include "plib.h"
#include "Glob_Var_Dec.h"


//*******************Function declerations *******************//
void initPORTs (void);

void initT1(unsigned short);
void disableT1(void);
void initT2(unsigned short);
void disableT2(void);
void initT3(unsigned short);
void disableT3(void);
void initT23(unsigned int);
void disableT23(void);
void initT4(unsigned short);
void disableT4(void);
void initT5(unsigned short);
void disableT5(void);
void initT45(unsigned int);
void disableT45(void);

void initSPI1_Master(void);  //RAM or other external periphral
void initSPI1_Master_EB(void);
void initSPI1_Master_EB_Disable(void);
void disableSPI1(void);

void initSPI3_Master(void);   //AFE
void initSPI3_Master_EB(void);
void initSPI3_Master_EB_Disable(void);
void disableSPI3(void);

void initUART2(unsigned short);
void disableUART2(void);

void initUART3(unsigned short);  //bluetooth
void disableUART3(void);

void initUART5(unsigned short);  //debug
void disableUART5(void);

void initINT1(void);  //AFE
void disableINT1(void);
void initINT2(void);  //External signal int
void disableINT2(void);


void initICN(void);
void disableICN(void);

void initRTCC(unsigned int, unsigned int);

void initI2C2(void);
void disableI2C2(void);
void initI2C1(void);


void initADC(void);
void disableADC(void);


//***************port initilization***********************//
void initPORTs (void)
{
 CS1_1=1; 
 CS1_2=1;
// AFE_RESET=1;
// H_BRG1A=0;   //H bridge control
// H_BRG1B=0;  
// H_BRG2A=0;   
// H_BRG2B=0; 

 SCK1_TRIS=1;  //memory
 SDI1_TRIS=1; 
 SDO1_TRIS=1; 
 CS1_1_TRIS=0; 
 CS1_2_TRIS=0; 

// SPI_DRDY_TRIS=1;   //AFE
// SPI_START_TRIS=0; 
// AFE_RESET_TRIS=0; 
 CS3_TRIS=0; 
 SCK3_TRIS=1; 
 SDO3_TRIS=1; 
 SDI3_TRIS=1; 

 T1CK_TRIS=1;   //32.768K clock
 T2CK_TRIS=1; 
 T3CK_TRIS=1; 

 U2RX_TRIS=1;   //UART2
 U2TX_TRIS=1; 

 U3RTS_TRIS=1;   //bluetooth
 U3RX_TRIS=1; //DEBUG PORT 
 U3TX_TRIS=1; 
 U3CTS_TRIS=1; 
//AUTOMSTR_BT_TRIS=1; 
// RESET_BT_TRIS=1; 
// GPIO2_BT_TRIS=1; 
// GPIO3_BT_TRIS=1; 	//DEBUG PORTS
//GPIO5_BT_TRIS=1; 

 U5RX_TRIS=1;  //debug port	
 U5TX_TRIS=1;

 IO1_TRIS=0;   //Digital IO port
 IO2_TRIS=0; 
 IO3_TRIS=0; 

 SCL1_TRIS=1;  //I2C1 (Slave)
 SDA1_TRIS=1; 

 SCL2_TRIS=1;   //I2C2 (Master)
 SDA2_TRIS=1; 

 C1RX_TRIS=1;   //CAN1
 C1TX_TRIS=1; 

 Vsense_TRIS=1;   //battery voltage sensing pin

 ACM_INT1_TRIS=1;   //ACM interrupt 1
 ACM_INT2_TRIS=1;   //ACM interrupt 2

 AN_IN1_TRIS=1;   //analog input
 AN_IN2_TRIS=1;  
 AN_IN3_TRIS=1;  
 AN_IN4_TRIS=1;  
 AN_IN5_TRIS=1;  
 AN_IN6_TRIS=1;  
 AN_IN7_TRIS=1;  
 AN_IN8_TRIS=1;  

 INT1_TRIS=1; 	//INT1 
 INT2_TRIS=1;   //INT2

 VibMotor_TRIS=0;   //vibration motor

 H_BRG1A_TRIS=0;   //H bridge control	//DEBUG PORTS
 H_BRG1B_TRIS=0;  
 H_BRG2A_TRIS=0;   
 H_BRG2B_TRIS=0; 

 LED1_R_TRIS=0;  //LED1,2
 LED1_G_TRIS=0;
 LED1_B_TRIS=0;
 LED2_R_TRIS=0;
 LED2_G_TRIS=0;
 LED2_B_TRIS=0;

 LED3_R_TRIS=0;  //LED3,4
 LED3_G_TRIS=0;
 LED3_B_TRIS=0;
 LED4_R_TRIS=0;
 LED4_G_TRIS=0;
 LED4_B_TRIS=0;


DAC_SYNC_TRIS=0;	//DAC
DAC_LDAC_TRIS=0;

DAC_SYNC=0;	//DEBUG
DAC_LDAC=0;
}


//*******************T1 configure***********************//
void initT1(unsigned short Timer_period)
{
    IFS0bits.T1IF=0;
    IPC1bits.T1IP=T1_PRIO;
    IPC1bits.T1IS=T1_SUBP;
    IEC0bits.T1IE=1;

    T1CON=0;
    TMR1=0;
    T1CONbits.TCKPS=0b00;
    T1CONbits.TCS=1;
    T1CONbits.TSYNC=1;
    PR1=Timer_period;  //32768/PR1 sample/s (128)
    T1CONbits.ON=1;
}

//******************T1 disable ************************//
void disableT1(void)  //disable timer 2
{
 T1CON=0; 
 TMR1=0;
 IFS0bits.T1IF=0;
 IEC0bits.T1IE=0;
}

//*******************T2 configure***********************//
void initT2(unsigned short Timer_period)
{
    T2CON=0;
    TMR2=0;
    T2CONbits.TCKPS=0b000;
    T2CONbits.TCS=1;
    PR2=Timer_period;  //period

    IFS0bits.T2IF=0;
    IPC2bits.T2IP=T2_PRIO;
    IPC2bits.T2IS=T2_SUBP;
    IEC0bits.T2IE=1;

    T2CONbits.ON=1;
}

//******************T2 disable ************************//
void disableT2(void)  //disable timer 2
{
 T2CON=0; 
 TMR2=0;
 IFS0bits.T2IF=0;
 IEC0bits.T2IE=0;
}

//*******************T3 configure***********************//
void initT3(unsigned short Timer_period)
{
/*
    T3CON=0;
    TMR3=0;
    T3CONbits.TCKPS=0b000;
    T3CONbits.TCS=1;
    PR3=Timer_period;  //period

    IFS0bits.T3IF=0;
    IPC3bits.T3IP=T3_PRIO;
    IPC3bits.T3IS=T3_SUBP;
    IEC0bits.T3IE=1;

    T3CONbits.ON=0; //DEBUG ON=1
*/

T3CON=0;
//T2CONbits.FRZ =0; // Continue operation when CPU is in Debug Exception mode  
T3CONbits.SIDL =1; // Continue operation In Idle mode 
T3CONbits.TGATE =0; //Gate time accumulation is disabled  
T3CONbits.TCKPS = 0b111; // 1:256 prescale value
T3CONbits.TCS =0; //Internal peripheral clock    

TMR3 = 0xF922;
PR3 =0xFFFF;

IEC0bits.T3IE = 1; // Enable Timer 3 overflow interrupt
IFS0bits.T3IF = 0; // Clear T3 Overflow interrupt flag
IPC3bits.T3IP = 2; //Timer 3 set to priority 5(highest priority) 
IPC3bits.T3IS =3; // Highest sub-priority
}

//******************T3 disable ************************//
void disableT3(void)
{
 T3CON=0;
 TMR3=0;
 IFS0bits.T3IF=0;
 IEC0bits.T3IE=0;
}

//******************T23 32 bit timer configure*************//
void initT23(unsigned int Timer_period)
{
    T2CON=0;        
    T3CON=0;
    T2CONbits.T32=1;       // T2/3 connected to a 32bit timer
    T2CONbits.TCKPS=0b111;
    T2CONbits.TCS=1;
    TMR2=0;
    TMR3=0;
    PR2=Timer_period;          //lower 16 bits of T23 period
    PR3=Timer_period>>16;      //higher 16 bits of T23 period

    IFS0bits.T3IF=0;
    IPC3bits.T3IP=T23_PRIO;
    IPC3bits.T3IS=T23_SUBP;
    IEC0bits.T3IE=1;

    T2CONbits.ON=0;
}

//******************T23 disable ************************//
void disableT23(void)
{
 IFS0bits.T2IF=0;
 IEC0bits.T2IE=0;
 IFS0bits.T3IF=0;
 IEC0bits.T3IE=0;
 T2CON=0;
 T3CON=0;
 TMR2=0;
 TMR3=0;
}

//*******************T4 configure***********************//
void initT4(unsigned short Timer_period)
{
    T4CON=0;
    TMR4=0;
    T4CONbits.TCKPS=0b111;
    PR4=Timer_period;  //period

    IFS0bits.T4IF=0;
    IPC4bits.T4IP=T4_PRIO;
    IPC4bits.T4IS=T4_SUBP;
    IEC0bits.T4IE=1;

    T4CONbits.ON=1;
}

//******************T4 disable ************************//
void disableT4(void)
{
 T4CON=0;
 TMR4=0;
 IFS0bits.T4IF=0;
 IEC0bits.T4IE=0;
}

//*******************T5 configure***********************//
//void initT5(unsigned short Timer_period)
//{
//    T5CON=0;
//    TMR5=0;
//    T5CONbits.TCKPS=0b111;
//    PR5=Timer_period;  //period
//
//    IFS0bits.T5IF=0;
//    IPC5bits.T5IP=T5_PRIO;
//    IPC5bits.T5IS=T5_SUBP;
//    IEC0bits.T5IE=1;
//
//    T5CONbits.ON=1;
//}

//******************T5 disable ************************//
//void disableT5(void)
//{
// T5CON=0;
// TMR5=0;
// IFS0bits.T5IF=0;
// IEC0bits.T5IE=0;
//}
//
//******************T45 32 bit timer configure*************//
void initT45(unsigned int Timer_period)
{
    T4CON=0;        
    T5CON=0;
    T4CONbits.T32=1;       // T4/5 connected to a 32bit timer
    T4CONbits.TCKPS=0b111;
    T4CONbits.TCS=0;
    TMR4=0;
    TMR5=0;
    PR4=Timer_period;          //lower 16 bits of T45 period
    PR5=Timer_period>>16;      //higher 16 bits of T45 period

    IFS0bits.T5IF=0;
    IPC5bits.T5IP=T45_PRIO;
    IPC5bits.T5IS=T45_SUBP;
    IEC0bits.T5IE=1;

    T4CONbits.ON=1;
}

//******************T45 disable ************************//
void disableT45(void)
{
 IFS0bits.T4IF=0;
 IEC0bits.T4IE=0;
 IFS0bits.T5IF=0;
 IEC0bits.T5IE=0;
 T4CON=0;
 T5CON=0;
 TMR4=0;
 TMR5=0;
}

//*******************SPI1 configure***********************//
void initSPI1_Master(void)
{
 SPI1CON = 0; // Stops and resets the SPI1.

 SPI1CONbits.FRZ=0;  //Freeze in Debug Exception Mode bit
 SPI1CONbits.SIDL=0;  //Stop in Idle Mode bit
 SPI1CONbits.DISSDO=0;   //Disable SDOx pin bit
 SPI1CONbits.MODE32=0;   //32Bit Communication Select bits
 SPI1CONbits.MODE16=0;   //16Bit Communication Select bits
 SPI1CONbits.SMP=0;
 SPI1CONbits.CKE=0;
 SPI1CONbits.SSEN=0;  //Slave select enable (Slave mode only). 1=!SSx pin is used for slave mode, 0=used for I/O
 SPI1CONbits.CKP=0;   //Clock polarity select. 1=idle state for clock is high, 0=idle is low
 SPI1CONbits.MSTEN=1; //1=master mode, 0=slave mode
 SPI1CONbits.STXISEL=0b00;
 SPI1CONbits.SRXISEL=0b00;

 SPI1CONbits.FRMEN=0;  //1=frame support enable,0=disable
 SPI1CONbits.FRMSYNC=0;  //Frame Sync Pulse Direction Control on SS pin 1=frame sync input(slave)
 SPI1CONbits.FRMPOL=0;  //Frame Sync Polarity bit
 SPI1CONbits.MSSEN=0;  //Master Mode Slave Select Enable bit
 SPI1CONbits.FRMSYPW=0;   //Frame Sync Pulse Width bit
 SPI1CONbits.SPIFE=0;  //Frame Sync Pulse Edge Select bit
 SPI1CONbits.ENHBUF=0;  //Enhanced Buffer Enable bit

 SPI1BRG=0x000; 
 SPI1STATCLR=0x40; // clear the Overflow
 SPI1CONbits.ON=1;  //SPI Peripheral On bit

 char temp_char;
 while (SPI1STATbits.SPIRBF)
 {
  temp_char=SPI1BUF;
 } 

}

//*******************SPI1 Enhanced buffer configure***********************//
void initSPI1_Master_EB(void)
{
 SPI1CONbits.ENHBUF=1;  //Enhanced Buffer Enable bit
 SPI1STATCLR=0x40; // clear the Overflow
}

//*******************SPI1 Enhanced buffer configure***********************//
void initSPI1_Master_EB_Disable(void)
{
 SPI1CONbits.ENHBUF=0;  //Enhanced Buffer Enable bit
 SPI1STATCLR=0x40; // clear the Overflow
}

//******************disable spi**********************//
void disableSPI1(void)
{
 SPI1CON = 0; // Stops and resets the SPI1.
}


//*******************SPI3 configure***********************//
void initSPI3_Master(void)
{
 SPI3CON = 0; // Stops and resets the SPI3.

 SPI3CONbits.FRZ=0;  //Freeze in Debug Exception Mode bit
 SPI3CONbits.SIDL=0;  //Stop in Idle Mode bit
 SPI3CONbits.DISSDO=0;   //Disable SDOx pin bit
 SPI3CONbits.MODE32=0;   //32Bit Communication Select bits
 SPI3CONbits.MODE16=0;   //16Bit Communication Select bits
 SPI3CONbits.SMP=0;
 SPI3CONbits.CKE=1;
 SPI3CONbits.SSEN=0;  //Slave select enable (Slave mode only). 1=!SSx pin is used for slave mode, 0=used for I/O
 SPI3CONbits.CKP=1;   //Clock polarity select. 1=idle state for clock is high, 0=idle is low
                      //ADS1298 requires the clock idle=low
 SPI3CONbits.MSTEN=1; //1=master mode, 0=slave mode
 SPI3CONbits.STXISEL=0b00;
 SPI3CONbits.SRXISEL=0b00;

 SPI3CONbits.FRMEN=0;  //1=frame support enable,0=disable
 SPI3CONbits.FRMSYNC=0;  //Frame Sync Pulse Direction Control on SS pin 1=frame sync input(slave)
 SPI3CONbits.FRMPOL=0;  //Frame Sync Polarity bit
 SPI3CONbits.MSSEN=0;  //Master Mode Slave Select Enable bit
 SPI3CONbits.FRMSYPW=0;   //Frame Sync Pulse Width bit
 SPI3CONbits.SPIFE=0;  //Frame Sync Pulse Edge Select bit
 SPI3CONbits.ENHBUF=0;  //Enhanced Buffer Enable bit

 SPI3BRG=0;    //
 SPI3STATCLR=0x40; // clear the Overflow
 SPI3CONbits.ON=1;  //SPI Peripheral On bit

 char temp_char;
 while (SPI3STATbits.SPIRBF)
 {
  temp_char=SPI3BUF;
 } 

}

//*******************SPI3 enhaced buffer configure***********************//
void initSPI3_Master_EB(void)
{
 SPI3CONbits.ENHBUF=1;  //Enhanced Buffer Enable bit
 SPI3STATCLR=0x40; // clear the Overflow
}

//*******************SPI3 enhaced buffer disable configure***********************//
void initSPI3_Master_EB_Disable(void)
{
 SPI3CONbits.ENHBUF=0;  //Enhanced Buffer Enable bit
 SPI3STATCLR=0x40; // clear the Overflow
}

//******************disable spi**********************//
void disableSPI3(void)
{
 SPI3CON = 0; // Stops and resets the SPI1.
}


//************************UART2 init********************************//
void initUART2(unsigned short baudrate)   // UART2 
{
 U2MODE=0;
 U2MODEbits.FRZ=0;
 U2MODEbits.SIDL=0;
 U2MODEbits.IREN=0;
 U2MODEbits.RTSMD=0;
 U2MODEbits.UEN=0b00;
 U2MODEbits.WAKE=1;
 U2MODEbits.LPBACK=0;
 U2MODEbits.ABAUD=0;
 U2MODEbits.RXINV=0;
 U2MODEbits.BRGH=1;
 U2MODEbits.PDSEL=0b00;
 U2MODEbits.STSEL=0;

 U2STA=0;
 U2STAbits.ADM_EN=0;
 U2STAbits.UTXINV=0;
 U2STAbits.URXEN=1;
 U2STAbits.UTXBRK=0;
 U2STAbits.UTXEN=1;
 U2STAbits.ADDEN=0;
 U2STAbits.OERR=0;
 U2STAbits.URXISEL=0b00;//Interrupt flag bit is set when receive buffer becomes full (i.e., has 4 data characters)
 U2BRG=baudrate;
/*DEBUG
 IFS1bits.U2RXIF=0;
 IPC8bits.U2IP=U2_PRIO;
 IPC8bits.U2IS=U2_SUBP;
 IEC1bits.U2RXIE=1;
*/
 U2MODEbits.ON=1;
}

//*****************disable uart*********************//
void disableUART2(void)
{
 U2MODE=0;
 U2STA=0;
 IFS1bits.U2RXIF=0;
 IEC1bits.U2RXIE=0;
}


//************************UART3 init********************************//
void initUART3(unsigned short baudrate)   // UART3 for bluetooth communications
{
 U3MODE=0;
 U3MODEbits.FRZ=0;
 U3MODEbits.SIDL=0;
 U3MODEbits.IREN=0;
 U3MODEbits.RTSMD=0;
 U3MODEbits.UEN=0b10;
 U3MODEbits.WAKE=1;
 U3MODEbits.LPBACK=0;
 U3MODEbits.ABAUD=0;
 U3MODEbits.RXINV=0;
 U3MODEbits.BRGH=1;
 U3MODEbits.PDSEL=0b00;
 U3MODEbits.STSEL=0;

 U3STA=0;
 U3STAbits.ADM_EN=0;
 U3STAbits.UTXINV=0;
 U3STAbits.URXEN=1;
 U3STAbits.UTXBRK=0;
 U3STAbits.UTXEN=1;
 U3STAbits.ADDEN=0;
 U3STAbits.OERR=0;
 
 U3BRG=baudrate;
// DEBUG turning off interrupts for now
 IFS1bits.U3RXIF=0;
 IPC7bits.U3IP=U3_PRIO;
 IPC7bits.U3IS=U3_SUBP;
 IEC1bits.U3RXIE=1;	

U3MODEbits.ON=1;
}

//*****************disable uart*********************//
void disableUART3(void)
{
 U3MODE=0;
 U3STA=0;
 IFS1bits.U3RXIF=0;
 IEC1bits.U3RXIE=0;
}

//************************UART5 init********************************//
void initUART5(unsigned short baudrate)   // UART5 for external computer communications, no hardware flow control
{
 #ifdef Debug_Mode  //if UART5EN is set
 U5MODE=0;
 U5MODEbits.FRZ=0;
 U5MODEbits.SIDL=0;
 U5MODEbits.IREN=0;

 U5MODEbits.WAKE=1;
 U5MODEbits.LPBACK=0;
 U5MODEbits.ABAUD=0;
 U5MODEbits.RXINV=0;
 U5MODEbits.BRGH=1;
 U5MODEbits.PDSEL=0b00;
 U5MODEbits.STSEL=0;

 U5STA=0;
 U5STAbits.ADM_EN=0;
 U5STAbits.UTXINV=0;
 U5STAbits.URXEN=1;
 U5STAbits.UTXBRK=0;
 U5STAbits.UTXEN=1;
 U5STAbits.ADDEN=0;
 U5STAbits.OERR=0;
 U5STAbits.URXISEL=0b00;//Interrupt flag bit is set when receive buffer becomes full (i.e., has 4 data characters)

 U5BRG=baudrate; 

 IFS2bits.U5RXIF=0;	//put in for I2C switch test
 IPC12bits.U5IP=U5_PRIO;
 IPC12bits.U5IS=U5_SUBP;
 IEC2bits.U5RXIE=1;


// IFS2bits.U5TXIF=0;
// IEC2bits.U5TXIE=0;
 
  U5MODEbits.ON=1;
 #endif
}

//*****************disable uart*********************//
void disableUART5(void)
{
 #ifdef Debug_Mode  //if it is in the debug mode
 U5MODE=0;
 U5STA=0;
 IFS2bits.U5RXIF=0;
 IEC2bits.U5RXIE=0;
 #endif
}


//****************************INT1 init*********************//
void initINT1(void)  // AFE int 
{
    INTCONbits.INT1EP=1;    // INT1 interrupt on negative edge 
    IFS0bits.INT1IF=0;
    IPC1bits.INT1IP=INT1_PRIO;   //priority
    IPC1bits.INT1IS=INT1_SUBP;   //sub-priority
    IEC0bits.INT1IE=1;
}

//***************************INT1 disable*************//
void disableINT1(void)
{
    IFS0bits.INT1IF=0;
    IEC0bits.INT1IE=0;
}

//****************************INT2 init*********************//
void initINT2(void)  //
{
    INTCONbits.INT2EP=0;    // INT2 interrupt on positive edge   
    IFS0bits.INT2IF=0;
    IPC2bits.INT2IP=INT2_PRIO;   //priority
    IPC2bits.INT2IS=INT2_SUBP;   //sub-priority
    IEC0bits.INT2IE=1;
}

//***************************INT2 disable*************//
void disableINT2(void)
{
    IFS0bits.INT2IF=0;
    IEC0bits.INT2IE=0;
}

//*******************ICN (Input change notification) configure***********************//
void initICN(void)
{
    CNCON=0x00008000;    //enable CN module

    CNEN= 0x000090F0;  //enable CN4,5,6,7,12,15

    CNPUE=0x00001000;

    CNI_portB_Buf=PORTB & 0x802C;   //read port value to clear the mismatch condition, with port mask
    CNI_portD_Buf=PORTD & 0x0040;   //read port value to clear the mismatch condition, with port mask

	IFS1bits.CNIF = 0;		// clear interrupt flag
	IPC6bits.CNIP = ICN_PRIO;		// set priority
	IPC6bits.CNIS = ICN_SUBP;		// set sub-priority
	IEC1bits.CNIE = 1;		// enable CN interrupt
}


//******************disable ICN********************//
void disableICN(void)
{
    CNCON=0;    //disable CN module
    CNEN=0;     //disable all CN pins
    CNPUE=0;    //disable weak pull up

	IFS1bits.CNIF = 0;		// clear interrupt flag
	IEC1bits.CNIE = 0;		// disable CN interrupt
}

//******************RTCC (real time clock and calendar) init*************//
void initRTCC(unsigned int InitDate, unsigned int InitTime)
{
 IEC1CLR=0x00008000;  //disable RTCC interrrupts
 RTCALRMCLR=0xFFFF;   //Disable alarm //clear ALRMEN, CHIME, AMASK and ARPT

 SYSKEY=0xAA996655;   //unlock the crucial register writting sequence lock
 SYSKEY=0x556699AA;   //unlock the crucial register writting sequence lock
 RTCCONCLR=0xFFFFFFFF;  //clear the RTCCON
 RTCCONSET=0x00000048;  //set the RTCCON value
 SYSKEY=0x0;          //lock the crucial register writting sequence lock

 while(RTCCON&0x40);  //wait for the RTCC clock to be turned off

 RTCTIME=InitTime;   //safe to update time to 16hr, 15min, 33sec
 RTCDATE=InitDate;   //updat the date to Friday 27 oct 2006

 T1CON=0;               //clear TxCON register
 TMR1=0;                //clear timer
 T1CONbits.TCKPS=0b10;  //timer input clock divider, 0b11=1/256
 T1CONbits.TCS=1;       //external clock input
 T1CONbits.TSYNC=1;     //sync with external clock input, 32.768 kHz
 IEC0bits.T1IE=0;       //disable Timer interrupts
 //T1 provide the miliseconde level time stamp. T1 interrupt will not be used in this firmware
 //TMR1 lower 8 bits will provide the time stamp with a unit=1/256 second

 T1CONbits.ON=1;        //Enable Timer 1

 RTCCONSET=0x00008000;   //start the RTCC

 RTCCONCLR=0x00000008;   //disable the RTCTIME and RTCDATE write in

 while(!(RTCCON&0x40));   //wait for the RTCC clock to be turned on
}

//****************************I2C2 init******************//

void initI2C2(void)
{

 I2C2CONCLR=0xFFFFFFFF;

 I2C2STATCLR=0xFFFFFFFF;


 //I2C2BRG=22;  //22 =400kHz 18MHz clock
I2C2BRG=88;  //88 =400kHz 72MHz clock
 //I2C2BRG=72;  //72 =100kHz 14.7456MHz clock
 //I2C1BRGCLR=0xFFFFFFFF;
 //I2C1BRGSET=35;  //35 =100kHz 7.3728MHz clock
 //I2C1BRG=7;  //7 =400kHz 7.3728MHz clock

 IFS1bits.I2C2MIF=0;
 IFS1bits.I2C2BIF=0;  //clear the collision flag
 IEC1bits.I2C2MIE=0;  //I2C2 master interrupt
 IPC8bits.I2C2IP=5;  //priority
 IPC8bits.I2C2IS=3;  //sub-priority

 I2C2CONSET=0x00008000;
//	I2C2CONbits.ON = 1;
}

//****************************disable I2C2******************//
void disableI2C2(void)
{
 I2C2CONCLR=0xFFFFFFFF;
 I2C2STATCLR=0xFFFFFFFF;

 IFS1bits.I2C2MIF=0;
 IFS1bits.I2C2BIF=0;  //clear the collision flag
 IEC1bits.I2C2MIE=0;  //I2C2 master interrupt
}

//****************************I2C1 init******************//

void initI2C1(void)
{
 
 I2C1CONCLR=0xFFFFFFFF;
 I2C1STATCLR=0xFFFFFFFF;

 //TRISEbits.TRISE8 = 1; //	SDA1 defined as input
// CNPUEbits.CNPUE11 = 1; //	SCL1 defined as input

// TRISAbits.TRISA15 = 1; //	SDA1 defined as input
// TRISAbits.TRISA14 = 1; //	SCL1 defined as input

	I2C1CONbits.ON = 0;
I2C1CONbits.FRZ = 0;
	I2C1CONbits.SIDL = 0;
	I2C1CONbits.SCLREL = 1;
I2C1CONbits.STRICT = 0;
	I2C1CONbits.A10M = 0;
	I2C1CONbits.DISSLW = 1;
	I2C1CONbits.SMEN = 0;
	I2C1CONbits.GCEN = 1;
	I2C1CONbits.STREN = 1;
	I2C1CONbits.ACKDT = 1;
	I2C1CONbits.ACKEN = 0;
	I2C1CONbits.RCEN = 0;
	I2C1CONbits.PEN = 0;
	I2C1CONbits.RSEN = 0;
	I2C1CONbits.SEN = 0;

//	IEC0bits.I2C1SIE=1;
	IFS0bits.I2C1SIF=0;  //I2C1 slave flag // ADDED

 IEC0bits.I2C1SIE=1;  //I2C1 slave interrupt // ADDED
 IFS0bits.I2C1MIF=0;
 IFS0bits.I2C1BIF=0;  //clear the collision flag
// IEC0bits.I2C1MIE=0;  //I2C1 master interrupt

 IPC6bits.I2C1IP=0b111;  //priority
 IPC6bits.I2C1IS=0b111;  //sub-priority

 I2C1ADDCLR = 0xFFFFFFFF;
 I2C1ADDSET = 0x75;
delay(20);
	I2C1CONbits.ON = 1;
}



//*******************ADC configure******************************//
void initADC(void)
{
 AD1PCFG=0x803F;  //select AN6,7,8,9,10,11,12,13,14 pin as the analog input
 AD1CON1=0x0000;
 AD1CHS=0x000E0000;  //AN14 pin to MUX A

 AD1CSSL=0;
 AD1CON3=0x002;
 AD1CON2=0;

 AD1CON1SET=0x8000;
}

//*******************disable ADC ******************************//
void disableADC(void)
{
 AD1CSSL=0;
 AD1CON1CLR=0xFFFF;
 AD1CON2CLR=0xFFFF;
 AD1CON3CLR=0xFFFF;
}


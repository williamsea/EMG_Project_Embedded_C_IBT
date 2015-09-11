//CX III port initialization//


//*****************define I/O control registers*****//
#define SCK1_TRIS _TRISD10  //memory
#define SDI1_TRIS _TRISC4
#define SDO1_TRIS _TRISD0
#define CS1_1_TRIS _TRISD9
#define CS1_2_TRIS _TRISD8

//#define SPI_DRDY_TRIS _TRISE8  //AFE
//#define SPI_START_TRIS _TRISB15
//#define AFE_RESET_TRIS _TRISF3
#define CS3_TRIS _TRISD14
#define SCK3_TRIS _TRISD15
#define SDO3_TRIS _TRISF8
#define SDI3_TRIS _TRISF2

#define T1CK_TRIS _TRISC14  //32.768K clock
#define T2CK_TRIS _TRISC1
#define T3CK_TRIS _TRISC2

#define U2RX_TRIS _TRISF4  //UART2
#define U2TX_TRIS _TRISF5

#define BUZZER_TRIS _TRISE7  //buzzer

#define U3RTS_TRIS _TRISG6  //UART 3 bluetooth
#define U3RX_TRIS _TRISG7
#define U3TX_TRIS _TRISG8
#define U3CTS_TRIS _TRISG9
//#define AUTOMSTR_BT_TRIS _TRISC15
//#define RESET_BT_TRIS _TRISC12
//#define GPIO2_BT_TRIS _TRISB5
//#define GPIO3_BT_TRIS _TRISD12	//DEBUG PORTS
//#define GPIO5_BT_TRIS _TRISB4

#define U5RX_TRIS _TRISF12  //UART 5 debug port
#define U5TX_TRIS _TRISF13

#define INT1_TRIS _TRISE8  //ext interrupt
#define INT2_TRIS _TRISE9

//#define IO1_TRIS _TRISA0  //Digital IO port
//#define IO2_TRIS _TRISA4
//#define IO3_TRIS _TRISA5
#define IO1_TRIS _TRISD5  
#define IO2_TRIS _TRISD6
#define IO3_TRIS _TRISD7

#define DAC_SYNC_TRIS _TRISA4	//DAC
#define DAC_LDAC_TRIS _TRISA5

#define SCL1_TRIS _TRISA14 //I2C1 (Slave)
#define SDA1_TRIS _TRISA15

#define SCL2_TRIS _TRISA2  //I2C2 (Master)
#define SDA2_TRIS _TRISA3

#define C1RX_TRIS _TRISF0  //CAN1
#define C1TX_TRIS _TRISF1

#define Vsense_TRIS _TRISB14  //battery voltage sensing pin

#define ACM_INT1_TRIS _TRISB3  //ACM interrupt 1
#define ACM_INT2_TRIS _TRISB2  //ACM interrupt 2

#define AN_IN1_TRIS _TRISB6  //analog input
#define AN_IN2_TRIS _TRISB7 
#define AN_IN3_TRIS _TRISB8 
#define AN_IN4_TRIS _TRISB9 
#define AN_IN5_TRIS _TRISB10 
#define AN_IN6_TRIS _TRISB11 
#define AN_IN7_TRIS _TRISB12 
#define AN_IN8_TRIS _TRISB13 

#define VibMotor_TRIS _TRISD11  //vibration motor

//#define H_BRG1A_TRIS _TRISG12	//DEBUG PORTS  //H bridge control
//#define H_BRG1B_TRIS _TRISG13 
//#define H_BRG2A_TRIS _TRISG14  
//#define H_BRG2B_TRIS _TRISG15 
#define H_BRG1A_TRIS _TRISB4	
#define H_BRG1B_TRIS _TRISB5
#define H_BRG2A_TRIS _TRISE5 
#define H_BRG2B_TRIS _TRISG15 

#define D_POS_TRIS _TRISG2  //buzzer
#define D_NEG_TRIS _TRISG3

#define LED1_R_TRIS _TRISG12  //LED1,2
#define LED1_G_TRIS _TRISE2
#define LED1_B_TRIS _TRISG13
#define LED2_R_TRIS _TRISE0
#define LED2_G_TRIS _TRISG14
#define LED2_B_TRIS _TRISE1

#define LED3_R_TRIS _TRISD12  //LED3,4
#define LED3_G_TRIS _TRISD4
#define LED3_B_TRIS _TRISD13
#define LED4_R_TRIS _TRISD1
#define LED4_G_TRIS _TRISD3
#define LED4_B_TRIS _TRISD2


//************define port function*****************//
#define SCK1 _LATD10  //memory
#define SDI1 _LATC4
#define SDO1 _LATD0
#define CS1_1 _LATD9
#define CS1_2 _LATD8

//#define SPI_DRDY _RE8  //AFE
//#define SPI_START _LATB15
//#define AFE_RESET _LATF3
#define CS3 _LATD14
#define SCK3 _LATD15
#define SDO3 _LATF8
#define SDI3 _LATF2

#define T1CK _LATC14  //32.768K clock
#define T2CK _LATC1
#define T3CK _LATC2

#define U2RX _LATF4  //UART 2
#define U2TX _LATF5

#define BUZZER _LATE7  //buzzer

#define U3RTS _LATG6  //bluetooth
#define U3RX _LATG7
#define U3TX _LATG8
#define U3CTS _LATG9
//#define AUTOMSTR_BT _LATC15
//#define RESET_BT _LATC12
//#define GPIO2_BT _LATB5
//#define GPIO3_BT _LATD12
//#define GPIO5_BT _LATB4

#define U5RX _LATF12  //UART 5 debug port
#define U5TX _LATF13

//#define IO1 _LATA0  //Digital IO port
//#define IO2 _LATA4
//#define IO3 _LATA5
#define IO1 _LATD5  
#define IO2 _LATD6
#define IO3 _LATD7

#define DAC_SYNC _LATA4	//DAC
#define DAC_LDAC _LATA5

#define SCL1 _LATA14 //I2C1 (Slave)
#define SDA1 _LATA15

#define SCL2 _LATA2  //I2C2 (Master)
#define SDA2 _LATA3

#define C1RX _LATF0  //CAN1
#define C1TX _LATF1

#define Vsense _RB14  //battery voltage sensing pin

#define ACM_INT1 _RB3  //ACM interrupt 1
#define ACM_INT2 _RB2  //ACM interrupt 2

#define AN_IN1 _RB6  //analog input
#define AN_IN2 _RB7 
#define AN_IN3 _RB8 
#define AN_IN4 _RB9 
#define AN_IN5 _RB10 
#define AN_IN6 _RB11 
#define AN_IN7 _RB12 
#define AN_IN8 _RB13 

#define INT1 _RE8  //ext interrupt
#define INT2 _RE9  

#define VibMotor _LATD11  //vibration motor

//#define H_BRG1A _LATG12  //H bridge control
//#define H_BRG1B _LATG13 
//#define H_BRG2A _LATG14  
//#define H_BRG2B _LATG15 
#define H_BRG1A _LATB4  
#define H_BRG1B _LATB5
#define H_BRG2A _LATE5  
#define H_BRG2B _LATG15 

#define LED1_R _LATG12  //LED1,2
#define LED1_G _LATE2
#define LED1_B _LATG13
#define LED2_R _LATE0
#define LED2_G _LATG14
#define LED2_B _LATE1

#define LED3_R _LATD12  //LED3,4
#define LED3_G _LATD4
#define LED3_B _LATD13
#define LED4_R _LATD1
#define LED4_G _LATD3
#define LED4_B _LATD2
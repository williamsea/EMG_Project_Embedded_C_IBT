//CX II global variable//

//*********************Interrupt Priorities (!!!important parameters!!!)*************************//
#define INT1_IPL ipl5   //
#define INT1_PRIO 5
#define INT1_SUBP 2

#define INT2_IPL ipl3   //
#define INT2_PRIO 3
#define INT2_SUBP 2

#define ICN_IPL ipl4    //all ICNs
#define ICN_PRIO 4
#define ICN_SUBP 2

#define T1_IPL ipl4     //Timer 1
#define T1_PRIO 4
#define T1_SUBP 2

#define T2_IPL ipl6     //Timer 2
#define T2_PRIO 6
#define T2_SUBP 2

#define T3_IPL ipl2     //Timer 3
#define T3_PRIO 2
#define T3_SUBP 3

#define T23_IPL ipl3     //Timer 2+3
#define T23_PRIO 3
#define T23_SUBP 3

#define T4_IPL ipl5     //Timer 4
#define T4_PRIO 5
#define T4_SUBP 3

//#define T5_IPL ipl3     //Timer 5
//#define T5_PRIO 3
//#define T5_SUBP 3

#define T45_IPL ipl4     //Timer 4+5
#define T45_PRIO 4
#define T45_SUBP 3

#define U2_IPL ipl3     //UART2
#define U2_PRIO 3
#define U2_SUBP 2

#define U3_IPL ipl5     //UART3, bluetooth
#define U3_PRIO 5
#define U3_SUBP 2

#define U5_IPL ipl6     //UART5, PC communication
#define U5_PRIO 6
#define U5_SUBP 2

#define RTCC_IPL ipl3   //RTCC
#define RTCC_PRIO 3
#define RTCC_SUBP 3

#define I2C2_IPL ipl3   //I2C2 master
#define I2C2_PRIO 3
#define I2C2_SUBP 3


#define DESIRED_BAUDRATE (115200)
#define	GetSystemClock() 			(80000000ul)
#define	GetPeripheralClock()		(GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()		(GetSystemClock())

//************address shift functions******************//
#define Lo(X)   (unsigned char)(X&0x00ff)
#define Mi(X)   (unsigned char)((X>>8)&0x00ff)
#define Hi(X)   (unsigned char)((X>>16)&0x00ff)

//************RTCC*******************************//
extern unsigned char RTCCtime[8];
#define InitialTime 0x23595000  //the factory initial time setting
#define InitialDate 0x12123107  //the factory initial date setting

//********************************************//
#define ALU_BaseSpeed (18000000UL)  //DEBUG, originally #define ALU_BaseSpeed (18000000UL)
#define ALU_BoostSpeed (72000000UL)

//************UART setting parameters********************//
    #define BR_9600 468    //Based on 18 MIPS core speed, 0.05% error
    #define BR_115200 38   //Based on 18 MIPS core speed, 0.16% error
    #define BR_460800 9    //based on 18 mips core speed, 2.4% error
    #define BR_921600 4    //based on 18 mips core speed, 2.4% error

    #define BR_9600_BST 1874    //Based on 72 MIPS core speed, 0% error
    #define BR_115200_BST 155   //Based on 72 MIPS core speed, 0.16% error
    #define BR_460800_BST 38    //based on 72 mips core speed, 0.16% error
    #define BR_921600_BST 19    //based on 72 mips core speed, 2.4% error


//*******************AFE data buffers**********************//
#define BufSize 30720  //60*256*2 bytes

extern unsigned char AFE_STATUS[];  //buffer for AFE status
extern char DATA_BUF[];
extern unsigned char TempBuf; //buffer register to damp any trash data

extern unsigned char DataReadyToSend;
extern char* BufDataEnd;       //end address of the data in the buffer

extern char* BufPointer_W;  //buffer write pointer
extern char* BufPointer_R;  //buffer read pointer
extern char* BufAddressStart;
extern char* BufAddressEnd;

//*******************Analog input reading********************//
extern unsigned short AN_DATABUF[8];

//**********************************************************//
//************************CNI port value buffer***********************//
extern unsigned short CNI_portB_Buf;
extern unsigned short CNI_portD_Buf;
//************************UART and SPI receive buffer**************************//
extern unsigned char U2Rx_Buf;
extern unsigned char U3Rx_Buf;
extern unsigned char U5Rx_Buf;

extern unsigned char SPI1Rx_Buf;
extern unsigned char SPI3Rx_Buf;

//*********************************ADS1298 register settings******************//

//EEG initialization
extern unsigned char config1_data;
extern unsigned char config2_data;
extern unsigned char config3_data;
extern unsigned char LOFF;
extern unsigned char CH1SET;
extern unsigned char CH2SET;
extern unsigned char CH3SET;
extern unsigned char CH4SET;
extern unsigned char CH5SET;
extern unsigned char CH6SET;
extern unsigned char CH7SET;
extern unsigned char CH8SET;
extern unsigned char RLD_SENSP;
extern unsigned char RLD_SENSN;
extern unsigned char LOFF_SENSP;
extern unsigned char LOFF_SENSN;
extern unsigned char LOFF_FLIP;
extern unsigned char LOFF_STATP;
extern unsigned char LOFF_STATN;
extern unsigned char GPIO;
extern unsigned char PACE;
//extern unsigned char RESP;
extern unsigned char config4_data;
extern unsigned char WCT1_data;
extern unsigned char WCT2_data;

//EKG initialization
extern unsigned char config1_data_C;
extern unsigned char config2_data_C;
extern unsigned char config3_data_C;
extern unsigned char LOFF_C;
extern unsigned char CH1SET_C;
extern unsigned char CH2SET_C;
extern unsigned char CH3SET_C;
extern unsigned char CH4SET_C;
extern unsigned char CH5SET_C;
extern unsigned char CH6SET_C;
extern unsigned char CH7SET_C;
extern unsigned char CH8SET_C;
extern unsigned char RLD_SENSP_C;
extern unsigned char RLD_SENSN_C;
extern unsigned char LOFF_SENSP_C;
extern unsigned char LOFF_SENSN_C;
extern unsigned char LOFF_FLIP_C;
extern unsigned char LOFF_STATP_C;
extern unsigned char LOFF_STATN_C;
extern unsigned char GPIO_C;
extern unsigned char PACE_C;
//extern unsigned char RESP_C;
extern unsigned char config4_data_C;
extern unsigned char WCT1_data_C;
extern unsigned char WCT2_data_C;

//**************define MR25H40 RAM op code********//

#define SRAM_READ  0b00000011   //Read data from memory array beginning at selected address
#define SRAM_WRITE 0b00000010   //Write data to memory array beginning at selected address
#define SRAM_RDSR  0b00000101   //Read STATUS register
#define SRAM_WRSR  0b00000001   //Write STATUS register

#define SRAM_WREN  0b00000110   //write enable
#define SRAM_WRDI  0b00000100   //Write disable

#define SRAM_MODE_BYTE 0b00000001   //SRAM byte mode, hold function disable
#define SRAM_MODE_PAGE 0b10000001   //SRAM page mode, hold function disable
#define SRAM_MODE_SEQT 0b01000001   //SRAM sequential mode, hold function disable



//*********************************ADS1298 register address******************//
#define config1_addr 0b01000001
#define config2_addr 0b01000010
#define config3_addr 0b01000011
#define LOFF_addr 0b01000100
#define CH1SET_addr 0b01000101
#define CH2SET_addr 0b01000110
#define CH3SET_addr 0b01000111
#define CH4SET_addr 0b01001000
#define CH5SET_addr 0b01001001
#define CH6SET_addr 0b01001010
#define CH7SET_addr 0b01001011
#define CH8SET_addr 0b01001100
#define RLD_SENSP_addr 0b01001101
#define RLD_SENSN_addr 0b01001110
#define LOFF_SENSP_addr 0b01001111
#define LOFF_SENSN_addr 0b01010000
#define LOFF_FLIP_addr 0b01010001
#define LOFF_STATP_addr 0b01010010
#define LOFF_STATN_addr 0b01010011
#define GPIO_addr 0b01010100
#define PACE_addr 0b01010101
#define RESP_addr 0b01010110
#define config4_addr 0b01010111
#define WCT1_addr 0b01011000
#define WCT2_addr 0b01011001

#define CMD_start_convert writeSPI2(0x08); writeSPI2(0);
#define CMD_stop_read writeSPI2(0x11); writeSPI2(0);
#define CMD_read_data writeSPI2(0x12); //read data
//**********************************************************************//


//*********************filter******************************//

#define FilterLength 128

extern unsigned int HPF_Buf_H[8];   //high pass filter value

extern unsigned short DATA_BUF_16b[8][FilterLength+1];

extern unsigned int Buf_16b_Index;
extern unsigned int Buf_16b_Index_Last;

extern unsigned int Samp_Value_temp;


//**************************I2C******************************//
#define I2C_TimeOut 0x08FF    //the time out limit for I2C1 operations


//***************************Accelerometer************************//
#define ACMAddress_W 0x38  //7 bit address=0x1C plus the R/W bit =0
#define ACMAddress_R 0x39  //7 bit address=0x1C plus the R/W bit =1
extern unsigned char ACM_STATYS_BUF;
extern unsigned short ACM_DATA_BUF[3];
extern unsigned char ACM_ORIENTATION_BUF;
extern unsigned char ACM_MOTION_BUF;
extern unsigned char ACM_TRANS_BUF;
extern unsigned char ACM_MODE_BUF;
extern unsigned char ACM_INT_BUF;
extern unsigned char ACM_RESULT;  //the accelerometer measuremnt result

//************************Dec number to Hex number*****************************//
#define DtoH_10E9 0x3B9ACA00   // 1,000,000,000 = 0x3B9ACA00
#define DtoH_10E8 0x05F5E100
#define DtoH_10E7 0x00989680
#define DtoH_10E6 0x000F4240
#define DtoH_10E5 0x000186A0
#define DtoH_10E4 0x00002710
#define DtoH_10E3 0x000003E8
#define DtoH_10E2 0x00000064
#define DtoH_10E1 0x0000000A


//*********************delay period constants for delay() function*****************//
//based on 18MHz main clock
#define d_10us  0x00000012  //10 us
#define d_20us  0x00000024  //20 us
#define d_50us  0x0000005A  //50 us
#define d_100us 0x000000B5  //100 us
#define d_200us 0x0000016A  //200 us
#define d_500us 0x00000389  //500 us
#define d_1ms   0x00000713  //1 ms
#define d_2ms   0x00000E26  //2 ms
#define d_5ms   0x00002360  //5 ms
#define d_10ms  0x000046C0  //10 ms
#define d_20ms  0x00008D80  //20 ms
#define d_50ms  0x000161C0  //50 ms
#define d_100ms 0x0002C380  //100 ms
#define d_200ms 0x00058700  //200 ms
#define d_500ms 0x000DD184  //500 ms
#define d_1s    0x001BA308  //1 s
#define d_2s    0x00374610  //2 s
#define d_5s    0x008A2F28  //5 s
#define d_10s   0x01145E50  //10 s
#define d_20s   0x0228BCA0  //20 s
#define d_50s   0x0565D790  //50 s
#define d_100s  0x0ACBAF20  //100 s

// FOLLOWING WAS ADDED TO ORIGINAL CODE: 

//*****************FeatureExtraction*******************************//

# define num_features 24  //used for calculating features

extern unsigned int mav_data[8]; 	//calculated average for feature matrix
extern unsigned int var_data[8];	//calculated variance for feature matrix
extern unsigned int wav_data[8];	//calculated waveform length for feature matrix
extern unsigned int mean_data[8];	//calculated mean of the signal for subtracting from variance

#define FeatureLength 350			//buffer length for calculating feature matrix (not using all the data)

extern unsigned int MAV_Buf[8];   //sum for mean absolute value
extern unsigned int mean_Buf[8]; // sum for signal mean

extern unsigned short Data_Buf[8][FeatureLength]; //stores last 350 data points from EMG channels
extern unsigned short Data_Rect_Buf[8][FeatureLength]; //stores last 350 rectified data points from EMG channels

extern unsigned int Var_Buf[8];   //sum for variance

extern unsigned short Data_Var_Buf[8][FeatureLength]; //buffer used to calculate variance

extern unsigned int Feat_Buf_Index;				//current filling index of moving buffer
extern unsigned int Feat_Buf_Index_Prev;		//last filling index of moving buffer for comparisons

extern unsigned int feature_mx [24];
extern unsigned char feature_tx [96];

//******************LDA*********************************************//

# define buff_length 5000 //2400(25*24*4) M bytes rest for padding + 2304 S bytes rest for padding = 5000

extern int num_classes; 
extern double M[25][25];
extern double S[25][25];
extern double PRIOR[50];
extern unsigned char mx_map[50];
extern unsigned char last_class;

//*************************main data processing*************************//
extern unsigned int start_process; //start processing data after enough is collected
extern unsigned int finish_process; //prevents processing from occurring again before it is finished

extern unsigned int Current_MAV_Buf[8]; //saving mav buffer for processing
extern unsigned int Current_Var_Buf[8]; //saving var buffer for processing
extern unsigned int Current_Wav_Buf[8]; //saving wav buffer for processing
extern unsigned int Current_mean_Buf[8]; //saving mean buffer for processing

#define MovingBufLength 200 //actual amount of data being processed
#define timerDiff 10 //minimum number of times timer 1 must fire before beginning feature calculations again

//**********************Analog or BeBionic************************//
extern char isBeBionic;

//************************GUI communication flags************************//
extern char training;
extern char signalViewer;
extern char sendLDA;
extern char stopSequence;
extern char disconnect;

//*****************************M and S status********************************//
//extern int M_status;
//extern int S_status;		//m and s check

//**********************Boolean Declaration**********//
typedef int bool;    				//initialization for boolean logic
# define true 1
# define false 0

# define Debug_Mode

//************************Handshaking************//
extern unsigned int sequence_transmission[4];
extern volatile unsigned int handshaketestflag;

//***************************DAC***********************************//
#define DACAddress_W 0x18  //
#define DACAddress_R 0x19  //
#define DACAddress_W2 0x1A  //
#define DACAddress_R2 0x1B  //

extern unsigned char DAC_WRITE_BUF_on[6];
extern unsigned char DAC_WRITE_BUF_off[6];
extern unsigned char DAC_READ_BUF[8];

extern unsigned short Motor_Run_Time;


//********************************Filtering******************************
#define class_opt 25 //number of classes available

//majority filter variables
extern char majority; //flag for using majority filter
extern char maj_buff_size; //size of buffer you are checking
extern char maj_buff_cut; //cut-off for the majority
extern char maj_buff[50]; //buffer that holds previous classes, can make larger if desired, but have to change GUI
extern char maj_counter[class_opt];
 
extern char filt_index;//keeps track of location in aon_buff and maj_buff
extern char oldest_class; //the class you are getting rid of (must decrement counter)

extern unsigned char get;
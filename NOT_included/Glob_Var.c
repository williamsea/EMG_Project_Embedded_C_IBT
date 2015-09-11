//CX II AFE global variables//
#include "Glob_Var_Dec.h"

//*******************AFE data buffers**********************//

unsigned char AFE_STATUS[3]={0x00,0x00,0x00};  //buffer for AFE status
char  DATA_BUF[BufSize+73];
unsigned char TempBuf; //buffer register to damp any trash data

unsigned char DataReadyToSend=0;
char* BufDataEnd;       //end address of the data in the buffer

char* BufPointer_W=DATA_BUF;  //buffer write pointer
char* BufPointer_R=DATA_BUF;  //buffer read pointer
char* BufAddressStart=DATA_BUF;
char* BufAddressEnd=&DATA_BUF[BufSize-1];

//*******************Analog input reading********************//
unsigned short AN_DATABUF[8]={0,0,0,0,0,0,0,0};

//***********************filter******************************//
unsigned short DATA_BUF_16b[8][FilterLength+1];

unsigned int Buf_16b_Index=0;
unsigned int Buf_16b_Index_Last=1; //the index of the earliest point in the buffer

unsigned int HPF_Buf_H[8]={0,0,0,0,0,0,0,0};   //high pass filter value

unsigned int Samp_Value_temp=0;

//************RTCC*******************************//
unsigned char RTCCtime[8];
//************************CNI port value buffer***********************//
unsigned short CNI_portB_Buf;
unsigned short CNI_portD_Buf;
//************************UART and SPI receive buffer**************************//
unsigned char U2Rx_Buf=0;
unsigned char U3Rx_Buf=0;
unsigned char U5Rx_Buf=0;

unsigned char SPI1Rx_Buf=0;
unsigned char SPI3Rx_Buf=0;

//*********************************ADS1298 register settings******************//

//EMG initialization
unsigned char config1_data=0b11100011;
unsigned char config2_data=0b00100010;
unsigned char config3_data=0b11000000;
unsigned char LOFF=0b00010001;
unsigned char CH1SET=0b01100000;
unsigned char CH2SET=0b01100000;
unsigned char CH3SET=0b01100000;
unsigned char CH4SET=0b01100000;
unsigned char CH5SET=0b01100000;
unsigned char CH6SET=0b01100000;
unsigned char CH7SET=0b01100000;
unsigned char CH8SET=0b01100000;
unsigned char RLD_SENSP=0b00000110;
unsigned char RLD_SENSN=0b00000010;
unsigned char LOFF_SENSP=0xff;
unsigned char LOFF_SENSN=0xff;
unsigned char LOFF_FLIP=0;
unsigned char LOFF_STATP=0;
unsigned char LOFF_STATN=0;
unsigned char GPIO=0;
unsigned char PACE=0;
//unsigned char RESP=0;
unsigned char config4_data=0b00001010;
unsigned char WCT1_data=0b00000000;
unsigned char WCT2_data=0b00000000;

//EKG initialization
unsigned char config1_data_C=0b11100110;
unsigned char config2_data_C=0b00100010;
unsigned char config3_data_C=0b11001100;
unsigned char LOFF_C=0b00010011;
unsigned char CH1SET_C=0b01100000;
unsigned char CH2SET_C=0b01100000;
unsigned char CH3SET_C=0b01100000;
unsigned char CH4SET_C=0b01100000;
unsigned char CH5SET_C=0b01100000;
unsigned char CH6SET_C=0b01100000;
unsigned char CH7SET_C=0b01100000;
unsigned char CH8SET_C=0b01100000;
unsigned char RLD_SENSP_C=0b00000110;
unsigned char RLD_SENSN_C=0b00000010;
unsigned char LOFF_SENSP_C=0xff;
unsigned char LOFF_SENSN_C=0xff;
unsigned char LOFF_FLIP_C=0;
unsigned char LOFF_STATP_C=10;
unsigned char LOFF_STATN_C=10;
unsigned char GPIO_C=0;
unsigned char PACE_C=0;
//unsigned char RESP_C=0;
unsigned char config4_data_C=0b00001010;
unsigned char WCT1_data_C=0b00001011;
unsigned char WCT2_data_C=0b11010100;

//*************************Accelerometer*******************************************//
unsigned char ACM_STATYS_BUF=0;
unsigned short ACM_DATA_BUF[3]={0,0,0};
unsigned char ACM_ORIENTATION_BUF=0;
unsigned char ACM_MOTION_BUF=0;
unsigned char ACM_TRANS_BUF=0;
unsigned char ACM_MODE_BUF=0;
unsigned char ACM_INT_BUF=0;
unsigned char ACM_RESULT=0;  //the accelerometer measuremnt result
                             //0b1xxxxxx : a motion is detected, 0b0xxxxxxx: no motion is detected
                             //0bxxxx000~0bxxxxx111: the orientaion info


//*****************FeatureExtraction*******************************//

unsigned int avg_data[8]={0,0,0,0,0,0,0,0};
unsigned int var_data[8]={0,0,0,0,0,0,0,0};
unsigned int wav_data[8]={0,0,0,0,0,0,0,0};

unsigned int avg_data_sum = 0;
unsigned int thresh_max[20] = {0};
unsigned int thresh_min[20] = {0};

unsigned short Data_Rect_Buf[8][FeatureLength]; 

unsigned int Avg_Buf[8]={0,0,0,0,0,0,0,0};   

unsigned short Data_Var_Buf[8][FeatureLength]; 

unsigned int Var_Buf[8]={0,0,0,0,0,0,0,0};   

unsigned short Data_Buf[8][FeatureLength]; 

unsigned int Feat_Buf_Index=0;
unsigned int Feat_Buf_Index_Prev=100;

unsigned short feature_mx [24];
unsigned short feature_tx [48];
//***********LDA**********************************************//

int num_classes; 
float M[25][25];
float S[25][25];
double PRIOR[50];
unsigned char mx_map[50];


//*************************main data processing*************************//
unsigned int start_process=0;
unsigned int finish_process=1;

unsigned int Current_Avg_Buf[8]={0,0,0,0,0,0,0,0};
unsigned int Current_Var_Buf[8]={0,0,0,0,0,0,0,0};
unsigned int Current_Wav_Buf[8]={0,0,0,0,0,0,0,0};

//**********************Analog or BeBionic************************//
char isBeBionic = false; //true for BeBionic hand (enable I2C)
						//of false for analog hand (disable I2C)

//************************Handshaking************//
unsigned int sequence_transmission[4] = {0,0,0,0};

volatile unsigned int handshaketestflag = 1;

//********************DAC*******************//
unsigned char DAC_WRITE_BUF_on[8]={255,255,255,255,255,255,255,255}; //3V*x/255 (Anne)
//unsigned char DAC_WRITE_BUF_on[6]={200,200,200,200,200}; //3V*x/255
//unsigned char DAC_WRITE_BUF_on[6]={100,100,100,100,100,100}; //Dana
unsigned char DAC_WRITE_BUF_off[8]={0,0,0,0,0,0,0,0};
unsigned char DAC_READ_BUF[8]={0,0,0,0,0,0,0,0};

unsigned short Motor_Run_Time=0x0100;

//********************I2C*******************//
Action hand_action = IDLE;
Action prev_hand_action = IDLE;

unsigned int tx_VIBRATE[3] = {0x1B, 0x14, 0xFF};

unsigned int txO_PINCH_UO_KEY[3] = {0x0C, 0xFE, 0xFF}; // { Command, Buffer, Buffer }
unsigned int txO_POWER_UO_FINGER_POINT[3] = {0x0D, 0xFE, 0xFF};
unsigned int txO_PRECOPEN_UO_MOUSE[3] = {0x0E, 0xFE, 0xFF};
unsigned int txO_TRIGGER_UO_COLUMN[3] = {0x0F, 0xFE, 0xFF};
unsigned int txO_PRECCLOSE_UO_RELAXED[3] = {0x14, 0xFE, 0xFF};

unsigned char I2C_sent = false;



//***********************keyboard control**************************//
char get=0;
char classtest=0;
char last_class=0;
unsigned char DAC_WRITE_BUF_onAll[8]={255,255,255,255,255,255,255,255};
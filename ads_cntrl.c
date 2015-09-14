#include <xc.h>
#include <plib.h>
#include "ads1291.h"
#include <inttypes.h>
#include <math.h>
#include "Port_init.h"

// pic32mx795f512l
#pragma config FNOSC=FRCPLL // clock source = internal fast rc with pll
                            // internal rc osc base freq = 8 MHz
#pragma config FPLLIDIV=DIV_2, FPLLMUL=MUL_20, FPLLODIV=DIV_1 // system clock
                                                              // f = 8/2 * 20 / 1 = 80 Mhz
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (ICE EMUC1/EMUD1 pins shared with PGC1/PGD1)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

/********************/
/* Global Variables */
/********************/

/*Changeable Variables*/
static volatile int txEnvelop = 1; // 1 to transmit envelop signal, 0 to transmit raw signal
static volatile int WindowLength = 100;//50,100,200,300,400,500;
static volatile int window[100];//[100];//window and WindowLength
static volatile int spikeCutoffVal = 20000; //for gain 6 //5000; for gain 1 //Set raw signal under threshold to peak
static volatile int rawSignalPeak = 20000;
static volatile int envelopPeak = 5000; //2k to 5k for gain = 6 //800; for gain = 1 //1400;//Used for EMG signal normalization to 0-255
static volatile int baselineThresh = 256;//get rid of the EMG baseline noises in envelop

/* Variables For Envelop Filter */
static volatile int envelop=0;
static volatile int DACenvelop = 0;
static volatile int DigitizedEnvelop = 0;
static volatile int digitizeLevel = 100;//77;
static volatile double stepSize = 2.56;//for 256/100  //3.32467532; for 256/77, for digitizing the DACenvelop signal (0-255)
static volatile int id = 0;
static volatile int i=0;
static volatile int resultPrev = 0;
static volatile int integral = 0;

/* Variables used to get data from ADS chip */
static volatile unsigned char temp;
static volatile int emg_data=0;
static volatile int result=0;

///* Variables for I2C2 interrupt */
//unsigned char CommandCount=0;
//unsigned char CmdBuf_temp[8];
//unsigned char prog_counter=0;  

#define MAX_ADS_OUT 8388607     // 2^23-1 i.e. the max 24-bit 2s complement #   
#define MIN_ADS_OUT -8388608    // -2^23 i.e. the min 24-bit 2s complement #

/* Variables to implement the high pass filter */
static volatile int prev_result=0; // y[n-1]]
static volatile int hpf_input = 0; // x[n]
static volatile int hpf_input_prev=0; // x[n-1]]

/* Variables used to implement the notch filter */
#define NOTCH_N     2   // The number of cascaded second-order filters 
                        // making up the notch filter 
#define NOTCH_Q     31  // The  number of fractional bits in 
                        // the notch filter coefficients
static volatile long long s_notch[NOTCH_N][2] = {0};    // notch filter internal 
                                                        // states must be held
                                                        // in memory in between 
                                                        // calls to the filter
/*************/
/* Functions */
/*************/

void delay(int delay ){
    int tick=(80000000/1000000)/2;
    WriteCoreTimer( 0 );
    while( ReadCoreTimer() < delay*tick );
}
void utx(unsigned char data){
    while( U2STAbits.UTXBF);
    U2TXREG=data;
}

int HighPass_filter_IIR_Hai(int new_data,int prev_data, int prev_output){
    /********************************************************************/
    /* First order butterworth filter                                   */
    /* implements the equations:                                        */
    /* y[i] = b_0*x[i] + b_1*x[i-1] - a_1*y[i-1]                        */
    /* Filter coefficients obtained with the following lines in MATLAB  */
    /* Fc = 30;                                                         */
    /* Fs = 3e3;                                                        */
    /* N = 32;                                                          */
    /* [bhat,ahat] = butter(1,Fc/(Fs/2),'high');                        */
    /* b = round(bhat*2^(N-1));                                         */
    /* a = round(ahat*2^(N-1));                                         */
    /* Since b_0 = -1*b_1 we only store a single coefficient below      */
    /* Note: coefficients derived when using 3kHz sampling, with 1 kHz  */
    /* sampling rate Fc becomes 10/3, which is still acceptable         */
    /* for baseline drift removal                                       */
    /********************************************************************/

//    //Fc=10
//    int b = 2125227504;
//    int a = -2102971360;

    //Fc=30, cutoff = 10Hz
    int b =  2082052512;
    int a = -2016621376;

//    //Fc=50
//    int b =  2040543305;
//    int a = -1933602962;

//    //Fc=60
//    int b =  2020372579;
//    int a = -1893261511;

//    //Fc=80
//    int b =  1981124016;
//    int a = -1814764385;

//    //Fc=90
//    int b =  1962018312;
//    int a = -1776552976;
    
    int delta;
    long long in_prod;
    long long out_prod;
    long long yM;
    int y;
    delta = new_data-prev_data;
    in_prod = (long long) b*delta;
    out_prod = (long long) a*prev_output;
    yM = in_prod - out_prod;
    y = yM >> 31;
    return(y);
}

int Notch_filter_IIR(int new_data){
    /************************************************************************/    
    /* 4th Order IIR notch filter implemented as a cascade of "NOTCH_N"     */
    /* biquad (2nd order) filters in Transposed-Direct-Form-II              */
    /* Each stage satisfies the following equation:                         */
    /* y[n] = b_0*x[n] + b_1*x[n-1] + b_2*x[n-2] - a_1*y[n-1] - a_2*y[n-2]  */
    /* Filter coefficients obtained with the following lines in MATLAB:     */
    /* Fs = 1e3;   % sampling frequency                                     */
    /* N = 4;      % filter order (must be even)                            */
    /* F0 = 60;    % center frequency                                       */
    /* Q = 8;     % quality factor                                          */
    /* Dparam = fdesign.notch('N,F0,Q',N,F0/(Fs/2),Q);                      */
    /* HN = design(Dparam,'FilterStructure','df1sos','SystemObject',true);  */
    /* sosMatrix = HN.SOSMatrix;                                            */
    /* sclValues = HN.ScaleValues;                                          */
    /* b = repmat(sclValues(1:end-1),1,3).*sosMatrix(:,(1:3));              */
    /* a = sosMatrix(:,(5:6));                                              */
    /* %% Quantize coefficients:                                            */
    /* QC = 31; % number of fractional bits in coefficients                 */
    /* b = int32(b*2^(QC-1));                                               */
    /* a = int32(a*2^(QC-1));                                               */
    /************************************************************************/
    /* Notch filter coefficients */
    static volatile int b_notch[NOTCH_N][3] = {{1056000713,-1963688804,1056000361},
                                              { 1056000713,-1963689070,1056000361}};
    static volatile int a_notch[NOTCH_N][2] =   {{-1949090498,1037096788},
                                                { -1977759923,1040010350}};
    int i;
    int x;
    int y;
    long long yM;
    x = new_data; // new data becomes input to first stage
    for(i=0; i<NOTCH_N; i++){
        // Output of biquad stage i:
        yM = (long long)b_notch[i][0]*x + s_notch[i][0];
        y = (int) (yM>>(NOTCH_Q-1));
        // Update internal states of stage i
        s_notch[i][0] = (long long) b_notch[i][1]*x - (long long)a_notch[i][0]*y + s_notch[i][1];
        s_notch[i][1] = (long long) b_notch[i][2]*x - (long long) a_notch[i][1]*y;
        // output of this stage becomes input to next stage
        x = y;
    }   
    return(y);
}

void init_spi1(void){
    _TRISD9=0;
    _RD9=1;
    SPI1CON=0x8020;
    SPI1BRG=79;
}

unsigned char spi_tx(unsigned char data){
    SPI1BUF=data;
    while(!SPI1STATbits.SPIRBF);
    return SPI1BUF;
}

void init_ads1291(void){
_RD9=0;
delay(100);
_RD9=0;

spi_tx(ADS_CMD_RESET); delay(10); //RESET BYTE

_RD9=1; delay(10);
delay(50065);


_RD9=0;
spi_tx(ADS_CMD_SDATAC); delay(10); //Throw SDATAC flag (stop continuous acquisition)
_RD9=1; delay(10);

// Write to CONFIG2
_RD9=0;
spi_tx(ADS_CMD_WRITEREG | ADS_REG_CONFIG2); delay(10);//CONFIG2
spi_tx(0x00); delay(10); // writing 1 byte
spi_tx(LEAD_OFF_DISABLED | REF_BUFFER_ENABLED | VREF_2V | OSC_CLK_OFF | INT_TEST_SIG_OFF | INT_TEST_SIG_DC); delay(10);
_RD9=1;

// Write to CONFIG1
_RD9=0;
spi_tx(ADS_CMD_WRITEREG | ADS_REG_CONFIG1); delay(10);
spi_tx(0x00); delay(10);
spi_tx(SINGLE_SHOT_MODE | DATA_RATE_4000); delay(10);
_RD9=1; delay(10);

// Write to CH1SET
_RD9=0;
spi_tx(ADS_CMD_WRITEREG | ADS_REG_CH1SET); delay(10);
spi_tx(0x00); delay(10);
spi_tx(PWR_UP | GAIN_6| ELECT_IN); delay(10);
_RD9=1; delay(10);

// Write to CH2SET - power down channel
_RD9=0;
spi_tx(ADS_CMD_WRITEREG | ADS_REG_CH2SET); delay(10);
spi_tx(0x00); delay(10);
spi_tx(PWR_DWN | GAIN_6 | ELECT_IN); delay(10);
_RD9=1; delay(10);

// Start ADS sampling
_RD9=0;
spi_tx(ADS_CMD_START_CONV); delay(10);
_RD9=1; delay(10);

//Throw RDATAC Flag (enable continuous read)
_RD9=0;
spi_tx(ADS_CMD_RDATAC); delay(10); 
_RD9=1; delay(10);

//Throw SDATAC Flag (stop continuous read)
_RD9=0;
spi_tx(ADS_CMD_SDATAC); delay(10); 
_RD9=1; delay(10);

// Start ADS Conversions
_RD9=0;
spi_tx(ADS_CMD_START_CONV); delay(10); //START ADS
_RD9=1; delay(10);

SPI2BRG=2;
}

void init_uart2(void){
    U2BRG = 19;
    U2STA = 0;
    U2MODE = 0x8000;
    U2STASET = 0x1400;
}

void init_timer1_intRC(void){
    // Set up timer to generate a 1k clock
    T1CON = 0x0;
    TMR1 = 0x0;
    PR1 = 1250;            // timer compare register set to 625
                          // interval depends on clock source and pre/post scalers
                          // interval = fclk / (PR1)
    IPC1SET = 0x0000000C; // set bits 3 and 2 in reg ipc1
                          //interrupt priority (timer 1 interrupt priority set to 011
    IPC1SET = 0x00000001; // set bit 0 in ipc1
                          // sub priority for timer 1 interrupt = 1
    IFS0CLR = 0x00000010; // clears bit 7 in the ifso register (t1 interrupt flag)
    IEC0SET = 0x00000010; // set bit 7 of ieco register (timer 1 interrupt enable)
    /* T1CON Settings 0x8006 = 0b1000 0000 0000 0110 */
    /* 15: 1 - timer 1 on
     * 5-4: 10 - prescaler = 64
     * 2 : 1 - external clock synchronized to to system clock (doesn't matter here)
     * 1 : 0 - timer 1 clock source = internal clock
     *          80 MHz clock
     *          80e6/(64 * 1250) = 1 kHz
     *
     */
    T1CONSET = 0x8024; // t1con (previously cleared) now set to:
                       // 0b1000 0000 0010 0100

}

void __ISR(_TIMER_1_IRQ, IPL3) Timer1Handler(void){
    /////////////////////////////////////
    // Obtain new sample from ADS chip //
    /////////////////////////////////////
    _RD9=0; //chip select on
    spi_tx(ADS_CMD_RDATAS); //RDATAS Flag Enable, read single sample
    spi_tx(0x00);
    spi_tx(0x00);
    spi_tx(0x00);
    temp=spi_tx(0x00);
    emg_data=(temp&0x80) ? (0xffffff00+temp) : temp;    // read 1st byte and 
                                                        // and extend sign bit 
    //build the rest of the data (3 bytes)
    emg_data=(emg_data<<8)+spi_tx(0x00);
    emg_data=(emg_data<<8)+spi_tx(0x00);
    _RD9=1; //chip select off

    //////////////////////////////////////////////
    // 1st apply 1st order high pass filter     //
    // to remove DC offset and baseline drift   //
    //////////////////////////////////////////////
    result = HighPass_filter_IIR_Hai(emg_data,hpf_input_prev, prev_result);

    //////////////////////////////////////////////
    //      Integral High Pass Filter           //
    //       By Hai Tang, 2015.9.11             //
    //////////////////////////////////////////////
    result = result - integral;
    integral = integral + (resultPrev + ((result - resultPrev)>>1))>>3; //Do not use *.125 etc, since it will force result's data type changed from int to float. Use >> instead.
    resultPrev = result;

    // Check for overflow, if so saturate result:
    if(result>MAX_ADS_OUT)
        result=MAX_ADS_OUT;
    else if(result<MIN_ADS_OUT)
        result=MIN_ADS_OUT;
    hpf_input_prev = emg_data;
    prev_result = result;

    //////////////////////////////////////////////
    // apply 4th order notch filter to remove   //
    // 60 Hz                                    //
    //////////////////////////////////////////////
    result = Notch_filter_IIR(result);
    // Check for overflow:
    if(result>MAX_ADS_OUT)
        result=MAX_ADS_OUT;
    else if(result<MIN_ADS_OUT)
        result=MIN_ADS_OUT;

    /////////////////////////////////////////////////
    //      Cut off Abnormal Large Spike on Raw    //
    //          By Hai Tang, 2015.9.2              //
    /////////////////////////////////////////////////
    if(result > spikeCutoffVal){ //Typical abnormal spike is in the order of 8*10^6 to 8*10^4. Can be as small as 5k when gain = 1. The peak raw sigal is 3k-4k for gain = 1.
        //So set the threshold as 5k for gain = 1.
        //The peak signal in gain 12 is about 4*10^4
        result = rawSignalPeak; //cut the spike
    }
    if(result < -spikeCutoffVal){
        result = -rawSignalPeak;
    }

    //////////////////////////////////////////
    // Convert Raw Data to Envelop          //
    // By Hai Tang, 2015.7.22               //
    //////////////////////////////////////////

    /////////////////////   Method 3: RMS (Root Mean Square) /////////////////////
    if(id<WindowLength){//Fill the window
        window[id] = result;
        id++;
    }
    else if(id>=WindowLength){
        envelop = 0;//clear previous envelop value
        //Take the RMS window
        for(i=0; i<WindowLength; i++){
            envelop += window[i]*window[i]/WindowLength;// square, mean, put mean here to avoid data overflow
        }
        envelop = sqrt(envelop);// root

       //Shift the window forward 1 step
        for(i=0; i<WindowLength-1; i++){
            window[i] = window[i+1];
        }
        window[WindowLength-1] = result;

    }
    //////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////
    //   Shift Down and Cut Negative Part of Envelop  //
    //            By Hai Tang, 2015.9.2           //
    ////////////////////////////////////////////////////
    envelop -= baselineThresh;
    if(envelop<0){
        envelop=0;
    }
    
    //////////////////////////////////////////
    //     Rescale the envelop to 0-255     //
    //     Which corresponding to 0-3.3V    //
    //          By Hai Tang, 2015.8.10      //
    //////////////////////////////////////////
    DACenvelop = envelop*255/(envelopPeak-baselineThresh);//D2 envelop peaks at 1400 with gain 1. Normalize DACenvelop to 0-255
    if(DACenvelop>255){
        DACenvelop = 255;//For DACenvelop>255, make it 255, otherwise it will become DACenvelop-255 as default
    }
    unsigned char DAC_WRITE_BUF_on[8] = {DACenvelop,DACenvelop,0,0,0,0,0,0};
    DAC_Output(0b00000011, DAC_WRITE_BUF_on);

    //////////////////////////////////////////
    //   Digitize the envelop to 77 levels  //
    //          By Hai Tang, 2015.8.13      //
    //////////////////////////////////////////
//    DigitizedEnvelop = floor(DACenvelop/stepSize);

    //////////////////////////////////////////
    // Transmit data though UART interface  //
    //////////////////////////////////////////
    static volatile int tx_data;
    utx(35);
    if(txEnvelop == 0){
    //Transimit the raw data
    tx_data = result>>16;
    utx(tx_data);
    tx_data = result>>8;
    utx(tx_data);
    tx_data = result;
    utx(tx_data);
    }
    else if (txEnvelop == 1){
    //Transmit the envelop
    tx_data = envelop>>16;//shift right, only the most significant byte left
    utx(tx_data);
    tx_data = envelop>>8;
    utx(tx_data);
    tx_data = envelop;
    utx(tx_data);
    }
    utx(36);

    //////////////////////////
    // Clear interrupt flag //
    //////////////////////////
    IFS0CLR = 0x00000010;
}

int main(void) {
    SYSTEMConfigPerformance(80000000L);// ALU performance optimized for the given clock frequency
    AD1PCFGSET = 0xf000; // setting some pins to digital function
    TRISBSET=0xf000;
    init_spi1();
    init_uart2();
    init_ads1291();
    init_timer1_intRC();
    INTCONbits.MVEC=1; // interrupt vector to multi-vector mode

    //////////////////////////////////
    // Initialize I2C2 and Ports    //
    // Added by Hai Tang,2015.8.11  //
    //////////////////////////////////
    // ETHCON1=0;
     DDPCONbits.JTAGEN = 0;   // disable JTAG port
     DDPCONbits.TROEN=0;
     DDPCONbits.TDOEN=0;
     initPORTs();
     INTEnableSystemMultiVectoredInt();  //enable the multivectored interrupt<<<<required for interrupt enabling>>>
     initI2C2();

    while(TRUE){
        // wait here to be interrupted by timer1 
        // would be better / more power efficient to sleep here
    }
    return 0;
}

//Added by Hai Tang, 2015.8.10
void initI2C2(void)
{

 I2C2CONCLR=0xFFFFFFFF;

 I2C2STATCLR=0xFFFFFFFF;

  I2C2BRG = 90;//89.68, Calculated BoudRate for 400kHz Comm, 80MHz CLK
                //(1/(2*400k)-tor)*80M-2, where tor = 104ns according to data sheet
                //Or 98, calculated by Megan with tor=0

 //I2C2BRG=22;  //22 =400kHz 18MHz clock
 //I2C2BRG=88;  //88 =400kHz 72MHz clock
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
}

//Added by Hai Tang, 2015.8.11
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
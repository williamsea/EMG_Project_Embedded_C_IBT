
// ADS1291 Command Definitions
#define ADS_CMD_WAKEUP 		2
#define ADS_CMD_STANDBY 	4
#define ADS_CMD_RESET		6
#define ADS_CMD_START_CONV	8
#define ADS_CMD_STOP_CONV	10
#define ADS_CMD_OFFSETCAL	26
#define ADS_CMD_RDATAC		16
#define ADS_CMD_SDATAC		17
#define ADS_CMD_RDATAS		18
#define ADS_CMD_READREG		32
#define ADS_CMD_WRITEREG	64

// ADS1291 Register definitions
#define ADS_REG_ID			0
#define ADS_REG_CONFIG1		1
#define ADS_REG_CONFIG2		2
#define ADS_REG_LOFF		3
#define ADS_REG_CH1SET		4
#define ADS_REG_CH2SET		5
#define ADS_REG_RLD_SENS	6
#define ADS_REG_LOFF_SENS	7
#define ADS_REG_LOFF_STAT	8
#define ADS_REG_RESP1		9
#define ADS_REG_RESP2		10
#define ADS_REG_GPIO		11

/* Register settings definitions
	These definitions can be OR'd together 
	to create a valid word to write to the
	respective register
*/

// Config1 : 
#define CONTINUOUS_MODE		0b00000000
#define SINGLE_SHOT_MODE	0b10000000
#define DATA_RATE_125		0b00000000
#define DATA_RATE_250		0b00000001
#define DATA_RATE_500		0b00000010
#define DATA_RATE_1000		0b00000011
#define DATA_RATE_2000		0b00000100
#define DATA_RATE_4000		0b00000101
#define DATA_RATE_8000		0b00000110

// Config2
// *** Note according to data sheet bit 7 must be set to 10
// *** therefore all definitions here have that bit set
#define LEAD_OFF_DISABLED	0b10000000
#define LEAD_OFF_ENABLED	0b11000000
#define REF_BUFFER_DISABLED 0b10000000
#define REF_BUFFER_ENABLED	0b10100000
#define VREF_2V				0b10000000
#define VREF_4V				0b10010000
#define OSC_CLK_OUT			0b10001000
#define OSC_CLK_OFF			0b10000000
#define INT_TEST_SIG_OFF	0b10000000
#define INT_TEST_SIG_ON		0b10000010
#define INT_TEST_SIG_AC		0b10000001
#define INT_TEST_SIG_DC		0b10000000

// CH1SET
#define PWR_UP	0b00000000
#define PWR_DWN	0b10000000
#define GAIN_6		0b00000000
#define GAIN_1		0b00010000
#define GAIN_2  	0b00100000
#define GAIN_3		0b00110000
#define GAIN_4		0b01000000
#define GAIN_8		0b01010000
#define GAIN_12		0b01100000
#define ELECT_IN	0b00000000
#define SHORT_INPUT	0b00000001
#define RLD_MEASURE 0b00000010
#define MEAS_VDD	0b00000011
#define TEMP_SENS	0b00000100
#define TEST_SIG	0b00000101
#define RLD_DRP		0b00000110
#define RLD_DRM		0b00000111
#define RLD_DRPM	0b00001000
#define INPUT3		0b00001001



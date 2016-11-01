#include <msp430.h> 
#include <driverlib.h>


void initGPIO(void);
void initClocks(void);


#define greenLED 		GPIO_PIN0
#define redLED			GPIO_PIN1
#define LEDPort			GPIO_PORT_P1
#define pushButton1		GPIO_PIN5
#define	pushButton2		GPIO_PIN6

#define	LF_CRYSTAL_FREQUENCY_IN_HZ	32768
#define HF_CRYSTAL_FREQUENCY_IN_HZ	0


uint32_t myACLK = 0;
uint32_t mySMCLK = 0;
uint32_t myMCLK = 0;
/*
 *
 * main.c
 */
int main(void)
{
	WDT_A_initIntervalTimer(WDT_A_BASE,
			WDT_A_CLOCKSOURCE_ACLK,
			WDT_A_CLOCKDIVIDER_512K);
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer to allow
    							//allow everything to intialize

	SFR_clearInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
	SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);

	//Initialization routines
    initGPIO();
    initClocks();

    WDT_A_start(WDT_A_BASE);
    _enable_interrupts();
    //PMM_turnOffRegulator();
    //PMM_disableSVSH();

    while(1)
    {
    	_low_power_mode_3(); //Enter Low Power Mode
    	_nop();
    }

}

void initClocks(void)
{
	//set the crystal frequencies attached to the LFXT and HFXT oscillator pins
	//so that driverlib knows how fast they are (needed for the clock 'get' functions)
	CS_setExternalClockSource(
			LF_CRYSTAL_FREQUENCY_IN_HZ,
			HF_CRYSTAL_FREQUENCY_IN_HZ
			);

//#ifdef __DEBUG
	//verify if the default clock settings are as expected
	myACLK = CS_getACLK();
	mySMCLK = CS_getSMCLK();
	myMCLK = CS_getMCLK();
//#endif

	//set FRAM controller waitstates to 1 when MCLK>8MHz (per datasheet)
	//Refer to the "Non-volatile memory" chapter for more details
	//FRAMCtl_configureWaitStateControl(FRAMCTL_ACCESS_TIME_CYCLES_1);

	//Set DCO to run at 8 MHz
	CS_setDCOFreq(CS_DCORSEL_1, CS_DCOFSEL_3);

	//Configure Clocks
	//Set ACLK to us VLO as its oscillator source (~10kHz)
	CS_initClockSignal(
			CS_ACLK,				//Clock we're configuring
			CS_VLOCLK_SELECT,		//Clock source
			CS_CLOCK_DIVIDER_1		//clock divider
			);
	//Set SMCLK to use DCO as its source
	CS_initClockSignal(
			CS_SMCLK,				//Clock we're configuring
			CS_DCOCLK_SELECT,		//Clock source
			CS_CLOCK_DIVIDER_1		//clock divider
			);

	//Set the MCLK to use the VLO clock
	/*
	CS_initClockSignal(
				CS_ACLK,				//Clock we're configuring
				CS_VLOCLK_SELECT,		//Clock source
				CS_CLOCK_DIVIDER_1		//clock divider
				);
				*/
//#ifdef __DEBUG
	//verify the modified clock settings are as expected
	myACLK = CS_getACLK();
	mySMCLK = CS_getSMCLK();
	myMCLK = CS_getMCLK();
//#endif

}

void initGPIO(void)
{

	P1DIR = 0xFF;
	P1OUT = 0x00;

	P2DIR = 0xFF;
	P2OUT = 0x00;

	P3DIR = 0xFF;
	P3OUT = 0x00;

	P4DIR = 0xFF;
	P4OUT = 0x00;

	P5DIR = 0xFF;
	P5OUT = 0x00;

	P6DIR = 0xFF;
	P6OUT = 0x00;

	P7DIR = 0xFF;
	P7OUT = 0x00;

	P8DIR = 0xFF;
	P8OUT = 0x00;

	PADIR = 0xFF;
	PAOUT = 0x00;

	PBDIR = 0xFF;
	PBOUT = 0x00;

	PCDIR = 0xFF;
	PCOUT = 0x00;

	PDDIR = 0xFF;
	PDOUT = 0x00;

	//set LED pin direction to output and turn off
	GPIO_setAsOutputPin(LEDPort,
				greenLED +
				redLED);

	GPIO_setOutputLowOnPin(LEDPort,
			greenLED +
			redLED);

	PMM_unlockLPM5();

	//Configure our Launchpad Push Buttons
	//inputs with pull-up resistors
	GPIO_setAsInputPinWithPullUpResistor(
			GPIO_PORT_P5, pushButton1 +
			pushButton2);

	//Configure GPIO pins ot trigger interrupt on a Low-to-High trnasition
	GPIO_selectInterruptEdge(
			GPIO_PORT_P5,
			pushButton1 +
			pushButton2,
			GPIO_LOW_TO_HIGH_TRANSITION);

	//Clear the interrupt flags for the push button pins
	GPIO_clearInterrupt(
			GPIO_PORT_P5,
			pushButton1 +
			pushButton2);

	//Enable the interrupt bits for the push button pins
	GPIO_enableInterrupt(
			GPIO_PORT_P5,
			pushButton1 +
			pushButton2);



	//Configure external crystal pins
	//Set LFXT to crystal input (rather than GPIO)
	//HFXT is not used, so we ignore those pins
	GPIO_setAsPeripheralModuleFunctionInputPin(
			GPIO_PORT_PJ,
			GPIO_PIN4 +		//LFXIN PJ.4
			GPIO_PIN5,		//LFXOUT PJ.5
			GPIO_PRIMARY_MODULE_FUNCTION);



//#ifdef __DEBUG
	//Output MSP clock signal to external pins for debugging purposes
	GPIO_setAsPeripheralModuleFunctionInputPin(
			GPIO_PORT_P5,
			GPIO_PIN7,		//MCLK on P5.7
			GPIO_TERNARY_MODULE_FUNCTION);

	GPIO_setAsPeripheralModuleFunctionInputPin(
			GPIO_PORT_P3,
			GPIO_PIN4,		//SMCLK on P3.4
			GPIO_SECONDARY_MODULE_FUNCTION);
//#endif

}

//Interrupt Service Routine
#pragma vector=PORT5_VECTOR
__interrupt void pushbutton_ISR(void)
{

	if(P5IV_P5IFG5)
	{
			GPIO_toggleOutputOnPin(LEDPort,redLED);
	}

}

#pragma vector=WDT_VECTOR
__interrupt void WDT_A_ISR(void)
{
	volatile uint32_t i;
	volatile uint32_t y;
	for(y=4;y>0;y--)
	{
		GPIO_toggleOutputOnPin(LEDPort,greenLED + redLED);
		for(i=100;i>0;i--)
		{
			_nop();
		}
	}

	GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
}

#include <msp430.h> 
#include <driverlib.h>
#include <cc2500comm.h>


void initGPIO(void);
void Chronomancer(void);
void initComms(void);

#define greenLED 		GPIO_PIN1
#define redLED			GPIO_PIN0
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
	//Initialize the watchdog as an interval timer to wake out of low power mode
	WDT_A_initIntervalTimer(WDT_A_BASE,
			WDT_A_CLOCKSOURCE_VLOCLK,
			WDT_A_CLOCKDIVIDER_32K);

	//Stop it until we're officially ready to start
	WDT_A_hold(WDT_A_BASE);

	//Arm the Watchdog interrupts
	SFR_clearInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
	SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);

	//Initialization routines
    initGPIO();
    Chronomancer();
    initComms();

    //Start the watchdog timer
    WDT_A_start(WDT_A_BASE);
    //PMM_turnOffRegulator(); //controls LPM.5 mode
    PMM_disableSVSH();

    //go to sleep
    while(1)
    {
    	_low_power_mode_4(); //Enter Low Power Mode
    }

}

void Chronomancer(void)
{
	//set the crystal frequencies attached to the LFXT and HFXT oscillator pins
	//so that driverlib knows how fast they are (needed for the clock 'get' functions)
	CS_setExternalClockSource(
			LF_CRYSTAL_FREQUENCY_IN_HZ,
			HF_CRYSTAL_FREQUENCY_IN_HZ
			);

	RTC_C_holdClock(RTC_C_BASE);
	CS_turnOffSMCLK();
	CS_turnOffLFXT();

	//Set ACLK to us VLO as its oscillator source (~10kHz)
	CS_initClockSignal(
			CS_ACLK,				//Clock we're configuring
			CS_VLOCLK_SELECT,		//Clock source
			CS_CLOCK_DIVIDER_1		//clock divider
			);

	//Set the MCLK to use the VLO clock
	CS_initClockSignal(
				CS_MCLK,				//Clock we're configuring
				CS_VLOCLK_SELECT,		//Clock source
				CS_CLOCK_DIVIDER_1		//clock divider
				);

}

void initGPIO(void)
{

#define GPIO_ALL        GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|  \
                        GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7

	//set unused pins

	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P1, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P6, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P7, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P8, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_PA, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_PB, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_PC, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_PD, GPIO_ALL);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_PJ, GPIO_ALL);
		PMM_unlockLPM5();


	//set LED pin direction to output and turn off
	GPIO_setAsOutputPin(LEDPort,
				greenLED +
				redLED);

	GPIO_setOutputLowOnPin(LEDPort,
			greenLED +
			redLED);

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
			GPIO_ALL);

	//Enable the interrupt bits for the push button pins
	GPIO_enableInterrupt(
			GPIO_PORT_P5,
			pushButton1 +
			pushButton2);


	//Configure external crystal pins
	//Set LFXT to crystal input (rather than GPIO)
	//HFXT is not used, so we ignore those pins
	//GPIO_setAsPeripheralModuleFunctionInputPin(
			//GPIO_PORT_PJ,
			//GPIO_PIN4 +		//LFXIN PJ.4
			//GPIO_PIN5,		//LFXOUT PJ.5
			//GPIO_PRIMARY_MODULE_FUNCTION);




//#ifdef __DEBUG
	//Output MSP clock signal to external pins for debugging purposes
	//GPIO_setAsPeripheralModuleFunctionInputPin(
			//GPIO_PORT_P5,
			//GPIO_PIN7,		//MCLK on P5.7
			//GPIO_TERNARY_MODULE_FUNCTION);

	//GPIO_setAsPeripheralModuleFunctionInputPin(
			//GPIO_PORT_P3,
			//GPIO_PIN4,		//SMCLK on P3.4
			//GPIO_SECONDARY_MODULE_FUNCTION);
//#endif


}



//Interrupt Service Routine
#pragma vector=PORT5_VECTOR
__interrupt void pushbutton_ISR(void)
{
	uint8_t i=0;
	uint32_t g=0;
	uint8_t buttonStatus = 0;

	//since we interrupted with a button push we'll pet the dog and put it
	//back in the house
	WDT_A_hold(WDT_A_BASE);
	WDT_A_resetTimer(WDT_A_BASE);

	//determine the button pushed and respond with LED output
	//TODO: possibly implement LED sequence as a timer based control to
	//conserve power instead of using nop loops
	buttonStatus = GPIO_getInterruptStatus(GPIO_PORT_P5,pushButton2);
	if(buttonStatus)
	{
		for(i=12;i>0;i--)
		{
			GPIO_toggleOutputOnPin(LEDPort,redLED);
			for(g=20;g>0;g--)
			{
				_nop();
			}
		}
		GPIO_setOutputLowOnPin(LEDPort,redLED);
		GPIO_clearInterrupt(
						GPIO_PORT_P5,
						pushButton2
						);
	}
	else
	{
		GPIO_setOutputLowOnPin(LEDPort,redLED);
	}

	buttonStatus = GPIO_getInterruptStatus(GPIO_PORT_P5,pushButton1);
	if(buttonStatus)
	{
		for(i=12;i>0;i--)
		{
			GPIO_toggleOutputOnPin(LEDPort,greenLED);
			for(g=20;g>0;g--)
			{
				_nop();
			}
		}
		GPIO_setOutputLowOnPin(LEDPort,redLED);
		GPIO_clearInterrupt(
						GPIO_PORT_P5,
						pushButton1
						);
	}
	else
		GPIO_setOutputLowOnPin(LEDPort,greenLED);

	//start the dog again
	WDT_A_start(WDT_A_BASE);

}

#pragma vector=WDT_VECTOR
__interrupt void WDT_A_ISR(void)
{
	//local loop variables
	volatile uint32_t i;
	volatile uint32_t y;

	for(y=6;y>0;y--)
	{
		GPIO_toggleOutputOnPin(LEDPort,greenLED + redLED);
		for(i=10;i>0;i--)
		{
			_nop();
		}
	}
	//in the case of an odd # loop count we need to make
	//sure the LED is turned off at the end of the routine
	GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
}

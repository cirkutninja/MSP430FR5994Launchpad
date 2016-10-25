#include <msp430.h> 
#include <driverlib.h>


void initGPIO(void);
/*
 * main.c
 */
int main(void)
{


	CS_initClockSignal(CS_ACLK,
			CS_VLOCLK_SELECT,
			CS_CLOCK_DIVIDER_1);
	CS_initClockSignal(CS_MCLK,
				CS_VLOCLK_SELECT,
				CS_CLOCK_DIVIDER_1);
	CS_initClockSignal(CS_SMCLK,
				CS_VLOCLK_SELECT,
				CS_CLOCK_DIVIDER_1);

	CS_turnOffSMCLK();




	//WDT_A_initIntervalTimer(WDT_A_BASE,
			//WDT_A_CLOCKSOURCE_ACLK,
			//WDT_A_CLOCKDIVIDER_512K);
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer



	//SFR_clearInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
	//SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
	
    initGPIO();



    // Set P1.0 to output direction
    //GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    //GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PMM_unlockLPM5();

    //WDT_A_start(WDT_A_BASE);
    //_enable_interrupts();
    PMM_turnOffRegulator();
    PMM_disableSVSH();
    _low_power_mode_4(); //Enter LPM3

           //_nop();


    return 0;
}

#pragma vector=WDT_VECTOR
__interrupt

void WDT_A_ISR(void)
{
	volatile uint32_t i;
	volatile uint32_t y;
	for(y=4;y>0;y--)
	{
		GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
		for(i=100;i>0;i--)
		{
			_nop();
		}
	}

	GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
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
}

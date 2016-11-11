/*
 * cc2500comm.c
 *
 *  Created on: Nov 10, 2016
 *      Author: kchristensen
 */

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_EUSCI_Ax__
#include "eusci_a_spi.h"

#include <assert.h>

//#define CSnRadio


void initSPIComms(void)
{
	//Initialize EUSCI SPI A Master
	EUSCI_A_SPI_initMasterParam param = {0};
	param.msbFirst = EUSCI_A_SPI_MSB_FIRST;
	param.clockPhase = EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
	param.clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
	param.spiMode = EUSCI_A_SPI_3PIN;
	EUSCI_A_SPI_initMaster(EUSCI_A0_BASE, &param);

	//Enable SPI Module
	EUSCI_A_SPI_enable(EUSCI_A0_BASE);


}
/*

void resetRadio(void)
{

}

void calibrateSynthOn(void)
{

}

void calibrateSynthOff(void)
{

}


void xtalOff(void)
{

}

void enableRX(void)
{

}

void enableTX(void)
{

}

void idle(void)
{

}

void wakeOnRadio(void)
{

}

void sleepRadio(void)
{

}

void flushRXbuffer(void)
{

}

void resetRadioRTC(void)
{

}

void chipStatus(void)
{

}

void spiStrobeCommand (uint8_t commandCode)
{
	//activate Hardware CSn line
	//send command
	//deactivate hardware CSnline

}
*/
uint8_t spiReadStatus(uint8_t readAddress)
{
	const uint8_t Status = 0xC0; //Status register requires both the read bit
						//and the burst mode bit set to '1'

	//=readAddress | Status	//shift bits to our read command register location request
	//send data
	//retrieve the received data

	return(0);
}

void spiReadConfig(uint8_t readAddress)
{
	const uint8_t Config = 0x80; // config request required header
	//readAddress = readAddress|Config
	//activate Hardware CSn line
	//send request
	//recieve data
	//deactivate hardware CSnline

}

void spiWrite(uint8_t readAddress, uint8_t writeData)
{
	//activate Hardware CSn line
		//send request
		//recieve data
		//deactivate hardware CSnline

}

#endif

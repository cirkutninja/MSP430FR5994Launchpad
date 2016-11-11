/*
///////////////////////////////////////////////////////////////
//////  *******Copyright (C) 2011 Actall Corporation***********
///////////////////////////////////////////////////////////////
 * cc2500comm.h
 *
 *  Created on: Nov 10, 2016
 *      Author: kchristensen
 *////////////////////////////////////////////////////////////

#ifndef CC2500COMM_H_
#define CC2500COMM_H_


//radio specific control registers
#define IOCFG2		0x00
#define IOCFG1 		0x01
#define IOCFG0 		0x02
#define FIFOTHR 	0x03
#define SYNC1 		0x04
#define SYNC0 		0x05
#define PKTLEN 		0x06
#define PKTCTRL1	0x07
#define PKTCTRL0 	0x08
#define ADDR		0x09
#define CHANNR 		0x0A
#define FSCTRL1		0x0B
#define FSCTRL0		0x0C
#define FREQ2 		0x0D
#define FREQ1 		0x0E
#define FREQ0 		0x0F
#define MDMCFG4 	0x10
#define MDMCFG3 	0x11
#define MDMCFG2 	0x12
#define MDMCFG1 	0x13
#define MDMCFG0 	0x14
#define DEVIATN 	0x15
#define MCSM2		0x16
#define MCSM1 		0x17
#define MCSM0 		0x18
#define FOCCFG		0x19
#define BSCFG		0x1A
#define AGCTRL2		0x1B
#define AGCTRL1		0x1C
#define AGCTRL0		0x1D
#define WOREVT1		0x1E
#define WOREVT0		0x1F
#define WORCTRL		0x20
#define FREND1		0x21
#define FREND0 		0x22
#define FSCAL3 		0x23
#define FSCAL2 		0x24
#define FSCAL1 		0x25
#define FSCAL0 		0x26
#define RCCTRL1		0x27
#define RCCTRL0		0x28
#define FSTEST 		0x29
#define PTEST 		0x2A
#define AGCTEST		0x2B
#define TEST2 		0x2C
#define TEST1 		0x2D
#define TEST0 		0x2E

//Status Registers
//The F_ indicates the burst bit access mode (a mode that is either a read or write access and must
//be terminated by setting CSn high)
#define PARTNUM 		0x30//0xF0
#define VERSION 		0x31//0xF1
#define FREQEST			0x32
#define LQI				0x33
#define RSSI			0x34
#define MARCSTATE 		0x35//0xF5
#define WORTIME1		0x36
#define WORTIME0		0x37
#define PKTSTATUS 		0x38//0xF8
#define VCO_VC_DAC 		0x39//0xF9
#define TXBYTES 		0x3A//0xFA
#define RXBYTES			0x3B
#define RCCTRL1_STATUS	0x3C
#define RCCTRL0_STATUS	0x3D


//Strobe Commands
#define SRES 		0x30 //Reset Chip
#define SFSTXON 	0x31 //Enable and calibrate frequency synthesizer (if MCSM0.FA_AUTOCAL = 1)
#define SXOFF 		0x32 //Turn off crystal oscillator
#define SCAL 		0x33 //Calibrate frequency synthesizer and turn it off (enables
							//quick start). SCAL can be strobed in IDLE state without
							//setting manual calibration mode (MCSM0.FA_AUTOCAL = 0)
#define SRX			0x34 //Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL=1
#define STX 		0x35 //Enable TX. Perform calibration first if MCSM0.FA_AUTOCAL = 1
#define SIDLE 		0x36 //Exit TX and turn off frequency synthesizer
#define SWOR		0x38 //Start automatic RX polling sequence (wake-on-radio)
#define SPWD 		0x39 //Enter power down mode when CSn goes high
#define SFRX		0x3A //Flush the RX FIFO buffer
#define SFTX 		0x3B //Flush the TX FIFO buffer
#define SWORRST		0x3C //reset real time clock to Event1 value
#define SNOP 		0x3D //No operation. May be used to get access to the chip status byte.

//Transmission Power level
#define PATABLE		0x3E
//64-byte TX FIFO register
#define TXFIFO			0x3F //single byte access
#define TXFIFO_burst 	0x7F //burst access mode

#define RXFIFO1		0x00 //Asserts when RX FIFO is filled at or above the RX FIFO threshold. De-asserts when RX FIFO is drained below the same threshold
#define RXFIFO2		0x01 //Asserts when RX FIFO is filled at or above the RX FIFO threshold or the end of packet is reached. De-asserts when the RX FIFO is empty
#define TXFIFO1		0x02 //Asserts when TX FIFO is filled at or above the TX FIFO threshold. De-asserts when TX FIFO is drained below the same threshold
#define TXFIFO2		0x03 //Asserts when TX FIFO is full. De-assserts when the TX FIFO is drained below the TX FIFO threshold
#define RXFIFOover	0x04 //Asserts when the RX FIFO has overflowed. De-asserts whenthe FIFO has been flushed
#define TXFIFOunder	0x05 //Asserts when the TX FIFO has underflowed. De-asserts when the FIFO is flushed
#define SYNC_radio	0x06 //Asserts when the sync work has been sent/received and de-asserts at the end of the packet. In RX, the pin will de-assert when the optional address check fails for the RX FIFO overflows. In TX it will de-assert if the TX FIFO underflows
#define CRCOK		0x07 //Asserts when the packet has been received with CRC OK. De-asserts when the first byte is read from the RX FIFO. Only valid if PCTCTRL0.CC2400 EN=1
#define PQR			0x08 //Asserts when the PQI is above the programmed PQT value
#define CCA			0x09 //High when RSSI level is below threshold
#define LDO			0x0A //The PLL is in lock if the lock detector output has a positive transition or is constantly logic high
#define SERCLK		0x0B //Serial Clock.
#define SSD			0x0C //Serial Syncronous Data Output (DO)
#define SDO_radio	0x0D //Serial Data Output
#define CARSENSE	0x0E //Carrier sense. High if RSSI level is above threshold
#define CRCOK2		0x15 //The last CRC comparison matched. Cleared when entering/restarting RX mode
#define RXHARD1		0x16 //Can be used together with RX_SYMBOL_TICk for alternative RX output
#define RXHARD2		0x17 //Can be used together with RX_SYMBOL_TICK for alternative RX output
#define PAPD		0x27
#define LNAPD		0x28
#define RXSYM		0x29
#define WOREVNT0	0x24
#define WOREVN1		0x25
#define CLK32K		0x27
#define CHIPRDY		0x29
#define XOSCSTABLE	0x2B
#define GDO0Z		0x2D
#define HIGHZ		0x2E
#define HW0			0x2F




#endif /* CC2500COMM_H_ */

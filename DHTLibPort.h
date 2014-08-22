#ifndef DHTLIBPORT_H
#define DHTLIBPORT_H
/*-------------------------------------------------------------*/
/*				Includes and dependencies						*/
/*-------------------------------------------------------------*/
#include <xc.h>			// Platform registers
#include <stdint.h>
#include "DHTLibConf.h"		// Project specific definitions

/*-------------------------------------------------------------*/
/*				Macros and definitions							*/
/*-------------------------------------------------------------*/
typedef uint8_t PORTBASE;
/*
 * This file is designed to be used with midrange family pic microcontrollers
 * and the XC8 C compiler from Microchip.
 */

// Macros for portability
/** This macro configures the output pin for the DHT11 as an output */
#define dhtlib_setout()	CONFIG_DHTLIB_TRIS = 0
/** This macro configures the input pin for the DHT22 as an output */
#define dhtlib_setin()	CONFIG_DHTLIB_TRIS = 1
/** This macro sets the DHT11 to a logic high state */
#define dhtlib_outhi()		{CONFIG_DHTLIB_OUTPIN = 1; CONFIG_DHTLIB_OUTPIN = 1;}
/** This macro sets the DHT11 to a logic low state */
#define dhtlib_outlow()		{CONFIG_DHTLIB_OUTPIN = 0; CONFIG_DHTLIB_OUTPIN = 0;}
/** This macro reads the current state of the DHT11 interface pin */
#define dhtlib_testpin()	CONFIG_DHTLIB_PORTPIN
/** This macro disables global interrupts while communicating with the DHT11 */
#define dhtlib_disint()		INTCONbits.GIE = 0;
/** This macro enables global interrupts after communication with the DHT11 */
#define dhtlib_enaint()		INTCONbits.GIE = 1;


#endif

#ifndef DHTLIBPORT_H
#define DHTLIBPORT_H
/*-------------------------------------------------------------*/
/*		Includes and dependencies			*/
/*-------------------------------------------------------------*/
#include <xc.h>			// Platform registers
#include <stdint.h>

/*-------------------------------------------------------------*/
/*		Macros and definition				*/
/*-------------------------------------------------------------*/
typedef uint8_t PORTBASE;
typedef uint8_t dht_t;

/** This macro disables global interrupts while communicating with the DHT11 */
#define dhtlib_disint()		INTCONbits.GIE = 0;
/** This macro enables global interrupts after communication with the DHT11 */
#define dhtlib_enaint()		INTCONbits.GIE = 1;


#endif

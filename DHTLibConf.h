#ifndef DHTLIBCONF_H
#define	DHTLIBCONF_H

#define CONFIG_TIMING_MAIN_CLOCK	2000000

/**
 * Configure here the registers and pins associated with the DHT11 sensor
 */
#define CONFIG_DHTLIB_OUTPIN		PORTBbits.RB2
#define CONFIG_DHTLIB_PORTPIN           PORTBbits.RB2
#define CONFIG_DHTLIB_TRIS		TRISBbits.TRISB2

#endif

/*	Library for DHT11 & DHT22 Temperature & Humidity Sensors
	Copyright (C) 2014 Jesus Ruben Santa Anna Zamudio.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Author website: http://www.geekfactory.mx
	Author e-mail: ruben at geekfactory dot mx
 */
#include"DHTLib.h"
#define CONFIG_TIMING_MAIN_CLOCK 2000000

unsigned char bits[5];

/*-------------------------------------------------------------*/
/*		Function prototypes (this file only)	*/
/*-------------------------------------------------------------*/
static void dhtlib_start(uint8_t pin);

/*-------------------------------------------------------------*/
/*		API Functions Implementation			*/

/*-------------------------------------------------------------*/
dht_t dhtlib_init(uint8_t pin)
{
	// Initialization just sets DHT11 pin as input
	io_mode(pin, INPUT);

	return pin;
}

static enum dht_status dhtlib_read(dht_t sensor)
{
	unsigned char i = 0;
	unsigned char aindex = 0;
	unsigned char bcount = 7;

	// This is a variable used as timeout counter, type of this variable should
	// be chosen according to the processor speed.
	PORTBASE tocounter = 0;

	// Clear reception buffer
	for (i = 0; i < 5; i++) bits[i] = 0;

	// Disable interrupts to keep timing accurate
	dhtlib_disint();

	// Generate DHT start signal
	dhtlib_start(sensor);

	// Wait for response from DHT11 max 80 uS low
	tocounter = 1;
	while (!io_read(sensor)) {
		if (!tocounter++)
			goto timeout;
	}
	// Wait for response from DHT11 max 80 uS high
	tocounter = 1;
	while (io_read(sensor)) {
		if (!tocounter++)
			goto timeout;
	}
	// Begin data reception, 40 bits to be received
	for (i = 0; i < 40; i++) {
		tocounter = 1;
		while (!io_read(sensor)) {
			if (!tocounter++)
				goto timeout;
		}
		// If after 50 uS the pin is low we're on the start of another bit
		delay_us(40);
		if (!io_read(sensor)) {
			if (bcount == 0) {
				bcount = 7;
				aindex++;
			} else {
				bcount--;
			}
			continue;
		}
		// If pin is high after 50 us we're receiving logic high
		tocounter = 1;
		while (io_read(sensor)) {
			if (!tocounter++)
				goto timeout;
		}
		// Set the bit and shift left
		bits[aindex] |= (1 << bcount);
		if (bcount == 0) {
			bcount = 7;
			aindex++;
		} else {
			bcount--;
		}
	}
	// Enable interrupts again
	dhtlib_enaint();
	return E_DHTLIB_OK;
	// Timeout while communicatig with DHT sensor
timeout:
	dhtlib_enaint();
	return E_DHTLIB_TIMEOUT_ERROR;
}

enum dht_status dhtlib_read11(dht_t sensor, uint8_t * temp, uint8_t * hum)
{
	// Read operation
	enum dht_status s = dhtlib_read(sensor);
	if (s != E_DHTLIB_OK)
		return s;
	// Checksum comprobation
	unsigned char chksum = bits[0] + bits[1] + bits[2] + bits[3];
	if (chksum != bits[4])
		return E_DHTLIB_CHKSUM_ERROR;

	* hum = bits[0];
	* temp = bits[2];
	// Return Ok code
	return E_DHTLIB_OK;
}

enum dht_status dhtlib_read22(dht_t sensor, uint16_t * temp, uint16_t * hum)
{
	// Read operation
	enum dht_status s = dhtlib_read(sensor);
	if (s != E_DHTLIB_OK)
		return s;
	// Checksum comprobation
	unsigned char chksum = bits[0] + bits[1] + bits[2] + bits[3];
	if (chksum != bits[4])
		return E_DHTLIB_CHKSUM_ERROR;

	* hum = ((bits[0] << 8) + bits[1]) & 0x7FFF;
	* temp = (bits[2] << 8) + bits[3];

	if (* temp & 0x8000) {
		* temp = -((* temp) & 0x7FFF);
	}
	// Return Ok code
	return E_DHTLIB_OK;

}

enum dht_status dhtlib_float22(dht_t sensor, float * temp, float * hum)
{
	enum dht_status s = dhtlib_read(sensor);
	if (s != E_DHTLIB_OK)
		return s;

	unsigned char chksum = bits[0] + bits[1] + bits[2] + bits[3];
	if (chksum != bits[4])
		return E_DHTLIB_CHKSUM_ERROR;

	* hum = 0.1 * (((bits[0] << 8) + bits[1]) & 0x7FFF);

	if (bits[2] & 0x80) {
		*temp = -0.1 * ((bits[2] << 8) + bits[3]);
	} else {
		*temp = 0.1 * ((bits[2] << 8) + bits[3]);
	}

	// Return Ok code
	return E_DHTLIB_OK;
}

/**
 * @brief Generates start signal on the bus
 *
 * This function generates the signal needed to start the comunication with the
 * tamperature and humidity sensor.
 */
static void dhtlib_start(uint8_t pin)
{
	io_mode(pin, OUTPUT); // Set pin as output
	io_write(pin, LOW); // Pull bus to low state
	delay_ms(20);
	io_write(pin, HIGH);
	io_mode(pin, INPUT);
	delay_us(60);
}

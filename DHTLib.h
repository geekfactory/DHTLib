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
#ifndef DHTLIB_H
#define DHTLIB_H
/*-------------------------------------------------------------*/
/*		Includes and dependencies			*/
/*-------------------------------------------------------------*/
#include "Delay/Delay.h"
#include "DHTLibPort.h"
/*-------------------------------------------------------------*/
/*		Macros and definitions				*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
/*		Typedefs enums & structs			*/
/*-------------------------------------------------------------*/

/**
 * Defines the sensor API return codes
 */
enum dht_status {
	E_DHTLIB_OK = 0, //!< Read operation on DHT11 succesfull
	E_DHTLIB_TIMEOUT_ERROR, //!< Timeout error occured on DHT11 comunication
	E_DHTLIB_CHKSUM_ERROR, //!< Checksum verification error
};
/*-------------------------------------------------------------*/
/*		Function prototypes				*/
/*-------------------------------------------------------------*/

/**
 * @brief Prepares the DHT11 communications channel
 *
 * Prepares the communication with the DHT11 sensor, initializes IO ports and
 * any other required peripherals.
 */
void dhtlib_init();

/**
 * @brief Reads the current temperature and humidity from DHT11 sensor
 *
 * Reads the current temperature and humidity, returns an enumerated value
 * defining if the comunication with the sensor was succesfull. This function
 * places the temperature and humidity readings on the provided buffers.
 * 
 * @param pxTemperature Pointer to place the temperature reading
 * @param pxHumidity Pointer to place the humidity reading
 *
 * @return An enumerated value indictating the communication and device status
 */
enum dht_status dhtlib_read11(uint8_t * temp, uint8_t * hum);

/**
 * @brief Reads the current temperature and humidity from DHT22 sensor
 *
 * Reads the current temperature and humidity, returns an enumerated value
 * defining if the comunication with the sensor was succesfull. This function
 * puts the temperature and humidity readings on the provided buffers.
 *
 * @param pxTemperature Pointer to place the temperature reading
 * @param pxHumidity Pointer to place the humidity reading
 *
 * @return An enumerated value indictating the communication and device status
 */
enum dht_status dhtlib_read22(uint16_t * temp, uint16_t * hum);

/**
 * @brief Reads the current temperature and humidity from DHT22 sensor
 *
 * Reads the current temperature and humidity, returns an enumerated value
 * defining if the comunication with the sensor was succesfull. This function
 * puts the temperature and humidity readings on the provided buffers.
 *
 * @param temp Pointer to float to store the temperature reading
 * @param hum Pointer to float to store the humidity reading
 *
 * @return An enumerated value indictating the communication and device status
 */
enum dht_status dhtlib_float22(float * temp, float * hum);

#endif
// End of Header file

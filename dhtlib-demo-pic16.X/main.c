#include <xc.h>
#include <stdio.h>
#include "../Tick/Tick.h"
#include "../LCD/LCD.h"
#include "../DHTLib.h"

// Configuración del dispositivo
// CONFIG1
#pragma config FOSC = INTOSCIO	// Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB0      // CCP1 Pin Selection bit (CCP1 function on RB0)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
// CONFIG2
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode disabled)

// Buffer para trabajar sobre el texto que se envía a la pantalla LCD
char buf[20];

// Prototipos de las funciones definidas en este archivo
void dht11_task(); // Actualiza la pantalla con la temperatura y humedad
void led_task(); // Hace parpadear el led de estado en el pin 4 del puerto A

/**
 * Implementación de la funcionalidad principal del programa
 */
void main()
{
	//OSCCON = 0x6C;	// Para usar el oscilador interno PIC16F88 4 Mhz
	OSCCON = 0x7C; // Para usar el oscilador interno PIC16F88 8 Mhz
	ANSEL = 0x00; // Configuracion de pines analogicos y digitales
	TRISA = 0xE0;
	TRISB = 0xFF;

	// Preparamos las librerías para su uso
	lcd_init(0, 16, 2); // Iniciar el controlador de pantalla
	tick_init(); // Iniciar el contador / temporizador del sistema
	dhtlib_init(); // Preparar la comunicación con el sensor DHT11

	// Encendemos la pantalla LCD y escribimos titulo
	lcd_on();
	lcd_puts("  PRUEBA DHT11  ");

	// Ciclo principal de nuestra aplicación
	for (;;) {
		led_task();
		dht11_task();
	}
}

/**
 * Código para leer el sensor de temperatura y mostrar en pantalla
 */
void dht11_task()
{
	// Variables locales
	static uint32_t ltick = 0;
	uint8_t tb = 0, hb = 0;

	// La lectura de la temperatura y la humedad se realiza cada 5 segundos
	if (tick_get() - ltick >= TICK_SECOND * 5) {
		// Recordar el tiempo en el que se toma la lectura
		ltick = tick_get();
		// Colocar el cursor en la segunda linea
		lcd_goto(0, 1);
		// EN ESTA LINEA REALIZAMOS LA LECTURA DEL SENSOR
		enum dht_status result = dhtlib_read11(&tb, &hb);
		// Preparamos la cadena a mostrar
		if (result == E_DHTLIB_OK)
			sprintf(buf, "T: %02d C H: %02d%%  ", tb, hb);
		else
			sprintf(buf, "Sensor error: %02d    ", result);
		// Escribimos la cadena a la pantalla
		lcd_puts(buf);
	}
}

/**
 * Esta funcion alterna el estado del led cada 1/2 segundo
 */
void led_task()
{
	// Guarda el valor del tick en el que se alterno el pin del led
	static uint32_t ltime = 0;

	if (tick_get() - ltime >= TICK_SECOND / 2) {
		ltime = tick_get();
		PORTA ^= 0x10;
	}
}

/**
 *  Implementación del vector de interrupción (único)
 */
void interrupt isr(void)
{
	// Actualizar el conteo del tiempo
	tick_update();
	// Otras interrupciones deben manejarse aqui
}

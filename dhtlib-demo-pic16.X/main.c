#include <xc.h>
#include <stdio.h>

#define CONFIG_TIMING_MAIN_CLOCK 2000000

#include "../Tick/Tick.h"
#include "../LCD/LCD.h"
#include "../DHTLib.h"

// Fusibles de configuración MCLR (reset) Activo, Protección de lectura desactivada
// Bronw-out reset desactivado, perro guardian desactivado, Power-up timer desactivado
// Oscilador Interno Activo
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
dht_t dht11;

// Prototipos de las funciones definidas en este archivo
void dht11_task(); // Actualiza la pantalla con la temperatura y humedad
void led_task(); // Hace parpadear el led de estado y dibuja las flechitas

/**
 Implementación de la funcionalidad principal del programa
 */
void main()
{
	//OSCCON = 0x6C; // Para usar el oscilador interno PIC16F88 4 Mhz
	OSCCON = 0x7C; // Para usar el oscilador interno PIC16F88 8 Mhz
	ANSEL = 0x00; // Configuracion de canales analogicos en PIC16F88

	// Preparamos las librerías para su uso
	tick_init(); // Iniciar el contador / temporizador del sistema
	lcd_init(0, 16, 2); // Iniciar el controlador de pantalla
	dht11 = dhtlib_init(10); // Preparar la comunicación con el sensor DHT11

	// Encendemos la pantalla LCD
	lcd_on();
	// Escribimos el encabezado
	lcd_puts("  PRUEBA DHT11  ");

	// Ciclo principal de nuestra aplicación
	// Aqui debemos incluir las funciones que deben ejecutarse periodicamente
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
	static unsigned long ltick = 0;
	// Temperature and humidity buffers
	uint8_t tb = 0, hb = 0;

	// La lectura de la temperatura y la humedad se realiza cada 5 segundos
	if (tick_get() - ltick >= TICK_SECOND * 5) {
		// Recordar el tiempo en el que se toma la lectura
		ltick = tick_get();
		// Colocar el cursor en la segunda linea
		lcd_goto(0, 1);
		// Dar formato e imprimir el texto
		enum dht_status eResult = dhtlib_read11( dht11, &tb, &hb);
		if (eResult == E_DHTLIB_OK)
			sprintf(buf, "T: %02d C H: %02d%%  ", tb, hb);
		else
			sprintf(buf, "Sensor error: %02d    ", eResult);
		// Escribimos la cadena a la pantalla
		lcd_puts(buf);
	}
}

/**
 * Esta funcion alterna el estado del pin donde esta conectado el led cada
 * 1/2 segundo y actualiza la pantalla LCD (la flecha, ver fotos)
 */
void led_task()
{
	// Guarda el valor del tick en el que se alterno el pin del led
	static unsigned long ltime = 0;

	if (tick_get() - ltime >= TICK_SECOND / 2) {
		ltime = tick_get();
		//lcd_goto(0, 0);
		//lcd_write(ucIconNumber++ & 0x03);
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

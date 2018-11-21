/*
 *
 *      Program uses a ATMega328p MCU to query a HC-SR04 ultrasonic module. Then Calculates and displays the distance to a object in cm on the NewHaven display.
 *
 *      The ultrasonic module is triggered every 60 ms by setting pin PC4 (the trigger) high for 10 us. A change in state on pin PC5 (the echo)
 *      causes a interrupt. The interrupt checks if PC5 is high. If it is a timer is started that increments every micro second. Once PC5 triggers a
 *      interrupt again and is low the result is calculated and displayed in centimeters on the NewHaven display. Distance in cm is calculated
 *      by the amount of microseconds PC5 (the echo) is active high divided by 58.
 *
 *      Pin placement of ATMega328p:
 *      Pin PC4				HC-SR04 Trig
 *      Pin PC5				HC-SR04 Echo
 */
#define F_CPU 16E6

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "scheduler.h"

#define UBBRVAL 51

#define HIGH 0x1
#define LOW  0x0


void init() {
	DDRD = 0xff;
	DDRB = 0xFF;
	DDRB &= ~(1<<DDB0);
	// Set Pin PB0 as input to read Echo
	PORTB |= (1<<PB0);
	// Enable pull up on C5
	PORTB &= ~(1<<PB1);
	// put trigger PB1 on low

	PRR &= ~(1<<PRTIM1);
	// To activate timer1 module
	TCNT1 = 0;
	// Initial timer value
	TCCR1B |= (1<<CS11);
	// prescaler of 8
	TCCR1B |= (1<<ICES1);
	// First capture on rising edge

	PCICR = (1<<PCIE0);	// Enable PCINT[7:0] we use pin PB0 which is PCINT0
	PCMSK0 = (1<<PCINT0);// Sets PB0 as interrupts port
	sei();				// Enable Interrupts
}


int readADC(uint8_t ADCport)
{
	// use ADC port
	ADMUX = ADCport;
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << ADLAR);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	// Enable ADC
	ADCSRA |= (1 << ADEN);
	// Start ADC conversion
	ADCSRA |= (1 << ADSC);
	// wait for ADC to finish
	while(ADCSRA & (1 << ADSC));
	int ADCvalue;
	ADCvalue = ADCL;
	ADCvalue = (ADCH << 8) + ADCvalue;
	//return ADCvalue
	return ADCvalue;
}

void encode(int sensor, int data){
	// maakt variabelen aan
	unsigned char value1;
	unsigned char value2;
	unsigned char value3;

	unsigned char first_hex;
	unsigned char second_hex;
	unsigned char xor;

	unsigned char temp;

	// zet de eerste 4 bits voor eth eerste hex getal op hun plek
	first_hex = (sensor & 0x0F)<<4;
	temp = (data & 0xF0)>>4;
	// naakt eerste hex waarde
	first_hex = first_hex | temp;
	// zet de eerste 4 bits op hun plek voor het 2de hex getal
	second_hex = (data & 0x0F)<<4;

	// wat bit shiften is nodig voor dat we het Xorren
	value1 = sensor;
	value2 = (data & 0xF0)>>4;
	value3 = data & 0x0F;

	// berekening voor de check
	xor = value1 ^ value2 ^ value3;

	//maakt de tweede hex waarde
	second_hex  = second_hex | xor;

	// verstuurt het packet
	transmit(first_hex);
	transmit(second_hex);
	transmit(0x0A);
}

void send_burst(){
	// zet de trigger hoog
	PORTB |= (1<<PB1);
	// voor 10uS
	_delay_us(10);
	// doet de trigger uit
	PORTB &= ~(1<<PB1);
	// turn on debugging LED
	PORTB |= (1<<PB3);
}

void getsensors()
{
	double ADCvalue = 0;
	int degrees = 0;
	while (1) {
		// reads temperature
		ADCvalue = readADC(1);
		// Celsius formule
		degrees = ((ADCvalue / 1024 * 5)-0.5)*100;
		// Int convert
		degrees = (int) degrees;

		// Encode data & transmit
		encode(8,degrees);
		// Encode data & transmit
		encode(4, readADC(0));
		send_burst();
		_delay_ms(1000);
	}
}


int main() {
	uart_init();
	init();
	SCH_Init_T1();
	unsigned char run_sensors;
	//unsigned char ultrasonar;
	//unsigned char run_adc;
	//unsigned char receive;
	//ultrasonar = SCH_Add_Task(send_burst,1,100);
	run_sensors = SCH_Add_Task(getsensors,5,1000);
	//receive = SCH_Add_Task(recieve,0,100);
	SCH_Start();
	while (1) {
		SCH_Dispatch_Tasks();
		//encode(1,0x23);
		_delay_ms(1000);
	}
}


ISR(PCINT0_vect) {
	cli();
	if (bit_is_set(PINB,PB0)) {					// Checks if echo is high
		TCNT1 = 0;								// Reset Timer
		PORTB |= (1<<PB3);
	} else {
		uint16_t gv_counter = TCNT1;					// Save Timer value
		uint8_t oldSREG = SREG;
		double distance;
		distance = gv_counter/58/2;

		//
		transmit((int)distance);
		if (distance < 10){
			PORTD = 0x80;
			
		}
		else if (distance > 40)
		{
			PORTD = 0x20;
			
		}
		else{
			PORTD = 0x40;
			
		}
		_delay_ms(1000);
		SREG = oldSREG;							// Enable interrupts
		PORTB &= ~(1<<PB3);						// Toggle debugging LED
		sei();
	}
}

ISR(USART_RX_vect)
{
	cli();
while(!(UCSR0A&(1<<RXC0))){};
// clear the USART interrupt
char recieved;
recieved = UDR0;
char recieved1;
recieved1 = recieve();

if (recieved == 0x01)
{
	PORTD |= (1<<PD3);
	_delay_ms(1000);
	if (recieved1 == 0x01)
	{
		PORTD |= (1<<PD6);
		_delay_ms(1000);
	}
	else if (recieved1 == 0x02)
	{
		PORTD |= (1<<PD5);
		_delay_ms(1000);
	}
}
_delay_ms(100);
sei();

}
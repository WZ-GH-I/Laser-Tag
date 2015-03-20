/*
 * main.c
 *
 *  Created on: 19 mar 2015
 *      Author: WZ_DELL
 *	Kod jest przeznaczony dla mikrokontrolera ATmega328
 */

#include <avr/io.h>
#include <avr/interrupt.h>

void timer0Init( void );
void timer1Init( void );
void int0Init( void );

volatile unsigned int counter = 0;

int main( void ) {
	DDRB = ( 1<<PB0 ) | ( 1<<PB1 );			//Ustawienie PB0 i PB1 jako wyjścia
	timer0Init();					//Inicjalizacja Timer0
	timer1Init();					//Inicjalizacja Timer1
	int0Init();					//Inicjalizacja INT0
	sei();						//Globalne odblokowanie przerwań

	while(1);					//Pętla główna
}

void timer0Init( void ) {
	TCCR0B |= ( 1<<CS02 );
	TIMSK0 |= ( 1<<TOIE0 );
}

void timer1Init( void ) {
	TCCR1B |= ( 1<<WGM12 ); 			//CTC
	TCCR1B |= ( 1<<CS12 ); 				//Preskaler 256
	OCR1A = 62500 - 1; 				//2s
//	TIMSK1 |= ( 1 << OCIE1A );			//Odblokowanie przerwania dla timera1
}

void int0Init( void ) {
	EICRA = ( 1<<ISC00 ) | ( 1<<ISC01 );		//Przerwanie wywoływane zboczem narastającym
	EIMSK = ( 1<<INT0 ); 				//Odblokowanie przerwania dla INT0
}

ISR( TIMER0_OVF_vect ) {
	if( counter >= 100 ) {
		PORTB ^= ( 1<<PB0 );			//Zmiana stanu diody na przeciwny
		counter = 0;
	}
	counter++;
}

ISR( TIMER1_COMPA_vect ) {
	EIMSK |= ( 1<<INT0 );				//Odblokowanie przerwania dla INT0
	TIMSK1 &= ~( 1 << OCIE1A );			//Zablokowanie przerwania dla timera1
	EIFR |= ( 1<<INTF0 );				//Zerowanie flagi przerwania INT0

}

ISR( INT0_vect ) {
	PORTB ^= ( 1<<PB1 );				//Zmiana stanu diody na przeciwny
	TIFR1 |= ( 1<<OCF1A );				//Zerowanie flagi przerwania timera1
	EIMSK &= ~( 1<<INT0 );				//Zablokowanie przerwania dla INT0
	TIMSK1 |= ( 1 << OCIE1A );			//Odblokowanie przerwania dla timera1
}

#include <avr/io.h>
#define F_CPU 1000000 // Clock Frequency
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect)//STEP2
{
	PORTA = 0xFF;
	GICR = 0; //STEP3
	_delay_ms(2000);
	GICR = (1 << INT0); //STEP3
	PORTA = 0x00;
}

int main(void){
	DDRA = 0xFF;
	GICR = (1 << INT0); //STEP3
	MCUCR = MCUCR | (1 << ISC01);//STEP4
	MCUCR = MCUCR & (~(1 << ISC00));//STEP4
	sei();//STEP5
	_delay_ms(500);
	while(1);
}
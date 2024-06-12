#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "instruction.h"

int main(void)
{
	// Some pins of PORTC can not be used for I/O directly.
	// turn of JTAG to use them
	// So write a 1 to the JTD bit twice consecutively to turn it off
	MCUCSR = (1 << JTD);
	MCUCSR = (1 << JTD);

	DDRB = 0x00; // input pc

	// Instruction : D[7:0]C[7:0]
	DDRC = 0xFF; // lower 8-bits of instruction
	DDRA = 0xFF; // upper 8-bits of instruction

	uint8_t pc = -1;
	uint16_t instruction = 0;
	/* Replace with your application code */
	while (1)
	{
		if (PINB != pc)
		{
			pc = PINB;
			if (pc < count)
			{
				instruction = INSTRUCTIONS[pc];
			}
			PORTC = (uint8_t)instruction;
			PORTA = (uint8_t)(instruction >> 8);
			_delay_ms(50);
		}
	}
}

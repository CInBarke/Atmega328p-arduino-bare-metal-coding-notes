#include <avr/io.h>
#include <util/delay.h>

//EXEMPLO 1: BLINKY:
//LITERALMENTE FIZ ESSE CÓDIGO EM TIPO 5 SEGUNDOS E NÃO TESTEI. SE TIVER ALGO
//DE ERRADO FAVOR AVISAR :D

int main()
{
	DDRD = DDRD | (1 << 3);
	while (1)
	{
		PORTD = PORTD | (1 << 3);
		_delay_ms(1000);
		PORTD = PORTD&~ (1 << 3);
		_delay_ms(1000);
	}
}

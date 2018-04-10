
#include <avr/io.h>

void setOE()
{
	PORTB |= 1;
}
void clrOE()
{
	PORTB &= ~(1);
}
void setRCLK()
{
	PORTB |= (1 << 1);
}
void clrRCLK()
{
	PORTB &= ~(1 << 1);
}
void setSRCLK()
{
	PORTB |= (1 << 2);
}
void clrSRCLK()
{
	PORTB &= ~(1 << 2);
}
void setSER()
{
	PORTB |= (1 << 3);
}
void clrSER()
{
	PORTB &= ~(1 << 3);
}

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}


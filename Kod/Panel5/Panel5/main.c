#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
//PISO - PORTD
#define PISO_DIR DDRD
#define PISO PORTD
#define LD 1 
#define CLK 2
#define Q 3

//SIPO - PORTD
#define SIPO_DIR DDRD
#define SIPO PORTD
#define OE 4
#define RCLK 5
#define SRCLK 6
#define SER 7

//DMPX - PORTC
#define DMPX_DIR DDRC
#define DMPX PORTC
#define DMPX0 0
#define DMPX1 1
#define DMPX2 2
#define DMPX3 3

//PWM - PORTB
#define PWM_DIR DDRB
#define PWM 1


void setSER()
{
	SIPO |= (1 << SER);
}
void clrSER()
{
	SIPO &= ~(1 << SER);
}
void setOE()
{
	SIPO |= (1 << OE);
}
void clrOE()
{
	SIPO &= ~(1 << OE);
}
void setRCLK()
{
	SIPO |= (1 << RCLK);
}
void clrRCLK()
{
	SIPO &= ~(1 << RCLK);
}
void setSRCLK()
{
	SIPO |= (1 << SRCLK);
}
void clrSRCLK()
{
	SIPO &= ~(1 << SRCLK);
}
void setCLK()
{
	PISO |= (1 << CLK);
}
void clrCLK()
{
	PISO &= ~(1 << CLK);
}
void setLD()
{
	PISO |= (1 << LD);
}
void clrLD()
{
	PISO &= ~(1 << LD);
}
int getQ()
{
	if((PIND & (1 << Q)) == (1 << Q)) return 1;
	else return 0;
}
void setValue(unsigned char val)
{
	clrRCLK();
	clrSRCLK();
	setOE();
	unsigned char i;

	for(i = 0; i < 8; i++)
	{
		if((val & (1 << i)) == (1 << i))
		{
			setSER();
		}
		else
		{
			clrSER();
		}
		//_delay_us(100);
		setSRCLK();
		clrRCLK();
		//_delay_us(100);
		clrSRCLK();
		setRCLK();
	}
	clrOE();
}
void Enable(unsigned char val)
{
	if((val & 1) == 1)
	{
		DMPX |= (1 << DMPX0);
	}
	else
	{
		DMPX &= ~(1 << DMPX0);
	}
	
	if((val & 2) == 2)
	{
		DMPX |= (1 << DMPX1);
	}
	else
	{
		DMPX &= ~(1 << DMPX1);
	}
	
	if((val & 4) == 4)
	{
		DMPX |= (1 << DMPX2);
	}
	else
	{
		DMPX &= ~(1 << DMPX2);
	}
	
	if((val & 8) == 8)
	{
		DMPX |= (1 << DMPX3);
	}
	else
	{
		DMPX &= ~(1 << DMPX3);
	}
}
int getValue()
{
	int val = 0;
	clrCLK();
	setLD();
	
	clrLD();
	setCLK();
	val |= getQ();
	clrCLK();
	setLD();
	for(int i = 1; i <= 15; i++)
	{
		setCLK();
		val |= (getQ() << i);
		clrCLK();
	}
	return val;
	
}
void init()
{
	PISO_DIR |= (1 << LD) | (1 << CLK);
	SIPO_DIR |= (1 << SER) | (1 << OE) | (1 << RCLK) | (1 << SRCLK);
	DMPX_DIR |= (1 << DMPX0) | (1 << DMPX1) | (1 << DMPX2) | (1 << DMPX3);
	PWM_DIR |= (1 << PWM);
}

int main(void)
{
	init();
	
	PORTB = 0;
	Enable(0);
    while (1) 
    {
		
		setValue(getValue()&0xff);
		
    }
}


/*
 * Panel1.c
 *
 * Created: 2018-03-15 14:18:49
 * Author : Kuba
 */ 

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
//PISO - PORTD
#define PISO_DIR DDRD
#define PISO PORTD
#define PISO_LD 1
#define PISO_CLK 2
#define PISO_Q 3
//SIPO - PORTD
#define SIPO_DIR DDRD
#define SIPO PORTD
#define OE 4
#define RCLK 5
#define SRCLK 6
#define SER 7

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
	PISO |= (1 << PISO_CLK);
}
void clrCLK()
{
	PISO &= ~(1 << PISO_CLK);
}
void setLD()
{
	PISO |= (1 << PISO_LD);
}
void clrLD()
{
	PISO &= ~(1 << PISO_LD);
}
int getQ()
{
	if((PIND & (1 << PISO_Q)) == (1 << PISO_Q)) return 1;
	else return 0;
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
void setValue(int val)
{
	clrRCLK();
	clrSRCLK();
	setOE();

	for(int i = 0; i < 16; i++)
	{
		if((val & (1 << i)) != (1 << i))
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
int getADCValue()
{
	ADCSRA |= (1 << ADSC);
	_delay_ms(1);
	return ((ADCL | (ADCH << 8)));
}
void LinijkaLED(int val)
{
	switch(val)
	{
		case 0:
		setValue(0);
		break;
		case 1:
		setValue(1 << 12);
		break;
		case 2: 
		setValue((1 << 12) | (1 << 11));
		break;
		case 3:
		setValue((1 << 12) | (1 << 11) | (1 << 10));
		break;
		case 4:
		setValue((1 << 12) | (1 << 11) | (1 << 10) | (1 << 9));
		break;
		case 5:
		setValue((1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8));
		break;
		default:
		break;
	}
}
int main(void)
{
   PISO_DIR |= (1 << PISO_LD) | (1 << PISO_CLK);
   SIPO_DIR |= (1 << SER) | (1 << OE) | (1 << RCLK) | (1 << SRCLK);
  // DDRC = 0xFF;
   DDRB = 4;
   //PORTB = 0xFF;
  // PORTC = 0;
   ADMUX = 0;
   ADCSRA = 0x80;
   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADMUX |= (1 << REFS0) | (1 << REFS0) | (1 << MUX1) | (1 << MUX0);
    TCCR1A = (1 << COM1B1) | (1 << COM1B0) | (1 << WGM10);
    TCCR1B = (1 << CS10);
    OCR1B = 231;
	while (1) 
    {
		setValue(getValue());
		
    }
}


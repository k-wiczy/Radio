#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
//PISO - PORTD
#define PISO_DIR DDRD
#define PISO PORTD
#define LD 0
#define CLK 1
#define Q 2

//SIPO - PORTD
#define SIPO_DIR DDRD
#define SIPO PORTD
#define OE 3
#define RCLK 4
#define SRCLK 5
#define SER 6

//DMPX - PORTC
#define DMPX_DIR DDRC
#define DMPX PORTC
#define DMPX0 1
#define DMPX1 2
#define DMPX2 3


//PWM - PORTB
#define PWM_DIR DDRB
#define PWM 1

//ANALOG - PORTC0 - ADC0
#define ADC_PIN 0

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
	DMPX_DIR |= (1 << DMPX0) | (1 << DMPX1) | (1 << DMPX2);
	PWM_DIR |= (1 << PWM);
	ADMUX = 0;
	ADCSRA = 0x80;
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADMUX |= (1 << REFS0);
	TCCR1A = (1 << COM1A1) | (1 << COM1A0) | (1 << WGM10);
	TCCR1B = (1 << WGM12) | (1 << CS10);
	OCR1A = 0;
}
int getADCValue()
{
	ADCSRA |= (1 << ADSC);
	_delay_ms(1);
	return (ADCL | (ADCH << 8));
}
int mapvalue(int val)
{
	if(val > 10 && val < 600)
	{
		return 0x0F;
	}
	if(val > 700 && val < 750)
	{
		return 0x7F;
	}
	if(val > 750 && val < 800)
	{
		return 0x77;
	}
	if(val > 800 && val < 850)
	{
		return 0xFF;
	}
	if(val > 850 && val < 900)
	{
		return 0x11;
	}
	if(val > 900 && val < 1024)
	{
		return 0x44;
	}
	return 0x00;
}
void setBrightness(int in)
{
	OCR1A = in;
}
int main(void)
{
	init();
	int s = 30;
	setValue(0xff);
	//PORTB = 2;
	Enable(0);
	//setBrightness(35);
	while (1)
	{
		setBrightness(s++);
		setValue(getValue());
		for(int i = 0; i <= 8; i++)
		{
			
			Enable(i);
			_delay_ms(1);
		}
		if(s == 50) s =30;
		_delay_ms(1);
	}
}


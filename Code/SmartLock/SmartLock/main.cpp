

#define OLED_WIDTH			128
#define OLED_HEIGHT			64
#define CPU_F				20000000UL
#define SCL_F				1250000UL
#define Skip_Flag			0xff
#define Dev_Adr				0b00111100

#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>


class TWI
{
	public:
	TWI(){}

	void twi_Init(void)
	{
		TWBR=(((CPU_F)/(SCL_F)-16)/2);
		TWSR=0;
	}

	void twi_SendAdress(void) 
	{
		TWDR=(Dev_Adr<<1)|0;
		TWCR=(1<<TWINT)|(1<<TWEN);
		while(!(TWCR & (1<<TWINT))){};
	}

	void twi_Start(void)
	{
		TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		while(!(TWCR & (1<<TWINT))){};
	}

	void twi_Stop(void)
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	}

	void twi_SendByte(int Inf)
	{
		TWDR=Inf;
		TWCR=(1<<TWINT)|(1<<TWEN);
		while(!(TWCR & (1<<TWINT))){};
	}
	
};

class OLED
{
private:
uint8_t  oled_bufer[(OLED_HEIGHT/8)*OLED_WIDTH];
TWI wire;
public:
OLED(TWI _wire)
{
	wire=_wire;
	wire.twi_Init();
	wire.twi_Start();
	wire.twi_SendAdress();
	OLED_Command(0xA8);
	OLED_Command(0x3F);

	OLED_Command(0xD3);
	OLED_Command(0x00);

	OLED_Command(0x40);
	OLED_Command(0xA1);

	OLED_Command(0xC8);
	OLED_Command(0xDA);
	OLED_Command(0x12);

	OLED_Command(0x81);
	OLED_Command(0xFF);
	OLED_Command(0xA4);

	OLED_Command(0xA6);
	OLED_Command(0xD5);
	OLED_Command(0x80);
	OLED_Command(0x8D);
	OLED_Command(0x14);
	OLED_Command(0xAF);

	OLED_Command(0x20);
	OLED_Command(0x00);
	OLED_Command(0x21);
	OLED_Command(0);
	OLED_Command(127);
	OLED_Command(0x22);
	OLED_Command(0);
	OLED_Command(7);
	
	for(int i=0;i<1024;i++)
	{
		oled_bufer[i]=0b00000000;
	}
	OLED_Write_Bufer();
	OLED_Data(0b11111111);
}
	

void OLED_Command(int command)
{
	TWDR=0x80;								
	TWCR=(1<<TWINT)|(1<<TWEN);				
	while(!(TWCR & (1<<TWINT))){};

	TWDR=command;							
	TWCR=(1<<TWINT)|(1<<TWEN);				
	while(!(TWCR & (1<<TWINT))){};
}


void OLED_Data(int data)
{
	TWDR=data;							
	TWCR=(1<<TWINT)|(1<<TWEN);				
	while(!(TWCR & (1<<TWINT))){};
}


void OLED_Write_Bufer()
{
	TWDR=0x40;
	TWCR=(1<<TWINT)|(1<<TWEN);
	for(int i=0;i<(OLED_HEIGHT/8)*OLED_WIDTH;i++)
	{
			OLED_Data(oled_bufer[i]);
	}
}


void OLED_Write_To_Bufer(int x,int y,int width,int height,uint8_t* img)
{
	for(int j=0;j<height;j++)
	{
		
			for(int i=0;i<width;i++)
			{
				for(int k=0;k<8;k++)
				{
				oled_bufer[(x+i*8+k)+(y+j/8)*OLED_WIDTH]|=((img[j*width+i]<<k)&0b10000000)>>(7-j%8);
			}
		}
	}
}



void writenumber(int i){
	switch(i)
	{
		case 0:
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b01111110);
		OLED_Data(0b01000010);
		OLED_Data(0b01000010);
		OLED_Data(0b01111110);
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		break;
		case 1:
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b00001000);
		OLED_Data(0b01000110);
		OLED_Data(0b01111110);
		OLED_Data(0b01000000);
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		break;
		case 2:
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b01110010);
		OLED_Data(0b01010010);
		OLED_Data(0b01001010);
		OLED_Data(0b01001110);
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		break;
		case 3:
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b01001010);
		OLED_Data(0b01001010);
		OLED_Data(0b01001010);
		OLED_Data(0b01111110);
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		break;
		case 4:
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b00011100);
		OLED_Data(0b00010000);
		OLED_Data(0b00010000);
		OLED_Data(0b01111110);
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		break;
		case 5:
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b01000110);
		OLED_Data(0b01001010);
		OLED_Data(0b01001010);
		OLED_Data(0b01110010);
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		break;
		case 6:
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b01111110);
		OLED_Data(0b01001010);
		OLED_Data(0b01001010);
		OLED_Data(0b01111010);
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		break;
		case 7:
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b00000010);
		OLED_Data(0b01110010);
		OLED_Data(0b00001010);
		OLED_Data(0b00000110);
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		break;
		case 8:
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b01111110);
		OLED_Data(0b01001010);
		OLED_Data(0b01001010);
		OLED_Data(0b01111110);
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		break;
		case 9:
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		OLED_Data(0b01011110);
		OLED_Data(0b01010010);
		OLED_Data(0b01010010);
		OLED_Data(0b01111110);
		OLED_Data(0b00000000);
		OLED_Data(0b00000000);
		break;
	}
}


};

TWI wire;
OLED oled(wire);
int num = 1;
short PINA_last_state=0;
bool button0=false;
bool button1=false;
bool button2=false;
bool button3=false;
bool butonpressed=false;

ISR(PCINT0_vect)
{
    cli();
	if (num == 1)
	num = 0;
	else
    num = 1;
//sleep_disable();
	if(PINA==0b00000100 )
	{
		oled.writenumber(0);
		SMCR|=0b00000000;
	}
	if(PINA==0b00001000)
	{
		oled.writenumber(1);
		SMCR|=0b00000000;
		
	}
	if(PINA==0b00010000)
	{
		oled.writenumber(2);
		
		SMCR|=0b00000001;
	}
	if(PINA==0b00100000)
	{
		oled.writenumber(3);
		
		SMCR|=0b00000001;
	}
		if(PINA==0b00011000)
		{
		oled.writenumber(4);	
		}
		if(PINA==0b00100100)
		{
			oled.writenumber(5);
		}
	sei();

}
uint8_t zero[]={
	0b00000000,
	0b01111110,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01111110,
	0b00000000
};

uint8_t one[]={
	0b00000000,
	0b00011000,
	0b00111000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b01111110,
	0b00000000	
};

uint8_t two[]={
	0b00000000,
	0b01111110,
	0b00000010,
	0b00000110,
	0b00011000,
	0b00110000,
	0b01111110,
	0b00000000
};

uint8_t three[]={
	0b00000000,
	0b01111110,
	0b00000010,
	0b01111110,
	0b00000010,
	0b00000010,
	0b01111110,
	0b00000000
};

uint8_t four[]={
	0b00000000,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01111110,
	0b00000010,
	0b00000010,
	0b00000000
};

uint8_t five[]={
	0b00000000,
	0b01111110,
	0b01000000,
	0b01111110,
	0b00000010,
	0b00000010,
	0b01111110,
	0b00000000
};

uint8_t six[]={
	0b00000000,
	0b01111110,
	0b01000000,
	0b01111110,
	0b01000010,
	0b01000010,
	0b01111110,
	0b00000000
};

uint8_t seven[]={
	0b00000000,
	0b01111110,
	0b00000010,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b00000000
};

uint8_t eight[]={
	0b00000000,
	0b01111110,
	0b01000010,
	0b01111110,
	0b01000010,
	0b01000010,
	0b01111110,
	0b00000000
};

uint8_t nine[]={
	0b00000000,
	0b01111110,
	0b01000010,
	0b01000010,
	0b01111110,
	0b00000010,
	0b01111110,
	0b00000000

};




uint8_t gear[] ={
	0B00000000, 0B00000001,0B10000000, 0B00000000,
	0B00000000, 0B00000011,0B11000000, 0B00000000,
	0B00000000, 0B00000111,0B11100000, 0B00000000,
	0B00011110, 0B00001111,0B11110000, 0B01111000,
	0B00011111, 0B10011110,0B01111001, 0B11111000,
	0B00011111, 0B11111100,0B00111111, 0B11111000,
	0B00011111, 0B11100000,0B00000111, 0B11111000,
	0B00001111, 0B00000000,0B00000000, 0B11111000,
	0B00001110, 0B00000111,0B11100000, 0B01110000,
	0B00000110, 0B00001110,0B01110000, 0B01100000,
	0B00000110, 0B00011000,0B00011000, 0B01100000,
	0B00001100, 0B00110000,0B00001100, 0B00110000,
	0B00011100, 0B01100001,0B10000110, 0B00111000,
	0B00111100, 0B11000011,0B11000011, 0B00111100,
	0B01111001, 0B10000111,0B11100001, 0B10011110,
	0B11110001, 0B10001110,0B01110001, 0B10011111,
	0B11110001, 0B10001110,0B01110001, 0B10011111,
	0B01111001, 0B10000111,0B11100001, 0B10011110,
	0B00111100, 0B11000011,0B11000011, 0B00111100,
	0B00011100, 0B01100001,0B10000110, 0B00111000,
	0B00001100, 0B00110000,0B00001100, 0B00110000,
	0B00000110, 0B00011000,0B00011000, 0B01100000,
	0B00000110, 0B00001110,0B01110000, 0B01100000,
	0B00001110, 0B00000111,0B11100000, 0B01110000,
	0B00001111, 0B00000000,0B00000000, 0B11111000,
	0B00011111, 0B11100000,0B00000111, 0B11111000,
	0B00011111, 0B11111100,0B00111111, 0B11111000,
	0B00011111, 0B10011110,0B01111001, 0B11111000,
	0B00011110, 0B00001111,0B11110000, 0B01111000,
	0B00000000, 0B00000111,0B11100000, 0B00000000,
	0B00000000, 0B00000011,0B11000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000001,0B10000000, 0B00000000
};

uint8_t lock[] =
{
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000001,0B10000000, 0B00000000,
	0B00000000, 0B00000111,0B11100000, 0B00000000,
	0B00000000, 0B00011100,0B00111000, 0B00000000,
	0B00000000, 0B01110000,0B00001110, 0B00000000,
	0B00000000, 0B11100000,0B00000111, 0B00000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000011, 0B11111111,0B11111111, 0B11000000,
	0B00000011, 0B11111111,0B11111111, 0B11000000,
	0B00000011, 0B11111111,0B11111111, 0B11000000,
	0B00000011, 0B11111110,0B01111111, 0B11000000,
	0B00000011, 0B11111100,0B00111111, 0B11000000,
	0B00000011, 0B11111100,0B00111111, 0B11000000,
	0B00000011, 0B11111110,0B01111111, 0B11000000,
	0B00000011, 0B11111110,0B01111111, 0B11000000,
	0B00000011, 0B11111100,0B01111111, 0B11000000,
	0B00000011, 0B11111110,0B01111111, 0B11000000,
	0B00000011, 0B11111111,0B11111111, 0B11000000,
	0B00000011, 0B11111111,0B11111111, 0B11000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000
};



uint8_t rt[] =
{

	0B10000000,
	0B11000000,
	0B11100000,
	0B11110000,
	0B11111000,
	0B11111100,
	0B11111110,
	0B11111111,
	0B11111111,
	0B11111110,
	0B11111100,
	0B11111000,
	0B11110000,
	0B11100000,
	0B11000000,
	0B10000000
};

uint8_t unlock[] =
{
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000001,0B10000000, 0B00000000,
	0B00000000, 0B00000111,0B11100000, 0B00000000,
	0B00000000, 0B00011100,0B00111000, 0B00000000,
	0B00000000, 0B01110000,0B00001110, 0B00000000,
	0B00000000, 0B11100000,0B00000111, 0B00000000,
	0B00000001, 0B11000000,0B00000011, 0B010000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000001, 0B11000000,0B00000011, 0B10000000,
	0B00000000, 0B10000000,0B00000011, 0B10000000,
	0B00000000, 0B00000000,0B00000011, 0B10000000,
	0B00000000, 0B00000000,0B00000011, 0B10000000,
	0B00000011, 0B11111111,0B11111111, 0B11000000,
	0B00000011, 0B11111111,0B11111111, 0B11000000,
	0B00000011, 0B11111111,0B11111111, 0B11000000,
	0B00000011, 0B11111110,0B01111111, 0B11000000,
	0B00000011, 0B11111100,0B00111111, 0B11000000,
	0B00000011, 0B11111100,0B00111111, 0B11000000,
	0B00000011, 0B11111110,0B01111111, 0B11000000,
	0B00000011, 0B11111110,0B01111111, 0B11000000,
	0B00000011, 0B11111100,0B01111111, 0B11000000,
	0B00000011, 0B11111110,0B01111111, 0B11000000,
	0B00000011, 0B11111111,0B11111111, 0B11000000,
	0B00000011, 0B11111111,0B11111111, 0B11000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000,
	0B00000000, 0B00000000,0B00000000, 0B00000000
};
uint8_t  lt[] =
{

	0B00000001,
	0B00000011,
	0B00000111,
	0B00001111,
	0B00011111,
	0B00111111,
	0B01111111,
	0B11111111,
	0B11111111,
	0B01111111,
	0B00111111,
	0B00011111,
	0B00001111,
	0B00000111,
	0B00000011,
	0B00000001
};

uint8_t  batary5[] =
{
	0B00111111, 0B11111111,0B11111111, 0B11111100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00101111, 0B01111011,0B11011110, 0B11110100,
	0B00101111, 0B01111011,0B11011110, 0B11110100,
	0B00101111, 0B01111011,0B11011110, 0B11110100,
	0B00101111, 0B01111011,0B11011110, 0B11110100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00111111, 0B11111111,0B11111111, 0B11111100
};

uint8_t batary4[] =
{
	0B00111111, 0B11111111,0B11111111, 0B11111100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00101111, 0B01111011,0B11011110, 0B00000100,
	0B00101111, 0B01111011,0B11011110, 0B00000100,
	0B00101111, 0B01111011,0B11011110, 0B00000100,
	0B00101111, 0B01111011,0B11011110, 0B00000100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00111111, 0B11111111,0B11111111, 0B11111100
};

uint8_t batary3[] =
{
	0B00111111, 0B11111111,0B11111111, 0B11111100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00101111, 0B01111011,0B11000000, 0B00000100,
	0B00101111, 0B01111011,0B11000000, 0B00000100,
	0B00101111, 0B01111011,0B11000000, 0B00000100,
	0B00101111, 0B01111011,0B11000000, 0B00000100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00111111, 0B11111111,0B11111111, 0B11111100
};

uint8_t batary2[] =
{
	0B00111111, 0B11111111,0B11111111, 0B11111100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00101111, 0B01111000,0B00000000, 0B00000100,
	0B00101111, 0B01111000,0B00000000, 0B00000100,
	0B00101111, 0B01111000,0B00000000, 0B00000100,
	0B00101111, 0B01111000,0B00000000, 0B00000100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00111111, 0B11111111,0B11111111, 0B11111100
};

uint8_t batary1[] =
{
	0B00111111, 0B11111111,0B11111111, 0B11111100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00101111, 0B00000000,0B00000000, 0B00000100,
	0B00101111, 0B00000000,0B00000000, 0B00000100,
	0B00101111, 0B00000000,0B00000000, 0B00000100,
	0B00101111, 0B00000000,0B00000000, 0B00000100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00111111, 0B11111111,0B11111111, 0B11111100
};

uint8_t batary0[] =
{
	0B00111111, 0B11111111,0B11111111, 0B11111100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00100000, 0B00000000,0B00000000, 0B00000100,
	0B00111111, 0B11111111,0B11111111, 0B11111100
};


uint8_t Net4[] =
{
	0B11100000, 0B00000000,
	0B11100000, 0B00000000,
	0B11101110, 0B00000000,
	0B11101110, 0B00000000,
	0B11101110, 0B11100000,
	0B11101110, 0B11100000,
	0B11101110, 0B11101110,
	0B11101110, 0B11101110,

};


uint8_t Net3[] =
{
	0B11100000, 0B00000000,
	0B10100000, 0B00000000,
	0B10101110, 0B00000000,
	0B10101110, 0B00000000,
	0B10101110, 0B11100000,
	0B10101110, 0B11100000,
	0B10101110, 0B11101110,
	0B11101110, 0B11101110,

};


uint8_t Net2[] =
{
	0B11100000, 0B00000000,
	0B10100000, 0B00000000,
	0B10101110, 0B00000000,
	0B10101010, 0B00000000,
	0B10101010, 0B11100000,
	0B10101010, 0B11100000,
	0B10101010, 0B11101110,
	0B11101110, 0B11101110,
};


uint8_t Net1[] =
{
	0B11100000, 0B00000000,
	0B10100000, 0B00000000,
	0B10101110, 0B00000000,
	0B10101010, 0B00000000,
	0B10101010, 0B11100000,
	0B10101010, 0B10100000,
	0B10101010, 0B10101110,
	0B11101110, 0B11101110,

};

uint8_t Net0[] =
{
	0B11100000, 0B00000000,
	0B10100000, 0B00001010,
	0B10101110, 0B00000100,
	0B10101010, 0B00001010,
	0B10101010, 0B11100000,
	0B10101010, 0B10100000,
	0B10101010, 0B10101110,
	0B11101110, 0B10101110,
};

int main(void){
		//TWI wire;
		//OLED oled(wire);
		//oled.OLED_Write_Bufer();
		//_delay_ms(1000);
		//oled.OLED_Write_To_Bufer(0,0,0b11111111);
		
		oled.OLED_Write_To_Bufer(0,0,4,32,gear);
		oled.OLED_Write_To_Bufer(33,0,4,32,lock);
		oled.OLED_Write_To_Bufer(80,0,4,32,unlock);
		oled.OLED_Write_To_Bufer(0,5,4,8,batary2);
		oled.OLED_Write_To_Bufer(40,5,4,8,batary5);
		oled.OLED_Write_To_Bufer(80,5,2,8,Net0);
		oled.OLED_Write_To_Bufer(100,5,2,8,Net4);
		//	oled. OLED_Write_To_Bufer(0,0b11111111);
		//oled.OLED_Write_Bufer();
		//oled.oled_bufer[1]=0b11111111;
		oled.OLED_Write_Bufer();
		//Interupts

		DDRA = 0x00;
		PORTA=0x00;
		PCMSK0=0b00111100;
		PCICR|=0b00000001;
		sei();
		//
		//DDRD|=0b10000000;
		
		
	while(1)
	{
	
	}
}


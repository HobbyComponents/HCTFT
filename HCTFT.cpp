/* FILE:    HCTFT.cpp
   DATE:    02/06/16
   VERSION: 0.2
   AUTHOR:  Andrew Davies

15/06/15 version 0.1: Original version
02/06/16 version 0.2: Updated to include Elec Freaks 3.2" display with ILI9341 contoller
   
   
Library for TFT screens. Currently supported screens:

MCUFriend ILI9481 based 3.5" TFT screen for Mega (MCUFRIEND_3_5_MEGA)
Elec Freaks 3.2" TFT screen with ILI9481 Contoller for Mega (ELECFREAKS_3_2_WITH_TFT01_MEGA)
MCUFriend 2_4" TFT screen for UNO (MCUFRIEND_2_4_UNO)
Elec Freaks 2_4" TFT screen for UNO (ELECFREAKS_2_4_WITH_TFT01_UNO)
Elec Freaks 3.2" TFT screen with ILI9341 Contoller for Mega (ELECFREAKS_3_2_WITH_TFT01_MEGA)

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/


#include "HCTFT.h"
#include "Arduino.h"
#include <avr/pgmspace.h>
#include "fonts/fonts.h"


/* Control pin mappings for each display type (see MemorySave.h for order) */
const PROGMEM byte DEF_WR[] = {39,39,18,15,39};
const PROGMEM byte DEF_CS[] = {40,40,17,17,40};
const PROGMEM byte DEF_DC[] = {38,38,19,16,38};
const PROGMEM byte DEF_RST[] = {41,41,16,18,41};
const PROGMEM byte DEF_SDCS[] = {53,53,10,10,53};

/* Screen resolutions (see MemorySave.h for order) */
const PROGMEM unsigned int MAX_X[] = {479,319,319,319, 319};
const PROGMEM unsigned int MAX_Y[] = {319,239,239,239, 239};

/* Port used for high and low bytes of data bus (see MemorySave.h for order) */
volatile byte *DPH[] = {&PORTA,&PORTA,&PORTD,&PORTD,&PORTA};
volatile byte *DPL[] = {&PORTC,&PORTC,&PORTD,&PORTB,&PORTC};
volatile byte *DDDRH[] = {&DDRA,&DDRA,&DDRD,&DDRD,&DDRA};
volatile byte *DDDRL[] = {&DDRC,&DDRC,&DDRD,&DDRB,&DDRC};

/* Mask for high and low bytes of data bus (see MemorySave.h for order) */
const PROGMEM byte DHMASK[] = {0xFF,0xFF,0xFF,0xFC,0xFF};
const PROGMEM byte DLMASK[] = {0xFF,0xFF,0xFF,0xF3,0xFF};



/* Constructor to initialise the GPIO and library */
HCTFT::HCTFT(byte DisplayType)
{
	_Display = DisplayType;
	
	/* Get pin number and pin mask for each control pin */
	_WRP =  portOutputRegister(digitalPinToPort(pgm_read_byte_near(&DEF_WR[_Display])));
	_WRM =  digitalPinToBitMask(pgm_read_byte_near(&DEF_WR[_Display]));
	_CSP =  portOutputRegister(digitalPinToPort(pgm_read_byte_near(&DEF_CS[_Display])));
	_CSM =  digitalPinToBitMask(pgm_read_byte_near(&DEF_CS[_Display]));
	_DCP =  portOutputRegister(digitalPinToPort(pgm_read_byte_near(&DEF_DC[_Display])));
	_DCM =  digitalPinToBitMask(pgm_read_byte_near(&DEF_DC[_Display]));
	_RST = pgm_read_byte_near(&DEF_RST[_Display]);	
	
	/* Set all control pins to outputs */
	pinMode(pgm_read_byte_near(&DEF_RST[_Display]), OUTPUT);
	pinMode(pgm_read_byte_near(&DEF_CS[_Display]), OUTPUT);
	pinMode(pgm_read_byte_near(&DEF_DC[_Display]), OUTPUT);
	pinMode(pgm_read_byte_near(&DEF_WR[_Display]), OUTPUT);

	/* Set data bus pins to outputs */
	*DDDRH[_Display] = pgm_read_byte_near(&DHMASK[_Display]);
	*DDDRL[_Display] = pgm_read_byte_near(&DLMASK[_Display]);
	
	/* Set control pins high to disable data bus */
	CSHIGH;
	DCHIGH;
	WRHIGH;

	/* Set default foreground and background colours */
	_FGR = 0x00;
	_FGG = 0x00;
	_FGB = 0x00;
	_BGR = 0xFF;
	_BGG = 0xFF;
	_BGB = 0xFF;	
	
	/* Set font and BW graphic scaling to 1 (no scaling) */
	_BMScale = 1;
	
	/* Get Chip Select pin for SD card */
	#ifdef SDCARD
	_SDCS = pgm_read_byte_near(&DEF_SDCS[_Display]);
	#endif
}

/* Initialise the selected display */
void HCTFT::Init(void)
{
	Reset();
	CSLOW;
	switch(_Display)
	{
		#ifdef MCUFRIEND_3_5_MEGA 
		case(MCUFRIEND_3_5_MEGA): //(ILI9481) Parameters referenced from UTFT library
			_CommandOut(0x11); //Exit sleep mode
			delay(20);


			 _CommandOut(0xD0); //Power setting
			 _DataOut(0x07); // reference voltage ratio
			 _DataOut(0x42);// reference voltage step up / start step up
			 _DataOut(0x18); //Set VRH = 4.875V

			 _CommandOut(0xD1); //Set VCOMH voltage
			 _DataOut(0x00);
			 _DataOut(0x07);
			 _DataOut(0x10);

			 _CommandOut(0xD2); //Power_Setting for Normal Mode 
			 _DataOut(0x01);
			 _DataOut(0x02);

			 _CommandOut(0xC0); //Panel Driving Setting
			 _DataOut(0x10);
			 _DataOut(0x3B);
			 _DataOut(0x00);
			 _DataOut(0x02);
			 _DataOut(0x11);

			 _CommandOut(0xC5); //Frame Rate and Inversion Control 
			 _DataOut(0x03);

			 _CommandOut(0xC8); //Gamma Setting 
			 _DataOut(0x00);
			 _DataOut(0x32);
			 _DataOut(0x36);
			 _DataOut(0x45);
			 _DataOut(0x06);
			 _DataOut(0x16);
			 _DataOut(0x37);
			 _DataOut(0x75);
			 _DataOut(0x77);
			 _DataOut(0x54);
			 _DataOut(0x0C);
			 _DataOut(0x00);
			
			_CommandOut(0x36); //Set_address_mode
			_DataOut(0x2A); //Horizontal flip, BGR order

			_CommandOut(0x3A); //Set_pixel_format
			_DataOut(0x55); //16bit/pixel (65,536 colours) 

			delay(100);
			_CommandOut(0x29); //Set_display_on 
			break;
		#endif

		#ifdef ELECFREAKS_3_2_WITH_TFT01_MEGA			
		case(ELECFREAKS_3_2_WITH_TFT01_MEGA): //(SSD1289) Parameters referenced from Elec Freaks
			_CommandOut(0x00); //Turn oscillator on
			_DataOut(0x0001);
			_CommandOut(0x03); //Power control
			_DataOut(0xA8A4);			
			_CommandOut(0x0C); //Power control 2
			_DataOut(0x0000);	
			_CommandOut(0x0D); //Power control 3
			_DataOut(0x080C);	
			_CommandOut(0x0E); //Power control 4
			_DataOut(0x2B00);
			_CommandOut(0x1E); // Power control 5
			_DataOut(0x00B7);
			_CommandOut(0x01); // Driver output control
			_DataOut(0x2B3F);
			_CommandOut(0x02); // LCD driving waveform control
			_DataOut(0x0600);
			_CommandOut(0x10); // Take display out of sleep
			_DataOut(0x0000);
			_CommandOut(0x11); //Entry mode 
			_DataOut(0x6070);	
			_CommandOut(0x07); // Display control - Turn display on
			_DataOut(0x0233);
			_CommandOut(0x0B); //Frame cycle control
			_DataOut(0x0000);
			_CommandOut(0x4A); //2nd screen driving position
			_DataOut(0x0000);
			_CommandOut(0x4B);
			_DataOut(0x0000);	
			_CommandOut(0x30); //Gamma control
			_DataOut(0x0707);	
			_CommandOut(0x31);
			_DataOut(0x0204);	
			_CommandOut(0x32);
			_DataOut(0x0204);	
			_CommandOut(0x33);
			_DataOut(0x0502);
			_CommandOut(0x34);
			_DataOut(0x0507);	
			_CommandOut(0x35);
			_DataOut(0x0204);	
			_CommandOut(0x36);
			_DataOut(0x0204);	
			_CommandOut(0x37); 
			_DataOut(0x0502);	
			_CommandOut(0x3A); 
			_DataOut(0x0302);
			_CommandOut(0x3B); 
			_DataOut(0x0302);
			_CommandOut(0x22); // Write data to GRAM	
			break;
		#endif	
		
		#ifdef ELECFREAKS_2_4_WITH_TFT01_UNO			
		case(ELECFREAKS_2_4_WITH_TFT01_UNO): //(ILI9325) Parameters referenced from Elec Freaks
			_CommandOut(0xE5);
			_DataOut(0x78F0);
			_CommandOut(0x01); 
			_DataOut(0x0100);
			_CommandOut(0x02);
			_DataOut(0x0700);
			_CommandOut(0x03);
			_DataOut(0x1030);
			_CommandOut(0x04);
			_DataOut(0x0000);
			_CommandOut(0x08);
			_DataOut(0x0207);
			_CommandOut(0x09);
			_DataOut(0x0000);
			_CommandOut(0x0A);
			_DataOut(0x0000);
			_CommandOut(0x0C);
			_DataOut(0x0000);
			_CommandOut(0x0D);
			_DataOut(0x0000);
			_CommandOut(0x0F);
			_DataOut(0x0000);
			_CommandOut(0x10);
			_DataOut(0x0000);
			_CommandOut(0x11);
			_DataOut(0x0007);
			_CommandOut(0x12);
			_DataOut(0x0000);
			_CommandOut(0x13);
			_DataOut(0x0000);
			_CommandOut(0x07);
			_DataOut(0x0001);
			delay(50); 
			_CommandOut(0x10);
			_DataOut(0x1090);
			_CommandOut(0x11);
			_DataOut(0x0227);
			delay(50); 
			_CommandOut(0x12);
			_DataOut(0x001F);
			delay(50); 
			_CommandOut(0x13);
			_DataOut(0x1500);
			_CommandOut(0x29);
			_DataOut(0x0027);
			_CommandOut(0x2B);
			_DataOut(0x000D);
			delay(50);
			_CommandOut(0x20);
			_DataOut(0x0000);
			_CommandOut(0x21);
			_DataOut(0x0000);
			_CommandOut(0x30);
			_DataOut(0x0000);
			_CommandOut(0x31);
			_DataOut(0x0707);
			_CommandOut(0x32);
			_DataOut(0x0307);
			_CommandOut(0x35);
			_DataOut(0x0200);
			_CommandOut(0x36);
			_DataOut(0x0008);
			_CommandOut(0x37);
			_DataOut(0x0004);
			_CommandOut(0x38);
			_DataOut(0x0000);
			_CommandOut(0x39);
			_DataOut(0x0707);
			_CommandOut(0x3C);
			_DataOut(0x0002);
			_CommandOut(0x3D);
			_DataOut(0x1D04);
			
			_CommandOut(0x50);
			_DataOut(0x0000);
			_CommandOut(0x51);
			_DataOut(0x00EF);
			_CommandOut(0x52);
			_DataOut(0x0000);
			_CommandOut(0x53);
			_DataOut(0x013F);
			_CommandOut(0x60);
			_DataOut(0xA700);
			_CommandOut(0x61);
			_DataOut(0x0001);
			_CommandOut(0x6A);
			_DataOut(0x0000);
			
			_CommandOut(0x80);
			_DataOut(0x0000);
			_CommandOut(0x81);
			_DataOut(0x0000);
			_CommandOut(0x82);
			_DataOut(0x0000);
			_CommandOut(0x83);
			_DataOut(0x0000);
			_CommandOut(0x84);
			_DataOut(0x0000);
			_CommandOut(0x85);
			_DataOut(0x0000);
			
			_CommandOut(0x90);
			_DataOut(0x0010);
			_CommandOut(0x92);
			_DataOut(0x0600);
			_CommandOut(0x07);
			_DataOut(0x0133);
			break;
		#endif
		
		#ifdef MCUFRIEND_2_4_UNO 
		case(MCUFRIEND_2_4_UNO): //(ILI9325D) Parameters referenced from similar Adafruit display
			pinMode(A0, OUTPUT);
			digitalWrite(A0, HIGH);
			_CommandOut(0xCB); 
			_DataOut(0x39);
			_DataOut(0x2C);
			_DataOut(0x00);
			_DataOut(0x34);
			_DataOut(0x02);
			_CommandOut(0xCF); 
			_DataOut(0x00);
			_DataOut(0xC1);
			_DataOut(0x30);	
			_CommandOut(0xE8);
			_DataOut(0x85);
			_DataOut(0x00);
			_DataOut(0x78);	
			_CommandOut(0xEA);
			_DataOut(0x00);
			_DataOut(0x00);	
			_CommandOut(0xED);
			_DataOut(0x64);
			_DataOut(0x03);
			_DataOut(0x12);
			_DataOut(0x81);			
			_CommandOut(0xF7);
			_DataOut(0x20);
			_CommandOut(0xC0);
			_DataOut(0x23);
			_CommandOut(0xC1);
			_DataOut(0x10);
			_CommandOut(0xC5);
			_DataOut(0x3E);
			_DataOut(0x28);
			_CommandOut(0xC7);
			_DataOut(0x86);
			_CommandOut(0x36); //Memory Access Control
			_DataOut(0x48);
			_CommandOut(0x3A); //COLMOD: Pixel Format Set
			_DataOut(0x55); // (16 bits per pixel)
			_CommandOut(0xB1); //Frame Rate Control
			_DataOut(0x00);
			_DataOut(0x18);
			_CommandOut(0xB6); //Display function control
			_DataOut(0x08);
			_DataOut(0x82);
			_DataOut(0x27);
			_CommandOut(0xF2); //??
			_DataOut(0x00);
			_CommandOut(0x26); //Gamma set
			_DataOut(0x01);
			_CommandOut(0xE0); //Positive gamma correction
			_DataOut(0x0f);
			_DataOut(0x31);
			_DataOut(0x2b);
			_DataOut(0x0c);
			_DataOut(0x0e);
			_DataOut(0x08);
			_DataOut(0x4e);
			_DataOut(0xf1);
			_DataOut(0x37);
			_DataOut(0x07);
			_DataOut(0x10);
			_DataOut(0x03);
			_DataOut(0x0e);
			_DataOut(0x09);
			_DataOut(0x00);
			_CommandOut(0xE1); //Negative gamma correction
			_DataOut(0x00);
			_DataOut(0x0e);
			_DataOut(0x14);
			_DataOut(0x03);
			_DataOut(0x11);
			_DataOut(0x07);
			_DataOut(0x31);
			_DataOut(0xC1);
			_DataOut(0x48);
			_DataOut(0x08);
			_DataOut(0x0f);
			_DataOut(0x0c);
			_DataOut(0x31);
			_DataOut(0x36);
			_DataOut(0x0f);
			_CommandOut(0x11); // Sleep Out
			delay(150);
			_CommandOut(0x29);
			_CommandOut(0x2C);		
		#endif
		
		#ifdef ELECFREAKS_3_2_9341_WITH_TFT01_MEGA 
		case(ELECFREAKS_3_2_9341_WITH_TFT01_MEGA): //(ILI9481) Parameters referenced from UTFT library
			_CommandOut(0x11); //Exit sleep mode
			delay(20);


			 _CommandOut(0xD0); //Power setting
			 _DataOut(0x07); // reference voltage ratio
			 _DataOut(0x42);// reference voltage step up / start step up
			 _DataOut(0x18); //Set VRH = 4.875V

			 _CommandOut(0xD1); //Set VCOMH voltage
			 _DataOut(0x00);
			 _DataOut(0x07);
			 _DataOut(0x10);

			 _CommandOut(0xD2); //Power_Setting for Normal Mode 
			 _DataOut(0x01);
			 _DataOut(0x02);

			 _CommandOut(0xC0); //Panel Driving Setting
			 _DataOut(0x10);
			 _DataOut(0x3B);
			 _DataOut(0x00);
			 _DataOut(0x02);
			 _DataOut(0x11);

			 _CommandOut(0xC5); //Frame Rate and Inversion Control 
			 _DataOut(0x03);

			 _CommandOut(0xC8); //Gamma Setting 
			 _DataOut(0x00);
			 _DataOut(0x32);
			 _DataOut(0x36);
			 _DataOut(0x45);
			 _DataOut(0x06);
			 _DataOut(0x16);
			 _DataOut(0x37);
			 _DataOut(0x75);
			 _DataOut(0x77);
			 _DataOut(0x54);
			 _DataOut(0x0C);
			 _DataOut(0x00);
			
			_CommandOut(0x36); //Set_address_mode
			_DataOut(0xEA); //Horizontal flip, BGR order

			_CommandOut(0x3A); //Set_pixel_format
			_DataOut(0x55); //16bit/pixel (65,536 colours) 

			delay(100);
			_CommandOut(0x29); //Set_display_on 
			break;
		#endif

	}		
	
	//Deselect the display
	CSHIGH;

	//If SD card is enabled then initialise the SD card library
	#ifdef SDCARD
	_SDError = false;

	if(!SD.begin(_SDCS))
		_SDError = true;
	#endif		

	//Clear the TFT screen.
	Clear();
}


// Resets the screen by pulsing the displays reset pin
void HCTFT::Reset(void)
{
	digitalWrite(_RST, LOW);
	delay(15);
	digitalWrite(_RST, HIGH); 
	delay(15);
}



/* Write to a single pixel on the display where:
   R is the red component of the pixel. Valid values are between 0 and 255.
   G is the green component of the pixel. Valid values are between 0 and 255.
   B is the blue component of the pixel. Valid values are between 0 and 255. 
   Note that although RGB colour is specified as 24bit, it is scaled down to 16 bit */
void HCTFT::Plot(byte R, byte G, byte B)
{

	//Scale 24 bit colour down to 16 bit.
	R = R >> 3;
	G = G >> 2;
	B = B >> 3;
	
	CSLOW;
	DCHIGH;
	//2.4" MCU friend display data bus is split across to ports so is a special case.
	#ifdef MCUFRIEND_2_4_UNO
	if(_Display == MCUFRIEND_2_4_UNO)
	{	
		_DataOut((R << 3) | (G >> 3));
		_DataOut((G << 5) | (B & 0x1F));
	
	}else
	#endif	
	{
		_DataOut((unsigned int)((R<<11) | ((G<<5) & 0x07E0) | (B & 0x1F)));
	}
	
	CSHIGH;
}


/* Draw a line using foreground colour where:
   X1 is the starting X axis coordinate of the line
   Y1 is the starting Y axis coordinate of the line
   X2 is the starting X axis coordinate of the line
   Y2 is the starting Y axis coordinate of the line */
void HCTFT::Line(int X1, int Y1, int X2, int Y2)
{
  double step;
  int x, y;

  /* If start coordinates are below and to the right of the end coordinate then flip them */
  CSLOW;
  if((X2 <= X1 && Y2 <= Y1) || (X2 >= X1 && Y1 >= Y2) || (X2 >= X1 && Y1 >= Y2)) 
  {
    X2^=X1;
    X1^=X2;
    X2^=X1;
    Y2^=Y1;
    Y1^=Y2;
    Y2^=Y1;
  }

  /* If X axis is wider than Y axis then step though X axis */
  if(((X2-X1) >= (Y2-Y1)) || ((X1 - X2) >= (Y2-Y1)))
  {
   	step = (double)(Y2-Y1) / (double)(X2-X1);
    if(X2 >= X1 && Y2 >= Y1)
    {
		for(x = X1; x <= X2; x++)
		{
			y = ((x-X1) * step) + Y1;
			_SetWriteArea(x, y, x, y);
			Plot(_FGR, _FGG, _FGB);
		}
    }else
    {
		for(x = X1; x >= X2; x--)
		{
			y = Y2 + ((x-X2) * step);

			_SetWriteArea(x, y, x, y);
			Plot(_FGR, _FGG, _FGB);
		}	
    } 
  }else /* If Y axis is wider than X axis then step though Y axis */
  {
  	step = (double)(X2-X1) / (Y2-Y1);
    if(Y2 >= Y1 && X2 >= X1)
    {
		for(y = Y1; y <= Y2; y++)
		{
			x = ((y-Y1) * step) + X1;
			_SetWriteArea(x, y, x, y);
			Plot(_FGR, _FGG, _FGB);
		}		
    }else
	{
		for(y = Y2; y >= Y1; y--)
		{
			x = X2 + ((y-Y2) * step);
			_SetWriteArea(x, y, x, y);
			Plot(_FGR, _FGG, _FGB);
		}
	}
  }
  CSHIGH;
}



/* Draw a rectangle using foreground colour where:
   X1 is the starting X axis coordinate of the first corner.
   Y1 is the starting Y axis coordinate of the first corner.
   X2 is the X axis coordinate of the opposite corner.
   Y2 is the Y axis coordinate of the opposite corner. 
   FillMode has the following options:
   OUTLINE (draw an outlined rectangle of 1 pixel width.
   SOLID (draw a filled rectangle using the foreground colour) */
void HCTFT::Rect(unsigned int X1, unsigned int Y1, unsigned int X2, unsigned int Y2, byte FillMode)
{
	unsigned int indexX, indexY;
	CSLOW;
	
	/* If first corner is below second corner then flip the coordinates */
	if(Y1 > Y2)
	{
		Y2^=Y1;
		Y1^=Y2;
		Y2^=Y1;  
	}
  
	if(X1 > X2)
	{
		X2^=X1;
		X1^=X2;
		X2^=X1;  
	}

	switch(FillMode)
	{
		case(SOLID):
			_SetWriteArea(X1, Y1, X2, Y2);

			for(indexY = Y1; indexY <= Y2; indexY++)
				for(indexX = X1; indexX <= X2; indexX++)	
					Plot(_FGR, _FGG, _FGB);
			break;
		
		case(OUTLINE):
			Line(X1, Y1, X2, Y1);
			Line(X1, Y2, X2, Y2);

			/* If rectangle is wider than two pixels set Y for drawing vertical borders */
			if(Y2-Y1 > 1)
			{
				Y1++;
				Y2--;
			}
 
			/* If box is solid then fill area between top and bottom border */
			if(FillMode == SOLID)
			{
				for(indexY = Y1; indexY <= Y2; indexY++)
				Line(X1, indexY, X2, indexY);
			}else  /* if not solid then just draw vertical borders */
			{
				Line(X1, Y1, X1, Y2);
				Line(X2, Y1, X2, Y2); 
			}	
		break;
	}

	CSHIGH;
}


/* Clears the display using the background colour */
void HCTFT::Clear(void)
{
	byte TempFGR = _FGR;
	byte TempFGG = _FGG;
	byte TempFGB = _FGB;
	
	_FGR = _BGR;
	_FGG = _BGG;
	_FGB = _BGB;

	Rect(0, 0, pgm_read_word_near(&MAX_X[_Display]), pgm_read_word_near(&MAX_Y[_Display]), SOLID);
	_FGR = TempFGR;
	_FGG = TempFGG;
	_FGB = TempFGB;	
}

/* Clears a rectangular area of the screen using hte background colour where:
   X1 is the starting X axis coordinate of the first corner.
   Y1 is the starting Y axis coordinate of the first corner.
   X2 is the X axis coordinate of the opposite corner.
   Y2 is the Y axis coordinate of the opposite corner. */
void HCTFT::Erase(unsigned int X1, unsigned int Y1, unsigned int X2, unsigned int Y2)
{
	byte TempFGR = _FGR;
	byte TempFGG = _FGG;
	byte TempFGB = _FGB;
	
	_FGR = _BGR;
	_FGG = _BGG;
	_FGB = _BGB;

	Rect(X1, Y1, X2, Y2, SOLID);

	_FGR = TempFGR;
	_FGG = TempFGG;
	_FGB = TempFGB;	
}

/* Sets the foreground colour used by the graphic and bitmap functions where:
   R is the red component of the pixel. Valid values are between 0 and 255.
   G is the green component of the pixel. Valid values are between 0 and 255.
   B is the blue component of the pixel. Valid values are between 0 and 255. 
   Note that although RGB colour is specified as 24bit, it is scaled down to 16 bit */
void HCTFT::SetFG(byte R, byte G, byte B)
{
	_FGR = R;
	_FGG = G;
	_FGB = B;
}

/* Sets the background colour used by the Clear() and Erase():
   R is the red component of the pixel. Valid values are between 0 and 255.
   G is the green component of the pixel. Valid values are between 0 and 255.
   B is the blue component of the pixel. Valid values are between 0 and 255. 
   Note that although RGB colour is specified as 24bit, it is scaled down to 16 bit */
void HCTFT::SetBG(byte R, byte G, byte B)
{
	_BGR = R;
	_BGG = G;
	_BGB = B;
}

/* Returns the maximum X axis coordinate of type unsigned int for the currently selected display. */
unsigned int HCTFT::Max_X(void)
{
	return pgm_read_word_near(&MAX_X[_Display]);
}

/* Returns the maximum Y axis coordinate of type unsigned int for the currently selected display. */
unsigned int HCTFT::Max_Y(void)
{
	return pgm_read_word_near(&MAX_Y[_Display]);
}


/* Sets the location of the cursor for text and bitmap graphics where:
   X and Y are the starting top left X & Y axis coordinates */
void HCTFT::Cursor(unsigned int X, unsigned int Y)
{
  _X1Pos = X;
  _Y1Pos = Y;
  _X2Pos = X;
  _Y2Pos = Y;
  CSLOW;
  _SetWriteArea(_X1Pos, _Y1Pos, _X2Pos, _Y2Pos);
  CSHIGH;
}


/* Sets a scaling factor used by the text and bitmap functions where:
   Scale is the scaling factor. Valid values are between 1 (no scaling) and 255. */
void HCTFT::Scale(byte Scale)
{
	_BMScale = Scale;
	if (_BMScale == 0)
		_BMScale = 1;
}


/* Write BW bitmap data to the LCD starting at the cursor location where:
   Cols is the number byte columns to write to.
   ByteRows is the number of rows to write to in 8 pixel chunks 
   BitmapData is an array containing the bitmap data to be written */
void HCTFT::BWBitmap(int Cols, byte ByteRows, const byte BitmapData[])
{
	unsigned int XIndex, YIndex, BitmapIndex;
	unsigned int X, Y, X_MAX, Y_MAX;
	byte PixIndex;
	
	X_MAX = pgm_read_word_near(&MAX_X[_Display]);
	Y_MAX = pgm_read_word_near(&MAX_Y[_Display]);

	/* Step through each column */
	for (XIndex = 0; XIndex < Cols * _BMScale; XIndex++)
	{
		/* Step through each 8 pixel row */
		for (YIndex = 0; YIndex  < ByteRows * _BMScale; YIndex+= _BMScale)
		{	
			BitmapIndex = ((YIndex / _BMScale) * Cols) + (XIndex / _BMScale);
			for(PixIndex=0; PixIndex < (8 * _BMScale); PixIndex++)
			{
				if((pgm_read_byte_near(&BitmapData[BitmapIndex]) >> (PixIndex / _BMScale))  & 0x01)
				{
					X = XIndex + _X1Pos;
					Y = ((YIndex * 8) + PixIndex ) + _Y1Pos;

					if (X < X_MAX && Y < Y_MAX)
					{
						_SetWriteArea(X, Y, X, Y);
						Plot(_FGR, _FGG, _FGB);
					}
				}		
			}	
		}
	}
}


/* Print a text string to the LCD starting at the cursor coordinates where:
   TextString[] is a string array containing the text to be displayed */
void HCTFT::Print(char TextString[])
{
  byte StringLength;
  byte Index;
  
  /* Finds length of string */
  StringLength = strlen(TextString) - 1;
  CSLOW;
  for (Index = 0; Index <= StringLength; Index++)
  {
	_WriteChar(TextString[Index] - 32);
  }
  CSHIGH;
}

/* Print a signed integer number to the LCD at the current cursor 
   coordinates where:
   Value is signed integer number of type long */
void HCTFT::Print(long Value)
{
  byte Digits[10];
  int long Temp;
  byte NumDigits = 0;
  CSLOW;
  /* Is the number negative ? */
  if (Value < 0)
  {
	_WriteChar(13);
	Temp = Value * -1;
  }else
  {
    Temp = Value;
  }
  
  /* Store each digit in a byte array so that they 
     can be printed in reverse order */
  while (Temp)
  {
    Digits[NumDigits] = Temp % 10;
	Temp /= 10;
	NumDigits++;
  } 

  /* Print each digit */
  while(NumDigits)
  {
	NumDigits--;
	_WriteChar(Digits[NumDigits] + 16);
  }
  CSHIGH;
}


/* Prints a floating point number to the LCD starting at the cursor coordinates where:
   value is the floating point value to display (max 10 digits including decimal places)
   digits is the number of digits to crop the number to (including decimal places)
   DecimalPlaces is the number of decimal places to display */
void HCTFT::Print(float value, byte digits, byte DecimalPlaces)
{
  char Buffer[10];
  
  /* Clip the number of digits to 10 */
  if (digits > 10)
   digits = 10;

  /* Convert the value to an character array */ 
  dtostrf(value, digits, DecimalPlaces, Buffer);
  
  /* Output the array to the display buffer */
  Print(Buffer);
}



/* Write a single character or digit at the current cursor coordinate */
void HCTFT::_WriteChar(char character)
{
    const byte *FontByteAddress;
    int FontWidth;
	
	FontByteAddress = _FontType + pgm_read_word_near(_FontDescriptor + (character * 2) + 1);
	FontWidth = pgm_read_word_near(_FontDescriptor + (character * 2));
	BWBitmap(FontWidth, _FontHight, FontByteAddress);  
	_X1Pos = _X1Pos + (FontWidth * _BMScale) + (2 * _BMScale);
	_X2Pos = _X1Pos;
	
}

/* Sets the font to be used when writing text or numbers where:
   *Font is the font type to be used. Options are:
   Terminal_8pt (A small 8x8 fixed width font)
   MedProp_11pt (A medium two row proportional font)
   LargeProp_25pt (A large four row proportional font)
   LCDLarge_24pt (A large 4 row LCD style font)
   LCDLarge_52pt (A large 8 row LCD style font)   */
void HCTFT::SetFont(const byte *Font)
{
	#ifdef TERMINAL_8PT 
	if(Font == Terminal_8pt)
	{
		_FontType = Terminal_8pt;
		_FontHight = Terminal_8ptFontInfo.CharacterHeight;
		_FontDescriptor = Terminal_8ptFontInfo.Descriptors;
	}
	#endif
	#ifdef MEDPROP_11PT
	if(Font == MedProp_11pt)
	{
		_FontType = MedProp_11pt;
		_FontHight = MedProp_11ptFontInfo.CharacterHeight;
		_FontDescriptor = MedProp_11ptFontInfo.Descriptors;
	}
	#endif
	#ifdef LARGEPROP_25PT
	if(Font == LargeProp_25pt)
	{
		_FontType = LargeProp_25pt;
		_FontHight = LargeProp_25ptFontInfo.CharacterHeight;
		_FontDescriptor = LargeProp_25ptFontInfo.Descriptors;
	}
	#endif
	#ifdef LCDLARGE_24PT
	if(Font == LCDLarge_24pt)
	{
		_FontType = LCDLarge_24pt;
		_FontHight = LCDLarge_24ptFontInfo.CharacterHeight;
		_FontDescriptor = LCDLarge_24ptFontInfo.Descriptors;
	}
	#endif
	#ifdef LCDLARGE_52PT
	if(Font == LCDLarge_52pt)
	{
		_FontType = LCDLarge_52pt;
		_FontHight = LCDLarge_52ptFontInfo.CharacterHeight;
		_FontDescriptor = LCDLarge_52ptFontInfo.Descriptors;
	}
	#endif
}


/* Writes a command to the displays data bus */
void HCTFT::_CommandOut(byte LByte)
{
	DCLOW;
		
	#ifdef MCUFRIEND_2_4_UNO
	if(_Display == MCUFRIEND_2_4_UNO)
	{
		DATAHIGH = ((DATAHIGH & ~DMASK_H) | LByte);
	}else
	#endif
	{
		DATAHIGH = (DATAHIGH & ~DMASK_H);
	}
		
	#ifdef ELECFREAKS_2_4_WITH_TFT01_UNO	
	if(_Display == ELECFREAKS_2_4_WITH_TFT01_UNO)
	{
		WRLOW;
		WRHIGH;	
	}
	#endif
	
	DATALOW = (DATALOW & ~DMASK_L) | (LByte & DMASK_L);

	WRLOW;
	WRHIGH;
}


/* Writes data to the displays data bus */
void HCTFT::_DataOut(unsigned int Data)
{
	byte DataH, DataL;

	#ifdef MCUFRIEND_2_4_UNO
	if(_Display == MCUFRIEND_2_4_UNO)
	{
		DataH = Data;
	}else
	#endif
	{
		DataH = Data >> 8;
	}
	DataL = Data;

	DCHIGH;
	
	DATAHIGH = (DATAHIGH & ~DMASK_H) | (DataH & DMASK_H) /*>> 8*/;
	#ifdef ELECFREAKS_2_4_WITH_TFT01_UNO
	if(_Display == ELECFREAKS_2_4_WITH_TFT01_UNO)
	{
		WRLOW;
		WRHIGH;	
	}
	#endif

	DATALOW = (DATALOW & ~DMASK_L) | (DataL & DMASK_L) /*& 0xFF*/;
	
	WRLOW;
	WRHIGH;
}


/* Loads a 24bit bitmap image from the SD card and automatically displays 
   it to the screen as the current cursor coordinates where:
   FileName[] is a string containing the filename of the bitmap image to load. */
#ifdef SDCARD
boolean HCTFT::Bitmap2TFT(char FileName[])
{
	unsigned int BMID;
	unsigned long BMFileSize, BMWidth, BMHight;
	byte R,G,B;
	unsigned int X1, Y1;

  
	if (!_SDError)  
		/* Check if the text file exists */
		if(!SD.exists(FileName))
			_SDError = true;
     
	if(!_SDError)
		/* The file exists so open it */
		_SDFileData = SD.open(FileName); 
    
	if(!_SDError)
	{  
		/* Check if file is a bitmap */
		BMID = (_SDFileData.read() << 8) | _SDFileData.read();
		if (BMID != 0x424D)
		_SDError = true;
	}
  
	if(!_SDError)
	{
		/* Get the size of the image so we know how much data to load */
		BMFileSize = ((unsigned long)_SDFileData.read()) | ((unsigned long)_SDFileData.read() << 8) | ((unsigned long)_SDFileData.read() << 16) | ((unsigned long)_SDFileData.read() << 24);
		_SDFileData.seek(0x0012);
		BMWidth = ((unsigned long)_SDFileData.read()) | ((unsigned long)_SDFileData.read() << 8) | ((unsigned long)_SDFileData.read() << 16) | ((unsigned long)_SDFileData.read() << 24);
		_SDFileData.seek(0x0016);
		BMHight = ((unsigned long)_SDFileData.read()) | ((unsigned long)_SDFileData.read() << 8) | ((unsigned long)_SDFileData.read() << 16) | ((unsigned long)_SDFileData.read() << 24);   

		CSLOW;
		
		/* Read the bitmap image data and write it straight to the display */
		for (Y1 = _Y1Pos; Y1 < BMHight + _Y1Pos; Y1++)
		{
			BMFileSize -= (BMWidth * 3); 
			_SDFileData.seek(BMFileSize); 
			for(X1 = _X1Pos; X1 < BMWidth + _X1Pos; X1++)
			{      
				_SetWriteArea(X1, Y1, X1, Y1);
				B = _SDFileData.read();
				G = _SDFileData.read();
				R = _SDFileData.read();
				Plot(R,G,B);
			}  
		}
    _SDFileData.close(); 
	}
    
	CSHIGH;
	
	return !_SDError;
}
#endif


/* Writes 24bit (automatically scaled down to 16bit) bitmap data to the LCD starting at the cursor location where:
   Cols is the X axis dimension of the image data.
   Rows is the Y axis dimension of the image data. 
   BitmapData is an array of type unsigned int containing the bitmap data to be written where each value represents
   the 24 bit RGB colour of each pixel. Each RGB pixel value is written to the display in sequence from the first row to the end row. */
void HCTFT::RGBBitmap(unsigned int Cols, unsigned int Rows, const unsigned int BitmapData[])
{
	unsigned int XIndex, YIndex, BitmapIndex;
	unsigned int X, Y;

	for (XIndex = 0; XIndex < Cols * _BMScale; XIndex++)
	{
		for (YIndex = 0; YIndex < Rows * _BMScale; YIndex++)
		{	
			BitmapIndex = ((YIndex / _BMScale) * Cols) + (XIndex / _BMScale);
			X = XIndex + _X1Pos;
			Y = YIndex + _Y1Pos;
			
			_SetWriteArea(X, Y, X, Y);
			CSLOW;
				#ifdef MCUFRIEND_2_4_UNO
				if(_Display == MCUFRIEND_2_4_UNO)
				{
					_DataOut(pgm_read_word_near(&BitmapData[BitmapIndex]) >> 8);
					_DataOut(pgm_read_word_near(&BitmapData[BitmapIndex]) & 0xFF);
				}else
				#endif			
				{	
					_DataOut(pgm_read_word_near(&BitmapData[BitmapIndex]));
				}
				
		}
	}
	CSHIGH;
}


/* Sets the area of the display to write to */
void HCTFT::_SetWriteArea(unsigned int X1, unsigned int Y1, unsigned int X2, unsigned int Y2)
{
	unsigned int XMAX = pgm_read_word_near(&MAX_X[_Display]);
	unsigned int YMAX = pgm_read_word_near(&MAX_Y[_Display]);
	
	if((X2 <= XMAX) && (Y2 <= YMAX))
	{
		CSLOW;

		switch(_Display)
		{
			#ifdef MCUFRIEND_3_5_MEGA
			case(MCUFRIEND_3_5_MEGA):
				_CommandOut(0x2a); 
				_DataOut(X1>>8);
				_DataOut(X1);
				_DataOut(X2>>8);
				_DataOut(X2);
				_CommandOut(0x2b); 
				_DataOut(Y1>>8);
				_DataOut(Y1);
				_DataOut(Y2>>8);
				_DataOut(Y2);
				_CommandOut(0x2c); 
				break;
			#endif
			
			#ifdef ELECFREAKS_3_2_WITH_TFT01_MEGA			 
			case(ELECFREAKS_3_2_WITH_TFT01_MEGA):
				_CommandOut(0x44);
				_DataOut((Y2<<8) | (Y1 & 0xFF));
				_CommandOut(0x45);
				_DataOut(XMAX - X2);
				_CommandOut(0x46);
				_DataOut(XMAX - X1);
				_CommandOut(0x4E);
				_DataOut(Y1);
				_CommandOut(0x4F);
				_DataOut(XMAX - X1);
				_CommandOut(0x22);			 		 
				break;
			#endif
			
			#ifdef ELECFREAKS_2_4_WITH_TFT01_UNO			 
			case(ELECFREAKS_2_4_WITH_TFT01_UNO):
				_CommandOut(0x20);
				_DataOut(Y1);
				_CommandOut(0x21);
				_DataOut(XMAX - X1);
				_CommandOut(0x50);
				_DataOut(Y1);
				_CommandOut(0x52);
				_DataOut(XMAX - X2);
				_CommandOut(0x51);
				_DataOut(Y2);
				_CommandOut(0x53);
				_DataOut(XMAX - X1);
				_CommandOut(0x22);	 
				break;
			#endif
			
			#ifdef MCUFRIEND_2_4_UNO	 
			case(MCUFRIEND_2_4_UNO):	 
				_CommandOut(0x2A); //column
				_DataOut((YMAX - Y2) >> 8);
				_DataOut(YMAX -Y2);
				_DataOut((YMAX - Y1) >> 8);
				_DataOut(YMAX - Y1);
				_CommandOut(0x2B); //page
				_DataOut((XMAX - X2) >> 8);
				_DataOut(XMAX - X2);
				_DataOut((XMAX - X1) >>8);
				_DataOut(XMAX - X1);
				_CommandOut(0x2C); //write
				break;
			#endif
			
			#ifdef ELECFREAKS_3_2_9341_WITH_TFT01_MEGA
			case(ELECFREAKS_3_2_9341_WITH_TFT01_MEGA):		
				_CommandOut(0x2a); 
				_DataOut(X1>>8);
				_DataOut(X1);
				_DataOut(X2>>8);
				_DataOut(X2);
				_CommandOut(0x2b); 
				_DataOut(Y1>>8);
				_DataOut(Y1);
				_DataOut(Y2>>8);
				_DataOut(Y2);
				_CommandOut(0x2c); 
				break;
			#endif
		}	
	CSHIGH;
	}
}

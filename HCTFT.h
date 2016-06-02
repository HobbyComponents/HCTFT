/* FILE:    HCTFT.h
   DATE:    02/06/16
   VERSION: 0.2
   AUTHOR:  Andrew Davies

15/06/15 version 0.1: Original version
02/06/16 version 0.2: Updated to include Elec Freaks 3.2" display with ILI9341 contoller
   
   
Library header for TFT screens. Currently supported screens:

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

#ifndef HCTFT_h
#define HCTFT_h

#include "Arduino.h"
#include <avr/pgmspace.h>
#include "MemorySave.h"


#ifdef SDCARD
	#include "SPI.h"
	#include "SD.h"
#endif 

/* If using an UNO create a fake PORTA (used by Mega) so that library can compile */
#if defined (__AVR_ATmega328P__)
	#define PORTA (* (volatile unsigned char *) 0xFF)
	#define DDRA (* (volatile unsigned char *) 0xFF)
#endif

/* Macros from control pins */
#define WRLOW	*_WRP &= ~_WRM
#define WRHIGH	*_WRP |= _WRM
#define DCLOW *_DCP &= ~_DCM
#define DCHIGH *_DCP |= _DCM
#define CSLOW *_CSP &= ~_CSM
#define CSHIGH *_CSP |= _CSM

/* Macros for data bus */
#define DATAHIGH *DPH[_Display]
#define DATALOW *DPL[_Display]
#define DMASK_H pgm_read_byte_near(&DHMASK[_Display])
#define DMASK_L pgm_read_byte_near(&DLMASK[_Display])


/* Various draw modes */
enum HCTFTDrawModes
{
//	NORMAL    = 1,
//	INVERT    = 2,
	CLEAR     = 3,
	OUTLINE   = 4,
	SOLID     = 5	
};

#define INVERT true
#define NORMAL false


/* Some useful predefined colours */
#define BLACK	0,0,0
#define	WHITE	255,255,255
#define RED		255,0,0
#define GREEN	0,255,0
#define BLUE	0,0,255
#define YELLOW	255,255,0
#define CYAN 	0,255,255
#define MAGENTA 255,0,255
#define SILVER	192,192,192
#define GRAY	128,128,128
#define MAROON	128,0,0
#define OLIVE	128,128,0
#define GREEN	0,128,0
#define PURPLE	128,0,128
#define TEAL	0,128,128
#define NAVY	0,0,128


/* Available fonts */
extern const PROGMEM byte Terminal_8pt[];
extern const PROGMEM byte MedProp_11pt[];
extern const PROGMEM byte LargeProp_25pt[];
extern const PROGMEM byte LCDLarge_24pt[];
extern const PROGMEM byte LCDLarge_52pt[];


class HCTFT
{
	public:
	HCTFT(byte DisplayType);
	void Cursor(unsigned int X, unsigned int Y);
	void Scale(byte Scale);
	void Plot(byte R, byte G, byte B);
	void Line(int X1, int Y1, int X2, int Y2);
	void Rect(unsigned int X1, unsigned int Y1, unsigned int X2, unsigned int Y2, byte FillMode);
	void BWBitmap(int Cols, byte ByteRows, const byte BitmapData[]);
	void RGBBitmap(unsigned int Cols, unsigned int Rows, const unsigned int BitmapData[]);
	#ifdef SDCARD
	boolean Bitmap2TFT(char FileName[]);
	#endif
	void Print(char TextString[]);
	void Print(long Value);
	void Print(float value, byte digits, byte DecimalPlaces);
	void SetFont(const byte *Font);
	void SetFG(byte R, byte G, byte B);
	void SetBG(byte R, byte G, byte B);
	unsigned int Max_X(void);
	unsigned int Max_Y(void);
	void Reset(void);
	void Init(void);
	void Clear(void);
	void Erase(unsigned int X1, unsigned int Y1, unsigned int X2, unsigned int Y2);

	
	private:
	void _SetWriteArea(unsigned int X1, unsigned int Y1, unsigned int X2, unsigned int Y2);
	void _CommandOut(byte LByte);
	void _DataOut(unsigned int Data);
	void _WriteChar(char character);
	byte _RST;
	volatile uint8_t* _WRP;
	byte _WRM;
	volatile uint8_t* _CSP;
	byte _CSM;
	volatile uint8_t* _DCP;
	byte _DCM;	
	unsigned int _X1Pos;
	unsigned int _Y1Pos;
	unsigned int _X2Pos;
	unsigned int _Y2Pos;
	byte _FGR;
	byte _FGG;
	byte _FGB;
	byte _BGR;
	byte _BGG;
	byte _BGB;
	byte _Display;
	const unsigned int *_FontDescriptor;
	const byte *_FontType;
	byte _FontHight;
	byte _BMScale;
	#ifdef SDCARD
	File _SDFileData;
	boolean _SDError;
	byte _SDCS;
	#endif	
}; 
#endif
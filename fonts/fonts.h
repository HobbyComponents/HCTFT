#ifndef fonts_h
#define fonts_h
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "MemorySave.h"


typedef struct _font_info 
{ 
   byte CharacterHeight; 
   char StartCharacter; 
   char EndCharacter; 
   const unsigned int *Descriptors; 
   const byte *Bitmaps; 
   //int Bitmaps;
}FONT_INFO; 


#include "Fonts/Terminal_8pt.h"
#include "Fonts/MedProp_11pt.h"
#include "Fonts/LargeProp_25pt.h"
#include "Fonts/LCDLarge_24pt.h"
#include "Fonts/LCDLarge_52pt.h"


#endif
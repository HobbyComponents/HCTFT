/* FILE:    HCTFT_Example_RGB_Bitmaps
   DATE:    17/06/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

This sketch demonstrates the use of the HCTFT.RGBBitmap() function to load
16 bit colour images on the screen directly from program memory. This function
is useful if you want an easy way of displaying icons in your application without
needing to use an SD card for storage.

Generating your own bitmaps:
To help in creating your own bitmaps included with the HCTFT library is a sketch 
called HCTFT_RGB_Bitmap_Generator. This sketch will use your Arduino to read a 
bitmap file from an SD card and generate the appropriate code to use within your 
sketch. 

IF YOU GET AN OUT OF MEMORY COMPILE ERROR:
Library support for TFT screens can be quite memory intensive. If you are using 
an UNO you may find that you will get and out of memory compile error. To get the
sketch to compile you will need to edit the MemorySave.h file in the HCTFT folder
to free up some memory.


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


/* Include the HCTFT and HCTOUCH libraries */
#include "HCTFT.h"
#include "Example_Bitmaps.h"


/* Uncomment one of the lines below to to create an instance of the TFT library 
   to match your screen */
//HCTFT HCTFT(ELECFREAKS_2_4_WITH_TFT01_UNO); // Elecfreaks 2.4" TFT (HCARDU0083)
//HCTFT HCTFT(ELECFREAKS_3_2_WITH_TFT01_MEGA); // Elecfreaks 3.2" TFT with ILI9481 contoller (HCARDU0097)
//HCTFT HCTFT(ELECFREAKS_3_2_9341_WITH_TFT01_MEGA); // Elecfreaks 3.2" TFT with ILI9341 controller (HCARDU0097)
//HCTFT HCTFT(MCUFRIEND_2_4_UNO); // McuFriend 2_4" TFT (HCARDU0095)
//HCTFT HCTFT(MCUFRIEND_3_5_MEGA); // Mcufriend 3.5" TFT (HCARDU0096)


void setup() 
{
  /* Initialise the TFT library */
  HCTFT.Init();
}


/* Main sketch */
void loop() 
{
  byte index, XStep, YStep;

    
  YStep = 0;
  /* Display 3 rows of icons, each time making them bigger */
  for (index = 1; index <= 3; index++)
  {
    XStep = 45 * index; 
    YStep = YStep + (50 * (index - 1));
    HCTFT.Scale(index);
    
    HCTFT.Cursor(0,YStep);
    HCTFT.RGBBitmap(44, 45, Alarm);

    HCTFT.Cursor(XStep,YStep);
    HCTFT.RGBBitmap(40, 45, Bulb);

  
    HCTFT.Cursor(XStep * 2,YStep);
    HCTFT.RGBBitmap(32, 40, Calc);
  
    HCTFT.Cursor(XStep * 3, YStep);
    HCTFT.RGBBitmap(40, 40, Clock);
  }

  /* Nothing else to do! */
  while(1);
}

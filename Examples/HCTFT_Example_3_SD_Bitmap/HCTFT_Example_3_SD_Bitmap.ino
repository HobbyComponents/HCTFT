/* FILE:    HCTFT_Example_3_SD_Bitmap
   DATE:    17/06/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

This example will require a compatible TFT screen with SD card reader. The
sketch demonstrates the Bitmap2TFT() function that will load any 24 bit colour
bitmap image directly from the root directory of an SD card and display it 
to the screen at the specified coordinates. The sketch assumes you already 
have a bitmap loaded onto the file named "test.bmp". However you can change the 
file name in the sketch to anything you like.

This function is useful if you need to load multiple images, for examples icons
as these can be stored on the SD card freeing up memory in your Arduino.

PLEASE NOTE: By default this sketch will not compile. This is due to the library 
SD card functions being disabled to save memory. To get this example to compile
you will need to uncomment the following line in the MemorySave.h file found in 
the HCTFT library folder:

//#define SDCARD

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
#include "SPI.h"
#include "SD.h"


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
  /* Set the cursor to the top left corner of the screen */
  HCTFT.Cursor(0,0);
  /* Load the bitmap from the SD card and display it on the screen */
  HCTFT.Bitmap2TFT("test.bmp");
  
  /* That's it, nothing else to do */ 
  while(1);
}

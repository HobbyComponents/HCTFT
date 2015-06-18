/* FILE:    HCTFT_RGB_Bitmap_Generator
   DATE:    17/06/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

This sketch will use your Arduino to read a bitmap file from an SD card and generate 
the appropriate code to store the image directly within your sketch using the
HCTFT.RGBBitmap() library function. Although the HCTFT library can load and display 
images from SD cards directly this function is useful if you want an easy way of 
displaying icons in your application without needing to use an SD card for storage.

To use this sketch you will need to save a 24 bit bitmap image to an SD card. You
can name the file anything you like but the file name must be 8 characters or less.
Next change the following line in the sketch to match the name of your bitmap:

#define FILENAME "Example.bmp"

Download the sketch and open a serial monitor window and the appropriate code will
be generated. This can then be cut and pasted into your own sketch. 


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


#include "SPI.h"
#include "SD.h"

/* CHANGE THIS TO MATCH THE FILE NAME OF YOUR BITMAP IMAGE */
#define FILENAME "Example.bmp"

/* CHANGE THIS TO WHATEVER YOU WIST TO CALL THE IMAGE IN YOUR SKETCH */
#define BITMAPNAME "ExampleBitmap"

/* Set this to whatever pin the Chip Select (CS) of your SD card reader is 
   connected to. This is usually pin 10 on an Uno and pin 53 on a Mega. */
#define CS_PIN 10
//#define CS_PIN 53


boolean Error = false;

File SDFileData;


void setup()
{
  Serial.begin(9600);
  if(!SD.begin(CS_PIN))
    Error = true;

  if(Error)
    Serial.println("Error starting SD card library");
}


void loop() 
{
  byte R,G,B;
    unsigned int BMID;
    unsigned long BMFileSize /*, BMStart*//*, BMDIBHeaderSize*/, BMWidth, BMHight;
    unsigned int X1,/*X2,*/Y1/*,Y2*/;
    
  if (!Error)  
   /* Check if the text file exists */
   if(!SD.exists(FILENAME))
   {
     Error = true;
     Serial.println("Error: File not found");
   }
   
  if(!Error)
    /* The file exists so open it */
    SDFileData = SD.open(FILENAME); 

  if(!Error)
  {  
    /* Check if file is a bitmap */
    BMID = (SDFileData.read() << 8) | SDFileData.read();
    if (BMID != 0x424D)
    {
      Error = true;
      Serial.println("Error: File is not a bitmap");
    }
  }

  if(!Error)
  {
    /* Get size of bitmap file */
    BMFileSize = ((unsigned long)SDFileData.read()) | ((unsigned long)SDFileData.read() << 8) | ((unsigned long)SDFileData.read() << 16) | ((unsigned long)SDFileData.read() << 24);
    
    /* Get image width in pixels */
    SDFileData.seek(0x0012);
    BMWidth = ((unsigned long)SDFileData.read()) | ((unsigned long)SDFileData.read() << 8) | ((unsigned long)SDFileData.read() << 16) | ((unsigned long)SDFileData.read() << 24);
    
    /* Get image width in pixels */
    SDFileData.seek(0x0016);
    BMHight = ((unsigned long)SDFileData.read()) | ((unsigned long)SDFileData.read() << 8) | ((unsigned long)SDFileData.read() << 16) | ((unsigned long)SDFileData.read() << 24);   

    Serial.print("const PROGMEM unsigned int ");
    Serial.print(BITMAPNAME);
    Serial.println("[] = ");
    Serial.print("{");

    /* Read the bitmap data and output it to the serial port */
    for (Y1 = 0; Y1 < BMHight; Y1++)
    {
      BMFileSize -= (BMWidth * 3);   
      SDFileData.seek(BMFileSize); 
      Serial.println();
      for(X1 = 0; X1 < BMWidth; X1++)
      {  
        B = SDFileData.read() >> 3;
        G = SDFileData.read() >> 2;
        R = SDFileData.read() >> 3;
        Serial.print("0x");
        Serial.print((unsigned int)((R << 11) | ((G <<5)  & 0x0FC0) | (B & 0x1F)),HEX);
        Serial.print(",");   
      }	  
    }
    
    /* Close the file and generate the code to use the image in a sketch */
    SDFileData.close(); 
    Serial.println();  
    Serial.println("};");
    Serial.println();  
    Serial.println();  
    Serial.println("Use the following line in your program to draw the bitmap:");
    Serial.print("HCTFT.RGBBitmap(");
    Serial.print(BMWidth);
    Serial.print(", ");
    Serial.print(BMHight);
    Serial.print(", ");
    Serial.print(BITMAPNAME);
    Serial.println(");");
  }
  
  /* Don't do anything else */
  while(1);
}

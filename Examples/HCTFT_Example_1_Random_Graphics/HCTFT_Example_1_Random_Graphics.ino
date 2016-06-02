/* FILE:    HCTFT_Example_1_Random_Graphics
   DATE:    17/06/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

   
This example will output graphics, text and bitmap data randomly to the screen.
The sketch demonstrates the use of these vairous functions including how to
set their colours and positions on the screen.

To use this sketch you will need to uncomment the appropriate initialisers below
for your particular display.

IF YOU GET AN OUT OF MEMRY COMPILE ERROR:
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



/* Include the HCTFT library */
#include "HCTFT.h"


/* Example bitmap */
const PROGMEM byte Tiny_Logo_Resistor[] =
{
  0xC0, 0xE0, 0x30, 0x30, 0xF0, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0x38, 0x0E, 0x07, 0x1E, 0x78, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x70, 0x1C, 0x07, 0x0F, 0x3C, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0xF0, 0x3C, 0x0F, 0x07, 0x1C, 0x70, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0x78, 0x1E, 0x07, 0x0E, 0x38, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xF0, 0x30, 0x30, 0xE0, 0xC0, 
  0x00, 0x01, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x3C, 0x38, 0x0E, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1E, 0x38, 0x1C, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0E, 0x38, 0x38, 0x0E, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1C, 0x38, 0x1E, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0E, 0x38, 0x3C, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x01, 0x00, 
};

/* Options used to select one of the demos */
#define LINE 0
#define RECTANGLE 1
#define FONT 2
#define BWGRAPHIC 3


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

  /* Select one of the libraries fonts and seed the Arduino random function*/
  HCTFT.SetFont(LargeProp_25pt);
  randomSeed(analogRead(0));
}


/* Main sketch */
void loop() 
{
  int Index;
  
  /* Draw lots of lines in random places and colours */
  for (Index = 0; Index < 500; Index++)
  {
    Draw_Graphic(LINE);
  }  
  HCTFT.Clear();
  
  /* Draw lots of rectangles in random places and colours */
  for (Index = 0; Index < 100; Index++)
  {
    Draw_Graphic(RECTANGLE);
  }  
  HCTFT.Clear();
  
  /* Draw the word HELLO in random sizes, places and colours */
  for (Index = 0; Index < 100; Index++)
  {
    Draw_Graphic(FONT);
  }  
  HCTFT.Clear();
  
  /* Draw an example graphic in random sizes, places and colours */
  for (Index = 0; Index < 150; Index++)
  {
    Draw_Graphic(BWGRAPHIC);
  }  
  HCTFT.Clear();

}


/* Function to randomly draw a graphic or test */
void Draw_Graphic(byte Type)
{
  /* Create some random X/Y coordinates and colours */
  int X1 = random(HCTFT.Max_X());
  int X2 = random(HCTFT.Max_X());
  int Y1 = random(HCTFT.Max_Y());
  int Y2 = random(HCTFT.Max_Y());
  byte Red = random(255);
  byte Green = random(255);
  byte Blue = random(255);
  byte Scale = random(4);
  
  /* Get the foreground colour */
  HCTFT.SetFG(Red,Green,Blue);
  
  /* Select a graphic function to draw */
  switch(Type)
  {
    /* Draw a random line */
    case(LINE):
      HCTFT.Line(X1,Y1,X2,Y2);
      break;
      
    /* Draw a random solid or outlined rectangle */ 
    case(RECTANGLE):
      if(random(2))
      {
        HCTFT.Rect(X1,Y1,X2,Y2,OUTLINE);
      }else
      {
        HCTFT.Rect(X1,Y1,X2,Y2,SOLID);
      }
      break;
      
    /* Print the text HELLO! in random sizes */  
    case(FONT):
      HCTFT.Scale(Scale);
      HCTFT.Cursor(X1,Y1);
      HCTFT.Print("HELLO!");
      break;
 
    /* Draw the example bitmap in random sizes */     
    case(BWGRAPHIC):
      HCTFT.Scale(Scale);
      HCTFT.Cursor(X1,Y1);
      HCTFT.BWBitmap(84, 2, Tiny_Logo_Resistor);
      break;       
  }
}

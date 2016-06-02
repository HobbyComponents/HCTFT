/* FILE:    HCTFT_Example_2_Draw
   DATE:    17/06/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

This example requires a compatible resistive touch TFT screen. To run this sketch 
you will need both the HCTFT and HCTFT_Touch screen libraries to be installed.

This sketch will allow you to used your displays resistive touch sensor to draw
on the screen. It also demonstrates you can use the touch screen feature to
select simple options, in this case selecting the colour to draw with.

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
#include "HCTFT_Touch.h"


/* Uncomment one of the lines below to to create an instance of the TFT library 
   to match your screen */
//HCTFT HCTFT(ELECFREAKS_2_4_WITH_TFT01_UNO); // Elecfreaks 2.4" TFT (HCARDU0083)
//HCTFT HCTFT(ELECFREAKS_3_2_WITH_TFT01_MEGA); // Elecfreaks 3.2" TFT with ILI9481 contoller (HCARDU0097)
//HCTFT HCTFT(ELECFREAKS_3_2_9341_WITH_TFT01_MEGA); // Elecfreaks 3.2" TFT with ILI9341 controller (HCARDU0097)
//HCTFT HCTFT(MCUFRIEND_3_5_MEGA); // Mcufriend 3.5" TFT (HCARDU0096)


/* Uncomment one of the lines below to to create an instance of the TFT_Touch 
   library to match your screen */
//HCTFT_Touch Touch(ELECFREAKS_2_4_WITH_TFT01_UNO); // Elecfreaks 2.4" TFT (HCARDU0083)
//HCTFT_Touch Touch(ELECFREAKS_3_2_WITH_TFT01_MEGA); // Elecfreaks 3.2" TFT (HCARDU0097)
//HCTFT_Touch Touch(MCUFRIEND_3_5_MEGA); // Mcufriend 3.5" TFT (HCARDU0096)


void setup() 
{
  /* Initialise the TFT library */
  HCTFT.Init();
}

/* Main sketch */
void loop() 
{
  int X_Coord, Y_Coord;
  
  /* Draw the menu to select a colour. */
  DrawMenu();
  
  while(1)
  {
    /* Check if the touch screen is currently pressed */
    if(TouchDebounce())
    {
      /* Read the raw data for the X and Y axis */
      Y_Coord = Touch.GetCoord(YAXIS);
      X_Coord = Touch.GetCoord(XAXIS);
      
      /* If the X xoord is less then 52 then it is in the menu area of the screen 
         otherwise it is in the drawing area. */
      if(X_Coord <= 52)
      {
        Colour(Y_Coord);
      }else
      {
        Draw(X_Coord, Y_Coord); 
      }     
    }  
  }
}


/* Draws the menu to select a colour. This will automatically scale to your 
   screens resolution */
void DrawMenu(void)
{
  //int MaxX = HCTFT.Max_X(); 
  int MaxY = HCTFT.Max_Y(); 
  int YPos = 2;
  int YStep = (MaxY/8) - 2;
  
  HCTFT.SetFG(BLACK);
  HCTFT.Rect(0,0,50,MaxY,SOLID);
  
  HCTFT.SetFG(WHITE);
  HCTFT.Rect(2, YPos, 48, YPos + YStep, OUTLINE);
  
  YPos += YStep + 2; 
  HCTFT.SetFG(WHITE);
  HCTFT.Rect(2, YPos, 48, YPos + YStep, SOLID);
  
  YPos += YStep + 2; 
  HCTFT.SetFG(RED);
  HCTFT.Rect(2, YPos, 48, YPos + YStep, SOLID);
  
  YPos += YStep + 2; 
  HCTFT.SetFG(YELLOW);
  HCTFT.Rect(2, YPos, 48, YPos + YStep, SOLID);
  
  YPos += YStep + 2; 
  HCTFT.SetFG(GREEN);
  HCTFT.Rect(2, YPos, 48, YPos + YStep, SOLID);
  
  YPos += YStep + 2; 
  HCTFT.SetFG(CYAN);
  HCTFT.Rect(2, YPos, 48, YPos + YStep, SOLID);
  
  YPos += YStep + 2; 
  HCTFT.SetFG(BLUE);
  HCTFT.Rect(2, YPos, 48, YPos + YStep, SOLID);
  
  YPos += YStep + 2; 
  HCTFT.SetFG(PURPLE);
  HCTFT.Rect(2, YPos, 48, YPos + YStep, SOLID);
}

/* Draw a large pixel on the screen */
void Draw(int X, int Y)
{
  HCTFT.Rect(X-2,Y-2,X+2,Y+2,SOLID); 
}

/* Set the foreground colour to currently selected menu option */
void Colour(int Y)
{
  Y = Y / (HCTFT.Max_Y() / 8);
  
  switch(Y)
  {
    case(0):
      HCTFT.SetFG(BLACK);
      break; 
      
    case(1):
      HCTFT.SetFG(WHITE);
      break; 
      
    case(2):
      HCTFT.SetFG(RED);
      break; 
      
    case(3):
      HCTFT.SetFG(YELLOW);
      break; 
      
    case(4):
      HCTFT.SetFG(GREEN);
      break; 
      
    case(5):
      HCTFT.SetFG(CYAN);
      break; 
      
    case(6):
      HCTFT.SetFG(BLUE);
      break; 
      
    case(7):
      HCTFT.SetFG(PURPLE);
      break;  
  } 
}


/* Simple de-bouncing function to avoid incorrect coordinates 
   when the screen is pressed. */
boolean TouchDebounce(void)
{
 boolean Flag = true;
 byte index;
 
 for(index = 0; index < 10; index++)
 {
   if(!Touch.Pressed())
     Flag = false;  
 }
  return Flag;  
}

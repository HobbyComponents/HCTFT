/******************************************************************************
							MEMORY SAVING OPTIONS.
							
Multiple screen support, bitmap data for fonts, and SD card functions can be
memory intensive. Therefore if you a experiencing an out of memory error when 
compiling your sketch you can free up program memory by commenting out any 
of the functions below that are not required by your sketch 
******************************************************************************/

/* Multiple screen support. Save memory by commenting out all but the line for 
   the screen you are currently using */

#define MCUFRIEND_3_5_MEGA 0 // 3.5" TFT display for Arduino Mega (HCARDU0096)
#define ELECFREAKS_3_2_WITH_TFT01_MEGA 1 // 3.2" TFT screen for Arduino Mega with ILI9481 (HCARDU0097)
#define ELECFREAKS_2_4_WITH_TFT01_UNO 2 // 2.4" TFT screen for Arduino Uno (HCARDU0083)
#define MCUFRIEND_2_4_UNO 3 // 2.4" TFT display for Arduino Uno (HCARDU0095)
#define ELECFREAKS_3_2_9341_WITH_TFT01_MEGA 4 // 3.2" TFT screen for Arduino Mega with ILI9341 controller (HCARDU0097)

/* If your sketch does not need to load bitmap image data from an SD card comment
   out the line below */
   
#define SDCARD

/* If your sketch does not require all of the fonts below comment out the ones you 
   do not require. Note that you need large fonts you can save further memory by
   only enabling the smaller fonts and using the Scale() function to enlarge them. */

#define TERMINAL_8PT
#define MEDPROP_11PT
#define LARGEPROP_25PT
#define LCDLARGE_24PT
//#define LCDLARGE_52PT



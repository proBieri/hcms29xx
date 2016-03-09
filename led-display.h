/*
  led-display -- controller library for Avago HCMS-297x displays -- version 0.4
  27 Jan 2010 ML: extended a bit to support up to four (4) 8 character displays.
  15 May 2014 AW: alterred setBrightness() to use 50% peak pixel current, and support cascaded displays.
  10 Nov 2014 AW: Moved the dotRegister[] array to be a public variable, this way we manipulate the pixels directly.
  05 Dec 2014 AW: Made the dotRegister[] array size a "#define" to conserve precious SRAM (see below this comment block - line 42).
 
   Copyright (c) 2009 Tom Igoe. Some right reserved.
 
   Revisions on version 0.2 and 0.3 by Mark Liebman, 27 Jan 2010
    * extended a bit to support up to four (4) 8 character displays.
   
   Revisions on version 0.4 by Andrew Wyatt, 15 May 2014
    * alterred setBrightness() to use 50% peak pixel current, and support cascaded displays.
 
   
  Controls an Avago HCMS29xx display. This display has 8 characters, each 5x7 LEDs
   
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA 
  
  Version changes:
  * Version 3 adds support for multiple displays.  Just change the displayLength to 
    the total number of characters (for example, 3 8-character displays, displayLength = 24).

  * Version 5 alters the setBrightness method for cascaded displays. It also sets the peak pixel current to 50%.
*/


// We need one byte per pixel column. Each character is 5 pixels wide, so we need 5 bytes per character.
#define DOT_REGISTER_BYTES 60 // 60 bytes = 12 characters.

 
// ensure this library description is only included once 
#ifndef LedDisplay_h
#define LedDisplay_h 

// include types & constants of Wiring core API
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Pascal Stang's 5x7 font library:
#include "font5x7.h"
// The font library is stored in prgram memory:
#include <avr/pgmspace.h>
// string functions used to manipulate strings:
#include <string.h>

// Arduino Print library provides print() and println() functionality 
#include "Print.h"

// library interface description

class LedDisplay : public Print {
  public:
    // constructor:
   LedDisplay(uint8_t _dataPin, 
              uint8_t _registerSelect, 
              uint8_t _clockPin, 
              uint8_t _chipEnable, 
              uint8_t _resetPin, 
              uint8_t _displayLength);
   

    // initializer method:
    void begin();
    
    void clear();                       // clear the display
    void home();                        // set cursor to far left hand position
    void setCursor(int whichPosition);  // set cursor to any position
    int  getCursor();                   // get the cursor position

#if ARDUINO >= 100
    virtual size_t write(uint8_t b);    // write a character to the display and advance cursor
#else
    virtual void write(uint8_t b);      // write a character to the display and advance cursor
#endif
    using Print::write;
    
    void setString(char* _stringToDisplay);     // set the displayString variable
    char* getString();                          // get the displayString
    int stringLength();                         // get the length of displayString

    void scroll(int direction);         // scroll whatever string is stored in library's displayString variable
    
    void setBrightness(uint8_t bright);         // set display brightness, 0 - 15

    // Control register setters. for addressing the display directly:
    void loadControlRegister(int dataByte);
    void loadDotRegister();
            
    int version(void);                          // return library version

    
    // I've moved the dot register to be a public variable - Andrew 2014-11-10
    uint8_t dotRegister[DOT_REGISTER_BYTES];

    // I've moved the writeCharacter function to be public - Andrew 2014-11-11
    // Character display setters:
    void writeCharacter(char whatCharacter, byte whatPosition); // write a character to a buffer which will
                                                                // be sent to the display by loadDotRegister()
  private:

    int cursorPos;              // position of the cursor       

    // Define pins for the LED display:
    uint8_t  dataPin;           // connects to the display's data in
    uint8_t registerSelect;     // the display's register select pin 
    uint8_t clockPin;           // the display's clock pin
    uint8_t chipEnable;         // the display's chip enable pin
    uint8_t resetPin;           // the display's reset pin
    uint8_t displayLength;      // number of bytes needed to pad the string
    char* displayString;        // string for scrolling
};

#endif


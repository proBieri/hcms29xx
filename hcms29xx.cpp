/*
  hcms29xx -- controller library for Avago HCMS-297x displays -- version 0.4
  
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

*/


#include "hcms29xx.h"

// Pascal Stang's 5x7 font library:
#include "font5x7.h"
// The font library is stored in program memory:
#include <avr/pgmspace.h>
#include <string.h> 

/*
 *  Constructor.  Initializes the pins and the instance variables.
 */
hcms29xx::hcms29xx(uint8_t _dataPin, 
                   uint8_t _registerSelect, 
                   uint8_t _clockPin, 
                   uint8_t _chipEnable, 
                   uint8_t _resetPin, 
                   uint8_t _displayLength)
{
    // Define pins for the LED display:
    this->dataPin = _dataPin;                   // connects to the display's data in
    this->registerSelect = _registerSelect;     // the display's register select pin 
    this->clockPin = _clockPin;                 // the display's clock pin
    this->chipEnable = _chipEnable;             // the display's chip enable pin
    this->resetPin = _resetPin;                 // the display's reset pin
    this->displayLength = _displayLength;       // number of bytes needed to pad the string
    this->cursorPos = 0;                        // position of the cursor in the display
    char stringBuffer[displayLength+1];         // default array that the displayString will point to
    
    // fill stringBuffer with spaces, and a trailing 0:
    for (int i = 0; i < displayLength; i++) {
        stringBuffer[i] = ' ';
    }
    stringBuffer[displayLength] = '\0';
    
    this->setString(stringBuffer);              // give displayString a default buffer
}

/*
 *  Initialize the display.
 */
 
void hcms29xx::begin() {
 // set pin modes for connections:
  pinMode(dataPin, OUTPUT);
  pinMode(registerSelect, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(chipEnable, OUTPUT);
  pinMode(resetPin, OUTPUT);

  // reset the display:
  digitalWrite(resetPin, LOW);
  delay(10);
  digitalWrite(resetPin, HIGH);

  // load dot register with lows
  loadDotRegister();

  loadControlRegister(B10000001); // set serial mode. see table 1, footnote 1
  // set control register 0 for max brightness, and no sleep:
  // added: ML send multiple inits to 2nd, 3rd, 4th display, etc.
  // set control register 0 for max brightness, and no sleep:

  // Set the brightness to max for each 4-character section
  for (int d = 0; d < displayLength / 4; d++)
    loadControlRegister(B01111111);
  
  // set control register 1 so all 8 characters display:
  // loadControlRegister(B10000001); 
}

/*
 *  Clear the display
 */
 
void hcms29xx::clear() {
 for (int displayPos = 0; displayPos < displayLength; displayPos++) {
    char charToShow = ' ';
      // put the character in the dot register:
    writeCharacter(charToShow, displayPos);  
    }

    // send the dot register array out to the display:
    loadDotRegister();
}


/*
 *  set the cursor to the home position (0)
 */
void hcms29xx::home() {
    // set the cursor to the upper left corner:
    this->cursorPos = 0;
}

/*
 *  set the cursor anywhere
 */
void hcms29xx::setCursor(int whichPosition){
    this->cursorPos = whichPosition;
}

/*
 *  return the cursor position
 */
 
int hcms29xx::getCursor() {
    return this->cursorPos;
}

/*
 *  write a byte out to the display at the cursor position,
 *  and advance the cursor position.
 */
 
#if ARDUINO >= 100
size_t hcms29xx::write(uint8_t b) {
#else
void hcms29xx::write(uint8_t b) {
#endif
    // make sure cursorPos is on the display:
    if (cursorPos >= 0 && cursorPos < displayLength) {  
        // put the character into the dot register:
        writeCharacter(b, cursorPos);
        // put the character into the displayString:
        if (cursorPos < this->stringLength()) {
            this->displayString[cursorPos] = b;
        }       
        cursorPos++;    
        // send the dot register array out to the display:
        loadDotRegister();
    }
#if ARDUINO >= 100
    return 1;
#endif
}


/*
 *  Scroll the displayString across the display.  left = -1, right = +1
 */


void hcms29xx::scroll(int direction) {
    clear();
    cursorPos += direction;
    // Loop over the string and take displayLength characters to write to the display:
    for (int displayPos = 0; displayPos < displayLength; displayPos++) {
      // which character in the strings you want:
      int whichCharacter =  displayPos - cursorPos;
      //  length of the string to display:
      int stringEnd = strlen(displayString);
     // which character you want to show from the string:
      char charToShow; 
      // display the characters until you have no more:
      if ((whichCharacter >= 0) && (whichCharacter < stringEnd)) {
        charToShow = displayString[whichCharacter]; 
      } 
      // if none of the above, show a space:
      else {
        charToShow = ' ';
      }
      // put the character in the dot register:
      writeCharacter(charToShow, displayPos);  
    }
    // send the dot register array out to the display:
    loadDotRegister();
}


/*
 *  set displayString
 */

void hcms29xx::setString(char* _displayString)  {
    this->displayString = _displayString;
}


/*
 *  return displayString
 */

char* hcms29xx::getString() {
    return displayString;
}


/*
 *  return displayString length
 */

    
int hcms29xx::stringLength() {
    return strlen(displayString);
}   
    
    

/*
 *  set brightness (0 - 15)
 */

    
void hcms29xx::setBrightness(uint8_t bright) {

    /*
    Update by andrewzuku (andrew@retrojdm.com)
    Added peakCurrentBrightness
    2014-05-15

    This function originally had the Peak Pixel current at 100%.
    I had problems with my 3x HCMS2904 (4 character) displays randomly blanking when the brightness was too high.
    Setting the peak pixel current to 50% seems to work for me (73% still blanked somtimes).

    I also had to add a for loop to send the brightness command to each individual display.
    
    See "Table 2. Control Shift Register" (page 12 of the datasheet).

    Control Typical Peak    Relative Full
            Pixel Current   Scale Current
            (mA)            (Relative Brightness, %)
    -------------------------------------------------
    H L     4.0              31
    L H     6.4              50
    L L     9.3              73 (Default at Power Up)
    H H     12.8            100
    */
    static const uint8_t RELATIVE_BRIGHTNESS_31     = B10;
    static const uint8_t RELATIVE_BRIGHTNESS_50     = B01;
    static const uint8_t RELATIVE_BRIGHTNESS_73     = B00;
    static const uint8_t RELATIVE_BRIGHTNESS_100    = B11;

    // This is a bit of a messy hack, but I didn't want to alter Tom Igoe's library too much to include a "Characters per Display" attribute.
    uint8_t charsPerDisplay         = 4;

    uint8_t peakCurrentBrightness   = RELATIVE_BRIGHTNESS_31; // %
    uint8_t controlRegister         = B01000000 | (peakCurrentBrightness << 4) | bright;

    /*
    See "Control Word 0" on page 9 of the datasheet.

    D7      = Select Control Word 0
    D6      = Set High for normal operation (low = sleep mode)
    D5..D4  = Brightness via Peak pixel Current
    D3..D0  = Brightness via PWM
    */

    // Set the brightness for each display.
    for (int d = 0; d < displayLength / charsPerDisplay; d++) loadControlRegister(controlRegister);
}


/* this method loads bits into the dot register array. It doesn't
 * actually communicate with the display at all,
 * it just prepares the data:
*/

void hcms29xx::writeCharacter(char whatCharacter, byte whatPosition) {
  // calculate the starting position in the array.
  // every character has 5 columns made of 8 bits:
  byte thisPosition =  whatPosition * 5;

  // copy the appropriate bits into the dot register array:
  for (int i = 0; i < 5; i++) {
    dotRegister[thisPosition+i] = (pgm_read_byte(&Font5x7[((whatCharacter - 0x20) * 5) + i]));
  }
}


// This method sends 8 bits to one of the control registers:
void hcms29xx::loadControlRegister(int dataByte) {
  // select the control registers:
  digitalWrite(registerSelect, HIGH);
  // enable writing to the display:
  digitalWrite(chipEnable, LOW);
  // shift the data out:

  shiftOut(dataPin, clockPin, MSBFIRST, dataByte);
  // disable writing:
  digitalWrite(chipEnable, HIGH);
}

// this method originally sent 320 bits to the dot register: 12_30_09 ML
void hcms29xx::loadDotRegister() {

  // define max data to send, patch for 4 length displays by KaR]V[aN
  int maxData = displayLength * 5;

  // select the dot register:
  digitalWrite(registerSelect, LOW);
  // enable writing to the display:
  digitalWrite(chipEnable, LOW);
  // shift the data out:
  for (int i = 0; i < maxData; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, dotRegister[i]);
  }
  // disable writing:
  digitalWrite(chipEnable, HIGH);
}

/*
  version() returns the version of the library:
*/
int hcms29xx::version(void)
{
  return 4;
}

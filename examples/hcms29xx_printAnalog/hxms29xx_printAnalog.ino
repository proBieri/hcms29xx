/*
HCMS-29xx Display 
Language: Arduino/Wiring

Displays an analog value on an Avago HCMS-297x display.

http://wiring.org.co/learning/reference/String.html

Created:    12 Jun 2008
Modified:   17 Apr 2009 by Tom Igoe
Modified:   10 Mar 2016 by Andrew Wyatt
*/

#include <hcms29xx.h>

// Define pins for the LED display. 
// You can change these, just re-wire your board:
#define dataPin         2 // connects to the display's data in
#define registerSelect  3 // the display's register select pin 
#define clockPin        4 // the display's clock pin
#define enable          5 // the display's chip enable pin
#define register        6 // the display's reset pin

#define displayLength   8 // number of characters in the display

// create am instance of the LED display library:
hcms29xx myDisplay = hcms29xx(dataPin, registerSelect, clockPin, enable, reset, displayLength);

// screen brightness
int brightness = 15;


void setup() {
  
    // initialize the display library:
    myDisplay.begin();
    
    // set the brightness of the display:
    myDisplay.setBrightness(brightness);
    
    Serial.begin(9600);
    Serial.println(myDisplay.version(), DEC);
}


void loop() {
  
    // set the cursor to 1:
    myDisplay.setCursor(1);
    myDisplay.print("A0: ");
    myDisplay.print(analogRead(0), DEC);
}
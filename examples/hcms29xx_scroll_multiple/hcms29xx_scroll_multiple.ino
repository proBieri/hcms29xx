/*
HCMS-29xx Display 
Language: Arduino/Wiring

Displays a string on a three Avago HCMS-297x displays.
Scrolls the string left and right.

http://wiring.org.co/learning/reference/String.html

Created:    12 Jun 2008
Modified:   11 Mar 2010 by Tom Igoe
Modified:   10 Mar 2016 by Andrew Wyatt
*/

#include <HCMS29xx.h>

// Define pins for the LED display. 
// You can change these, just re-wire your board:
#define dataPin         2   // connects to the display's data in
#define registerSelect  3   // the display's register select pin 
#define clockPin        4   // the display's clock pin
#define enable          5   // the display's chip enable pin
#define reset           6   // the display's reset pin

#define displayLength   24  // number of characters in the display

// create am instance of the LED display library:
HCMS29xx myDisplay = HCMS29xx(dataPin, registerSelect, clockPin, enable, reset, displayLength);

// screen brightness
int brightness = 15;

// direction of scrolling. -1 = left, 1 = right.
int myDirection = 1;


void setup() {
  
    Serial.begin(9600);
    
    // initialize the display library:
    myDisplay.begin();
    myDisplay.clear();
    myDisplay.setString("Aardvarks mark the park after dark.");
    myDisplay.setBrightness(brightness);
    delay(100);
}


void loop() {

  // when the string scrolls off the display, reverse scroll direction.
  // On the right, it scrolls off at position 8.
  // on the left, it scrolls off when the cursor is less than -(the length of the string):
  if ((myDisplay.getCursor() > displayLength) ||
      (myDisplay.getCursor() <= -(myDisplay.stringLength()))) {
      myDirection = -myDirection;
      delay(1000);
  }
  
  // scroll:
  myDisplay.scroll(myDirection);
  delay(100);
}
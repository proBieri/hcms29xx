/*
HCMS-29xx Display 
Language: Arduino/Wiring

Displays a string on an Avago HCMS-297x display and fades it up and down.

http://wiring.org.co/learning/reference/String.html

Created:    10 Apr 2009
Modified:   11 Mar 2010 by Tom Igoe
Modified:   10 Mar 2016 by Andrew Wyatt
*/

#include <HCMS29xx.h>

// Define pins for the LED display. 
// You can change these, just re-wire your board:
#define dataPin         2 // connects to the display's data in
#define registerSelect  3 // the display's register select pin 
#define clockPin        4 // the display's clock pin
#define enable          5 // the display's chip enable pin
#define reset           6 // the display's reset pin

#define displayLength   8 // number of characters in the display

// create am instance of the LED display library:
HCMS29xx myDisplay = HCMS29xx(dataPin, registerSelect, clockPin, enable, reset, displayLength);

// screen brightness
int brightness = 15;


void setup() {

    Serial.begin(9600);

    // initialize the display library:
    myDisplay.begin();
}


void loop() {

    for (int brightness = 0; brightness < 16; brightness++) {
      
        // set the display string brightness:
        myDisplay.setBrightness(brightness);
        
        // set the cursor to position 1:
        myDisplay.setCursor(1);
        
        // print to the display:
        myDisplay.print("Fading");

        delay(50);
    }
    delay(100);

    for (int brightness = 15; brightness >= 0; brightness--) {
      
        // set the display string brightness:
        myDisplay.setBrightness(brightness);
        
        // set the cursor to position 1:
        myDisplay.setCursor(1);
        
        // print to the display:
        myDisplay.print("Fading");

        delay(50);
    }  
    delay(300);
}
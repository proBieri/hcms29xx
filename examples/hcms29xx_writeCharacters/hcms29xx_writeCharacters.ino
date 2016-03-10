/*
HCMS-29xx Display 
Language: Arduino/Wiring

Writes characters on an Avago HCMS-297x display

http://wiring.org.co/learning/reference/String.html

Created:    12 Jun 2008
Modified:   11 Mar 2010 by Tom Igoe
Modified:   10 Mar 2016 by Andrew Wyatt
*/

#include <hcms29xx.h>

#define maxStringLength 180  // max string length

// Define pins for the LED display. 
// You can change these, just re-wire your board:
#define dataPin         2 // connects to the display's data in
#define registerSelect  3 // the display's register select pin 
#define clockPin        4 // the display's clock pin
#define enable          5 // the display's chip enable pin
#define reset           6 // the display's reset pin

#define displayLength   8 // number of characters in the display

// create am instance of the LED display:
hcms29xx myDisplay = hcms29xx(dataPin, registerSelect, clockPin, enable, reset, displayLength);

// screen brightness
int brightness = 15;

char myString[] = { 'p','r','i','n','t','i','n','g' };


void setup() {
  
    Serial.begin(9600);

    // initialize the display library:
    myDisplay.begin();
    myDisplay.setString("Printing");
    myDisplay.home();
    myDisplay.setBrightness(brightness);
}


void loop() {

    for (int thisPosition = 0; thisPosition < 8; thisPosition++) {
        
        for (int thisChar = ' '; thisChar < 'z'; thisChar++) {
            
            myDisplay.write(thisChar);
            myDisplay.setCursor(thisPosition);
            delay(3);
        }
        
        myDisplay.write(myString[thisPosition]);
        delay(10);
    }

    delay(500);
    myDisplay.clear();
    myDisplay.home();
}

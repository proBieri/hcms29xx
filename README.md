hcms29xx Arduino Library
========================

An Arduino library for the Avago HCMS-29xx LED Displays (slightly modified by me (Andrew Wyatt) to allow multiples of 4 characters, and direct access to the Dot Register).

For details on the original version, see http://www.arduino.cc/playground/Main/LedDisplay

--------------------------------------------------------------------------------

This library allows you to send text to an Avago HCMS-29xx LED display. The HCMS 29xx displays are pretty little displays that contain a row of 5x7 LED matrices. 

The displays have a synchronous serial interface. You'll need five digital output lines to control them.

The pins are as follows:
* **data:** bits in from the microcontroller
* **register select:** selects whether you're sending display data or control data
* **clock:** timing clock from the microcontroller
* **enable:** enables or disables the display
* **reset:** resets the display 

The library manages all the necessary pin control and data shifting for you.

--------------------------------------------------------------------------------



Class Constructor / Initialisation
----------------------------------

### hcms29xx(int dataPin, int registerSelect, int clockPin, int chipEnable, int resetPin, int displayLength)

Instantiates the library. The first five parameters are the Arduino pin numbers that are connected to the display. The last sets the length of the display (8 for the HCMS-291x and HCMS-297x, 4 for the HCMS-290x and 296x).

**example:**

```c
hcms29xx myDisplay = hcms29xx(2,3,4,5,6,8);
```



### begin()

Initializes and resets the display.

**example:**

```c
myDisplay.begin();
```



Printing Methods
----------------

#### write(char whatCharacter, byte whatPosition)

Writes a single character to the display at a particular position.

**example:**

```c
myDisplay.write('A', 4);
```

LedDisplay inherits all the print() and println() methods from the Print library, so you can use those methods to print to the display, as long as what you print fits within the display. For example here's how to print the millis and a title string to the display:

```c
myDisplay.print("ms:");
myDisplay.print(millis());
```

You can also do the usual Print things, like:

```c
int myInt = 12;
myDisplay.print(myInt, DEC);
// or
myDisplay.print(myInt, HEX);
```



Scrolling Methods
-----------------

If you want to be able to automatically scroll a string of text, you need to use the setString() method first. This method stores a character string in the library instance, so you can then use the scroll() method to move it left and right.

setString(char* stringToDisplay) - displays a string on the display. If the string is longer than the display, the beginning of the string is displayed. You can use the scroll() method to step the string forward on the display.




### getString()

Returns the string to display.

**example:**

```c
// print out the display string for debugging:
Serial.println(myDisplay.getString());
```



### stringLength()

Returns the length of the display string. See the scrolling example below for usage.



### scroll(int direction)

Scrolls the display using the string set by setString(). Negative numbers scroll left, positive numbers scroll right.

**example:**

```c
//...

void setup() { 

    //...

    // scroll a string that's longer than the display:
    myDisplay.setString("This is a very long string");
}

void loop() {

    // when the string scrolls off the display, reverse scroll direction.
    // On the right, it scrolls off at position 8.
    // on the left, it scrolls off when the cursor is less than -(the length of the string):

    if ((myDisplay.cursorPosition() > displayLength) ||
        (myDisplay.cursorPosition() <= -(myDisplay.stringLength()))) {
        myDirection = -myDirection;
        delay(1000);
    }

    // scroll:
    myDisplay.scroll(myDirection);
    delay(100);
}
```



Cursor Control Methods
----------------------

### clear()

Clear the display.

**example:**

```c
myDisplay.clear();
```



### home()

Set cursor to far left hand position.

**example:**

```c
myDisplay.home();
```



### setCursor(int whichPosition)

Set cursor to any position.

**example:**

```c
myDisplay.serCursor(4);
```



### getCursor()

Get the cursor position.

**example:**

```c
int cursorPosition = myDisplay.getCursor();
```



Display Control Methods
-----------------------

### setBrightness(int bright)

Lets you set the brightness from 0 to 15.

**example:**

```c
myDisplay.setBrightness(15);
```



Direct Manipulation Methods
---------------------------

If you want to set the opcodes of the display directly (you can learn them from the data sheet), the following methods will do the trick.




### loadControlRegister(int dataByte)

Sends 8 bits to one of the control registers.




### loadDotRegister() 

Sends 320 bits to the dot register




Version Method
--------------

### version()

Returns the version number of the library. 
# Lab 2

> Frances Coronel
>
> Mitchell Wear Napolitano
>
> Sindhu Banavara Ravindra

## Objective

The objective of this lab is to build a Morse code receiver/decoder. It must accept Morse code (digits 0-9) as input via user button presses, and output the result to a dot matrix display.

## Executive summary

We connected the USB cable to the `Arduino Fio` and set up the `IDE` so we can could compile and run our `.ino` code to the Arduino.

That context was used to allow the Arduino to read the digit that the console was given as input and output the number using the `LED` light on the Arduino.

### How your debouncing algorithm works and why you chose it (i.e. How is it better than alternatives for this application?)

### How you recognize and decode user inputs, including dealing with variations in timing and erroneous inputs?

## System-Level Block Diagram

![diagram](diagram.png)

## Software Description

Each digit in Morse Code is represented with a series of dots and dashes.

For example, the digit `3` in Morse Code is represented by 3 dots and 2 dashes: `...--`.

The 0-9 digits were stored in a `String` type array with actual `.` and `-` representing the dots and dashes, respectively, as shown in the example above.

Functions were created to represent how long the `LED` light would blink for both the `dot` and `dash` functions.

Based on what digit is input into the console, the dot and dash functions were called a certain number of times.

In turn, these dot and dash functions turned on and delayed the `LED` light on the Arduino Fio by certain time spans.

## References

- [Wikipedia - Morse Code - Chart of the Morse code letters and numbers](https://www.wikiwand.com/en/Morse_code)
    * We used this to see how the 0-9 digits are translated via dots and dashes to put into our array.
- [Arduino - Reference Sheet](https://www.arduino.cc/en/Reference/HomePage)
    * Used the reference sheet to learn language syntax specific to Arduino. For example the function `splitstring`
- Arduino - Built-in examples
    * Used the built-in examples in the Arduino software to learn how to initialize the serial port, read from serial port, initialize the LED-light and make the LED-light blink.

----

## TO READ

Shift Registers
Shift registers
When you read the datasheet for the LED dot matrix, and look at your arduino, you will notice that the arduino does not have enough pins to run the display.  One way to solve this problem is using a serial in, parallel out (SIPO) shift register.  The basic idea is that you can use one arduino pin to send a series of signals to the register (serial in), which will store those signals and then output all at once (parallel out) on a set of output pins.

Read more about shift registers on the Wikipedia article.

You will be given one shift register for this assignment.  The part numbers vary, so you should look at the part number on the chip and then search google for the datasheet to find out how to operate it.



LED dot matrix
You will be displaying the numbers on the LED dot matrix we handed out in class.  You can view this just as it sounds: it is a bunch of LEDs wired in a grid.  The circuit diagram for the matrix is included in the datasheet.  Remember that a resistor is required in the LED circuit.

The datasheet lists two part numbers.  The part number that has been handed out is the LTP-757G.

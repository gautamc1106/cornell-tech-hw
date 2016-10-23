# Lab 3

> Frances Coronel
>
> Varad Kishore
>
> Nishad Prinja

## Objective

The objective of this lab is to create a wireless measurement device. The Arduino Fio and the range finder will be used to build a hand held device which which can measure the distance from the device.  The device will have a button, which will trigger transmission of the measurement readings to a computer using the Xbee.

## Executive Summary

The `Xbee` modules were appropriately setup on the board so they could communicate with each other. We had to download the XCTU program in order to test this.

Once we verified they could communicate, we set up the single sonic range finder to transmit the distance every 500 ms to the computer.

## System-Level Block Diagram

[INSERT DIAGRAM HERE]

## Software Description

We set up the serial port and initialized the relevant button and LED pins and states.

The interface for XBee was then setup using the serial port through the `Software Serial Library`.

There was a loop made to check what the distance was at the current time and then that distance was printed to the computer using `Xbee`'s module. This happened every 500 ms.

## References

- [Exploring Xbees](https://learn.sparkfun.com/tutorials/exploring-xbees-and-xctu)
- [Ultrasonic Range Finder - Data Sheet](http://maxbotix.com/documents/HRLV-MaxSonar-EZ_Datasheet.pdf)
- [Distance Detection with MaxSonar](http://playground.arduino.cc/Main/MaxSonar)
- [Software Serial Library](https://www.arduino.cc/en/Reference/SoftwareSerial)
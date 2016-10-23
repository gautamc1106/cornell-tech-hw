# Description

Your task is to create a wireless measurement device.  You will use the Arduino and the range finder to build a handheld device which which can measure the distance from the device.  The device will have a button, which will trigger transmission of the measurement readings to a computer using the XBee.

To complete the lab, you will need two XBee modules and an XBee explorer.

# Wireless setup

The XBee modules can be used for wireless communication using the standard Serial interface. They must be appropriately initialized for this to work. Detailed instructions on setting up two XBee modules so that they can communicate with each other can be found here.

Go through the tutorial and make sure your two XBee devices can communicate with each other. While the tutorial is written for any type of Arduino and mentions an Arduino shield, the Arduino Fio already has the necessary connector for the XBee. It is on the opposite side of the board compared to the serial connection used to program the Fio.

# The lab

The ultrasonic range finder can be used to report an analog signal that corresponds to distance. (Data sheet available from this link).You will need to design a board with the Arduino, ultrasonic range finder, and button to record the distance.

Make sure the following two pieces work before doing the final part of the lab.

Make sure you can have the Arduino Fio communicate with the laptop via wireless
Make sure the Arduino Fio can report the distance when the button is pressed.
After this is done, combine the two solutions and use the battery to power the Fio.
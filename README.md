MAX6954 LED Driver Library
==========================

This library was written to control one or two MAX6954 chips with an Arduino.

Wiring
------

I connected the MAX6954s to the arduino using the SPI pins (see the
[connections](http://arduino.cc/en/Reference/SPI) section from the arduino
reference page on SPI). For the Uno or Duemilanove the pins are:

* Master out slave in (MOSI) or DOUT - Pin 11
* Master in slave out (MISO) or DIN - Pin 12
* Clock (SCK) or CLK - Pin 13
* Chip 1 Select (SS) or CS - Pin 9
* Chip 2 Select (SS) or CS - Pin 8

The [MAX6954 Data Sheet](http://datasheets.maximintegrated.com/en/ds/MAX6954.pdf)
says that the chips can be chained (see Figure 2. MAX6954 Daisy-Chain
Connection) but I was getting data corruption with my setup. The corruption completely went away when I controlled each chip individually.

To connect the MAX6954 to your 16 segment digits refer to Table 2. Connection Scheme for Eight 16-Segment Digits and Figure 1. Segment Labeling for 7-Segment Display, 14-Segment Display, and 16-Segment Display in the [data sheet](http://datasheets.maximintegrated.com/en/ds/MAX6954.pdf). I used [these digits](http://shop.evilmadscientist.com/productsmenu/partsmenu/232).

Code
----

To get started see the example provided in the library.

#include <SPI.h>
#include "max6954.h"

#define CS1        9  // MAX6954 1
#define CS2        8  // MAX6954 2
#define DATAOUT    11 // MOSI
#define DATAIN     12 // MISO
#define CLK        13 // sck

MAX6954 alpha_board = MAX6954(DATAOUT, DATAIN, CLK, CS1, CS2);

void setup() {
  alpha_board.begin(); // Initialize SPI Connections
  alpha_board.set_global_brightness(15); // Set brightness level (0-15)

  // Disable decode mode so we can control each segment individually
  alpha_board.disable_decode_mode();
}

void loop() {
  int i = 0;

  // Each Digit is controlled by two integers values between 0 and 255. Each
  // integer is stored in two different registers on the max6954:
  // Digit 1: 0x20 and 0x28
  // Digit 2: 0x21 and 0x29
  // Digit 3: 0x22 and 0x2a
  // Digit 4: 0x23 and 0x2b
  // Digit 5: 0x24 and 0x2c
  // Digit 6: 0x25 and 0x2d
  // Digit 7: 0x26 and 0x2e
  // Digit 8: 0x27 and 0x2f

  // Note: When you connect 8 16-segment digits to the max6954 using the
  // connection scheme in the datasheet on Table 2. Connection Scheme for Eight
  // 16-Segment Digits you cannot turn on the upper right segment. Every segment
  // including the decimal place will work except for the upper right one.

  // Turn off all segments
  for (i=0x20; i<=0x2F; i++) {
    alpha_board.write(i, B00000000);
  }

  // Incrementally turn on each digit using individual segment control
  for(i=0x20; i<0x28; i++) {
    // Turn on all segments of digit i
    alpha_board.write(i, B11111111);
    alpha_board.write(i+8, B11111111);
    delay(1000);
  }

  // Turn off all segments
  for (int i=0x20; i<=0x2F; i++) {
    alpha_board.write(i, B00000000);
  }

  // Spinning Clock Animation on the first Digit
  int dl = 80;
  for (int i=0; i<4; i++) {
    alpha_board.write(0x20,B00000001);
    delay(dl);
    alpha_board.write(0x20,B00000000);
    alpha_board.write(0x28,B00100000);
    delay(dl);
    alpha_board.write(0x28,B00010000);
    delay(dl);
    alpha_board.write(0x28,B00001000);
    delay(dl);
    alpha_board.write(0x28,B01000000);
    delay(dl);
    alpha_board.write(0x28,B00000100);
    delay(dl);
    alpha_board.write(0x28,B00000010);
    delay(dl);
    alpha_board.write(0x28,B00000001);
    delay(dl);
  }

  while (1){
    // Set each digit to a random value
    for (i=0x20; i<=0x2F; i++) {
      alpha_board.write_chip1(i, (uint8_t) random(0, 256));
      alpha_board.write_chip2(i, (uint8_t) random(0, 256));
    }
    delay(100);
  }
}


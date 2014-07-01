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

  // use control_intensity to change the brightness level of each digit
  alpha_board.enable_individual_segment_brightness();
  // Set all digits to level 0 brightness (15 is the highest)
  alpha_board.control_intensity(0,0,0,0,0,0,0,0);
}

// brighness levels for the larson scanner
int brightness1 = 4;
int brightness2 = 8;
int brightness3 = 12;

// delay between each frame of the larson scanner
int dl = 50;

void loop() {
  // set brightness of each digit
  alpha_board.control_intensity(0, 2, 4, 6, 8, 10, 12, 14);
  // output "********" on both chips (row = -1)
  alpha_board.write_string("********", -1, 0);
  delay(5000);

  // output "abcdefgh" on chip 1 (row = 0)
  alpha_board.write_string("abcdefgh", 0, 0);
  // output "ijklmnop" on chip 2 (row = 1)
  alpha_board.write_string("ijklmnop", 1, 0);
  delay(5000);

  // run a larson scanner animation on both chips
  while (1) {
    for (int i=0; i<2; i++) {
      alpha_board.control_intensity(0, brightness1, brightness2, brightness3, 0, 0, 0, 0);
      alpha_board.write_string("****    ", -1, 0);
      delay(dl);

      alpha_board.control_intensity(0, 0, brightness1, brightness2, brightness3, 0, 0, 0);
      alpha_board.write_string(" ****   ", -1, 0);
      delay(dl);

      alpha_board.control_intensity(0, 0, 0, brightness1, brightness2, brightness3, 0, 0);
      alpha_board.write_string("  ****  ", -1, 0);
      delay(dl);

      alpha_board.control_intensity(0, 0, 0, 0, brightness1, brightness2, brightness3, 0);
      alpha_board.write_string("   **** ", -1, 0);
      delay(dl);

      alpha_board.control_intensity(0, 0, 0, 0, 0, brightness1, brightness2, brightness3);
      alpha_board.write_string("    ****", -1, 0);
      delay(dl);

      alpha_board.control_intensity(0, 0, 0, 0, 0, 0, brightness3, brightness2);
      alpha_board.write_string("     ***", -1, 0);
      delay(dl);

      alpha_board.control_intensity(0, 0, 0, 0, 0, brightness3, brightness2, brightness1);
      alpha_board.write_string("     ***", -1, 0);
      delay(dl);

      alpha_board.control_intensity(0, 0, 0, 0, brightness3, brightness2, brightness1, 0);
      alpha_board.write_string("    ****", -1, 0);
      delay(dl);

      alpha_board.control_intensity(0, 0, 0, brightness3, brightness2, brightness1, 0, 0);
      alpha_board.write_string("   **** ", -1, 0);
      delay(dl);

      alpha_board.control_intensity(0, 0, brightness3, brightness2, brightness1, 0, 0, 0);
      alpha_board.write_string("  ****  ", -1, 0);
      delay(dl);

      alpha_board.control_intensity(0, brightness3, brightness2, brightness1, 0, 0, 0, 0);
      alpha_board.write_string(" ****   ", -1, 0);
      delay(dl);

      alpha_board.control_intensity(brightness3, brightness2, brightness1, 0, 0, 0, 0, 0);
      alpha_board.write_string("****    ", -1, 0);
      delay(dl);

      alpha_board.control_intensity(brightness2, brightness3, 0, 0, 0, 0, 0, 0);
      alpha_board.write_string("***     ", -1, 0);
      delay(dl);

      alpha_board.control_intensity(brightness1, brightness2, brightness3, 0, 0, 0, 0, 0);
      alpha_board.write_string("***     ", -1, 0);
      delay(dl);
    }
  }
}


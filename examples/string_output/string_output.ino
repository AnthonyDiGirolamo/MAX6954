#include <avr/pgmspace.h>
#include <SPI.h>
#include "max6954.h"

#define CS1        9  // MAX6954 1
#define CS2        8  // MAX6954 2
#define DATAOUT    11 // MOSI
#define DATAIN     12 // MISO
#define CLK        13 // sck

MAX6954 alpha_board = MAX6954(DATAOUT, DATAIN, CLK, CS1, CS2);

// Some strings to print, stored in program memory (saves RAM)

char string_empty[]    PROGMEM = "                ";
char string_lights[]   PROGMEM = "Lights  ";
char string_out[]      PROGMEM = "Out     ";

// Helper function to print PROGMEM strings

void max_print_progmem(char* string, uint8_t row = -1, uint8_t col = -1, bool lowercase = 0) {
  char buffer[32];
  strcpy_P(buffer, string);
  if (lowercase) {
    alpha_board.disable_decode_mode();
    alpha_board.write_lowercase_string(buffer,
        row>=0 ? row : 0,
        col>=0 ? col : 0);
  }
  else {
    alpha_board.enable_decode_mode();
    alpha_board.write_string(buffer,
        row>=0 ? row : 0,
        col>=0 ? col : 0);
  }
}

void setup() {
  alpha_board.begin(); // Initialize SPI Connections
  alpha_board.set_global_brightness(4); // Set brightness level (0-15)
}

void loop() {
  alpha_board.enable_decode_mode(); // Enable MAX6954 Built-in Font

  // Output Lights Out
  max_print_progmem(string_lights, 0, 0); // write 'Lights' to first chip (row 0)
  max_print_progmem(string_out, 1, 0);    // write 'Out' to second chip (row 1)
  delay(2000);

  // Disable MAX6954 Built-in Font to allow individual segment control for lower case
  alpha_board.disable_decode_mode();

  // Output Lights Out in lowercase
  max_print_progmem(string_lights, 0, 0, 1); // write 'Lights' to first chip (row 0)
  max_print_progmem(string_out, 1, 0, 1);    // write 'Out' to second chip (row 1)
  delay(2000);

  // Write lowercase letters
  // in-line strings like this are stored in RAM
  alpha_board.write_lowercase_string("Grumpy  wizards ",0,0);
  delay(2000);
  alpha_board.write_lowercase_string("make    toxic   ",0,0);
  delay(2000);
  alpha_board.write_lowercase_string("brew forthe     ",0,0);
  delay(2000);
  alpha_board.write_lowercase_string("evil    Queen   ",0,0);
  delay(2000);
  alpha_board.write_lowercase_string("and Jack        ",0,0);
  delay(2000);
}


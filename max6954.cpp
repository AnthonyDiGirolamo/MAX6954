#include "Arduino.h"
#include "max6954.h"
#include <SPI.h>

prog_uint8_t lowercase_character_map[][2] = {
  { B00000000, B00000000 }, // space
  { B00000000, B00000000 }, // ! 32
  { B00000000, B00000000 }, // "
  { B00000000, B00000000 }, // #
  { B00000000, B00000000 }, // $
  { B00000000, B00000000 }, // %
  { B00000000, B00000000 }, // &
  { B00000000, B00000000 }, // '
  { B00000000, B00000000 }, // (
  { B00000000, B00000000 }, // )
  { B00000000, B00000000 }, // *
  { B00000000, B00000000 }, // +
  { B00000000, B00000000 }, // ,
  { B00000000, B00000000 }, // -
  { B00000000, B00000000 }, // .
  { B00000000, B00000000 }, // /
  { B00000000, B00000000 }, // 0
  { B00000000, B00000000 }, // 1
  { B00000000, B00000000 }, // 2
  { B00000000, B00000000 }, // 3
  { B00000000, B00000000 }, // 4
  { B00000000, B00000000 }, // 5
  { B00000000, B00000000 }, // 6
  { B00000000, B00000000 }, // 7
  { B00000000, B00000000 }, // 8
  { B00000000, B00000000 }, // 9
  { B00000000, B00000000 }, // :
  { B00000000, B00000000 }, // ;
  { B00000000, B00000000 }, // <
  { B00000000, B00000000 }, // =
  { B00000000, B00000000 }, // >
  { B00000000, B00000000 }, // ?
  { B00000000, B00000000 }, // @
  { B10001101, B00000010 }, // A
  { B10000111, B00000010 }, // B
  { B10000101, B00000000 }, // C
  { B10000101, B00010010 }, // D
  { B10000101, B00000001 }, // E
  { B01000111, B00000000 }, // F
  { B11000011, B00010010 }, // G
  { B00000111, B00000010 }, // H
  { B00000100, B00000000 }, // I
  { B10000000, B00000010 }, // J
  { B00000000, B01010110 }, // K
  { B00001000, B00010010 }, // L
  { B00010101, B01000010 }, // M
  { B00000101, B00000010 }, // N
  { B10000101, B00000010 }, // O
  { B01000111, B00010000 }, // P
  { B01001011, B00010010 }, // Q
  { B00000101, B00000000 }, // R
  { B00001000, B01000100 }, // S
  { B00001001, B01010010 }, // T
  { B10000100, B00000010 }, // U
  { B00000100, B00000001 }, // V
  { B10011100, B00000010 }, // W
  { B00000000, B00101101 }, // X
  { B10000011, B00010010 }, // Y
  { B10000001, B00000001 }, // Z
  { B00000000, B00000000 }, // [
  { B00000000, B00000000 }, // backslash
  { B00000000, B00000000 }, // ]
  { B00000000, B00000000 }, // ^
  { B00000000, B00000000 }, // _
  { B00000000, B00000000 } // ` 96
// { 123
// |
// }
// ~
};

MAX6954::MAX6954(uint8_t out, uint8_t in, uint8_t clk, uint8_t c1, uint8_t c2) {
  data_out = out;
  data_in  = in;
  clock    = clk;
  chip1    = c1;
  chip2    = c2;

  pinMode(data_out, OUTPUT);
  pinMode(data_in, INPUT);
  pinMode(clock, OUTPUT);

  pinMode(chip1, OUTPUT);
  pinMode(chip2, OUTPUT);

  digitalWrite(chip1, HIGH); // disable device
  digitalWrite(chip2, HIGH); // disable device
}

void MAX6954::begin() {
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  // SPI Speed = 1MHz
  // SPI.setClockDivider(SPI_CLOCK_DIV8);  // 8MHz Arduino
  SPI.setClockDivider(SPI_CLOCK_DIV16); // 16MHz Arduino
  // SPI_CLOCK_DIV possibilities 2,4,8,16,32,64,128

  // Decode mode enabled
  write(0x01, B11111111);
  // Scan Limit - all digits
  write(0x03, B00000111);
  // Configuration - fast blinking and normal operation, invidual digit brightness
  //write(0x04, B01000101);
  // Configuration - fast blinking and normal operation, global brightness
  write(0x04, B00000101);
  // Digit Type - all 16 segments
  write(0x0C, B00000000);
}

void MAX6954::read(byte address, uint8_t cs){
  //Serial.print(address, HEX);
  //Serial.print(" = ");

  digitalWrite(cs, LOW);
  SPI.transfer(address | B10000000);
  SPI.transfer(0x00);
  digitalWrite(cs, HIGH);

  digitalWrite(cs, LOW);
  SPI.transfer(address);
  //Serial.println(SPI.transfer(0x00), BIN);
  digitalWrite(cs, HIGH);
}

void MAX6954::write(byte address, byte value) {
  digitalWrite(chip1, LOW);
  digitalWrite(chip2, LOW);

  SPI.transfer(address);
  SPI.transfer(value);

  digitalWrite(chip1, HIGH);
  digitalWrite(chip2, HIGH);
}

void MAX6954::write_chip1(byte address, byte value) {
  digitalWrite(chip1, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(chip1, HIGH);
}

void MAX6954::write_chip2(byte address, byte value) {
  digitalWrite(chip2, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(chip2, HIGH);
}

void MAX6954::control_intensity(int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7) {
  write(0x10, d1 << 4 | d0);
  write(0x11, d3 << 4 | d2);
  write(0x12, d5 << 4 | d4);
  write(0x13, d7 << 4 | d6);
}

void MAX6954::write_string(char str[], int row, int col){
  int i=0;
  for (int digit = 0x20+col; digit <= 0x27; digit++) {
    if (str[i] == '\0')
      return;
    // If the next character is a period
    if (str[i+1] == '.') {
      if (row==0)
        write_chip1(digit, str[i] | B10000000);
      else if (row==1)
        write_chip2(digit, str[i] | B10000000);
      else
        write(digit, str[i] | B10000000);
      i+=2; // skip the decimal
    }
    else {
      if (row==0)
        write_chip1(digit, str[i]);
      else if (row==1)
        write_chip2(digit, str[i]);
      else
        write(digit, str[i]);
      i++;
    }
  }
}

void MAX6954::write_string1(char str[]){
  write_chip1(0x20, str[0]);
  write_chip1(0x21, str[1]);
  write_chip1(0x22, str[2]);
  write_chip1(0x23, str[3]);
  write_chip1(0x24, str[4]);
  write_chip1(0x25, str[5]);
  write_chip1(0x26, str[6]);
  write_chip1(0x27, str[7]);
}

void MAX6954::write_string2(char str[]){
  write_chip2(0x20, str[0]);
  write_chip2(0x21, str[1]);
  write_chip2(0x22, str[2]);
  write_chip2(0x23, str[3]);
  write_chip2(0x24, str[4]);
  write_chip2(0x25, str[5]);
  write_chip2(0x26, str[6]);
  write_chip2(0x27, str[7]);
}

void MAX6954::write_int1(int value){
  char buffer[16];
  int column = 0x20;
  ultoa(value, buffer, 10);
  int len = strlen(buffer);
  for (int i=0; i<len; i++) {
    write_chip1(column++, buffer[i]);
  }
}

void MAX6954::write_lowercase(char c, int row=0, int col=0) {
  int offset = 32;
  if (c >= 'a')
    offset = 64;
  if (row==0) {
    write_chip1(0x20+col, lowercase_character_map[c-offset][0]);
    write_chip1(0x28+col, lowercase_character_map[c-offset][1]);
  }
  else {
    write_chip2(0x20+col, lowercase_character_map[c-offset][0]);
    write_chip2(0x28+col, lowercase_character_map[c-offset][1]);
  }
}

void MAX6954::write_lowercase_string(char string[], int row=0, int col=0) {
  uint16_t i;
  for (i=0; i<strlen(string); i++) {
    write_lowercase(string[i], row+i/8, col+i%8);
  }
}

void MAX6954::enable_individual_segment_brightness() {
  // Global Intensity to zero - individual segment control
  write(0x02, B00000000);
  // Set Configuration - fast blinking and normal operation, individual brightness
  write(0x04, B01000101);
}

void MAX6954::enable_global_segment_brightness() {
  // Global Intensity to one
  write(0x02, B00000001);
  // Set Configuration - fast blinking and normal operation, global brightness
  write(0x04, B00000101);
}

void MAX6954::disable_decode_mode() {
  write(0x01, B00000000); // Disable Decode Mode
}

void MAX6954::turn_off_individual_segments() {
  // Turn off all segments manually
  for (int i=0x20; i<=0x2F; i++) {
    write(i, B00000000);
  }
}

void MAX6954::enable_decode_mode() {
  write(0x01, B11111111); // Decode mode enabled
}

void MAX6954::set_global_brightness(int i) {
  if (i>15) i=15;
  if (i<0) i=0;
  // Global Intensity to one
  write(0x02, i);
}

void MAX6954::individual_segment_test() {
  // Decode mode disabled
  write(0x01, B00000000);

  // Turn off all segments manually
  for (int i=0x20; i<=0x2F; i++) {
    write(i, B00000000);
  }

  // Turn on all segments manually (decode mode off)
  // write(0x20, B11111111);
  // write(0x28, B11111111);
  // delay(5000);

  int dl = 40;
  for (int i=0; i<4; i++) {
    write(0x20,B00000001);
    delay(dl);
    write(0x20,B00000000);
    write(0x28,B00100000);
    delay(dl);
    write(0x28,B00010000);
    delay(dl);
    write(0x28,B00001000);
    delay(dl);
    write(0x28,B01000000);
    delay(dl);
    write(0x28,B00000100);
    delay(dl);
    write(0x28,B00000010);
    delay(dl);
    write(0x28,B00000001);
    delay(dl);
  }

  for (int i=0x20; i<=0x2F; i++) {
    write(i, B00000000);
  }

  // Test each segment bit slowly
//  dl = 3000;
//  int segment = B10000000;
//  for (int i=0; i<8; i++) {
//    write(0x20,segment);
//    segment = segment >> 1;
//    delay(dl);
//  }
//  write(0x20,B00000000);
//  segment = B10000000;
//  for (int i=0; i<8; i++) {
//    write(0x28,segment);
//    segment = segment >> 1;
//    delay(dl);
//  }
//  write(0x28,B00000000);

  // Decode mode enabled
  write(0x01, B11111111);
}


